#include  "../include/WebPageQuery.h"
#include<algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include<string>
//#include<pair>
#include "../include/SplitCppJieba.h"
#include <unicode/unistr.h>
using std::map;
using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::make_pair;
using std::multimap;
using std::pair;

//Mysql mysql;

WebPageQuery* WebPageQuery::_pInstance=nullptr;
WebPageQuery::AutoRelease WebPageQuery::_ar;

WebPageQuery::WebPageQuery() {
    loadLibrary();
    _jieba=new SplitToolCppJieba();    
}


WebPageQuery::~WebPageQuery() {}

WebPageQuery*WebPageQuery::getInstance()
{
    if(_pInstance==nullptr)
    {
        _pInstance=new WebPageQuery();
    }
    return _pInstance;
}

//预热

void WebPageQuery::loadLibrary()
{

    //1.偏移库读取
    ifstream ifs1("data/newoffset.dat");
    if(!ifs1.good())
    {
        cerr<<"error_in_WebPageQuery::loadLibrary:newoffset.dat打开失败\n";
        return;
    } 
    string line;
    string strId,strStart,strLength;
    int id,start,len;
    while(getline(ifs1,line))
    {
        istringstream iss(line);
        iss>>strId>>strStart>>strLength;
        id=stoi(strId);
        start=stoi(strStart);
        len=stoi(strLength);
        WebPageQuery::_offsetLib.insert(make_pair(id,make_pair(start,len)));
    } 
    ifs1.close();


    //2.网页库读取
    ifstream ifs2("data/newripepage.data");
    if(!ifs2.good())
    {
        cerr<<"error_in_WebPageQuery::loadLibrary:newripepage.dat打开失败\n";
        return;
    } 

    char *buff =new char[1000000];  

    for(auto it=_offsetLib.begin();it!=_offsetLib.end();++it)
    {
        bzero(buff,sizeof(buff));
        ifs2.seekg(it->second.first);
        ifs2.read(buff,it->second.second);
        string strDoc(buff);
        WebPage page(strDoc);
        _pageLib.insert(make_pair(page.getDocId(),page));
    } 

    ifs2.close();
    delete[]buff;
    buff=nullptr;

    cout<<"读取文件数:"<<_pageLib.size()<<endl;

    //3.倒排索引库
    ifstream ifs3("data/invertIndex.txt");
    if(!ifs3.good())
    {
        cerr<<"error_in_WebPageQuery::loadLibrary:invertIndex.txt打开失败\n";
        return;
    }
    string word,strFreq;
    double freq;
    while(getline(ifs3,line))
    {
        istringstream iss(line);
        iss>>word;
        vector<pair<int,double>>idAndFreq;
        while(iss>>strId>>strFreq,!iss.eof())
        {
            freq=stod(strFreq);
            id=stoi(strId);
            idAndFreq.push_back(make_pair(id,freq));
        }
        _invertIndexTable.insert(make_pair(word,idAndFreq));
    }
    ifs3.close();


    //4.中英文停用词库
    ifstream ifs4("data/stop_words_eng.txt");
    if(!ifs4.good())
    {
        cerr<<"error_in_WebPageQuery::loadLibrary:stop_words_eng.txt打开失败\n";
        return;
    }

    while(getline(ifs4,line))
    {
        istringstream iss(line);
        iss>>word;
        _stopWordList.insert(word);
    }
    ifs4.close();



    ifstream ifs5("data/stop_words_zh.txt");
    if(!ifs5.good())
    {
        cerr<<"error_in_WebPageQuery::loadLibrary:stop_words_zh.txt打开失败\n";
        return;
    }
    while(getline(ifs5,line))
    {
        istringstream iss(line);
        iss>>word;
        _stopWordList.insert(word);
    } 
    ifs5.close();

}


//计算查询词权重
vector<double> WebPageQuery::getQueryWeightVector(map<string,int>queryWords) {
    vector<double>base;
    for(auto it = queryWords.begin();it!=queryWords.end();++it)
    { 
        double TF=it->second;
        double DF ,IDF ,N;
        DF=_invertIndexTable.find(it->first)->second.size();//单词在所有文章中出现的次数，即包含该单词的文档数量
        N=_pageLib.size();//网页库的文档数量
        IDF=(log(N/(DF+1)))/(log(2.0));//逆文档频率，表示该词对于该篇文章的重要性的一个系数
        base.push_back(TF*IDF);//TF*IDF权重
    }
    return base;
}

Json WebPageQuery::doQuery(const string& str)
{
    //1.忽略停用词，插入查询map
    map<string,int> queryWords;
    //istringstream iss(str);
    vector<string>tempstr=_jieba->cut(str);
 
    for(auto ch:tempstr)
    {
        if(_stopWordList.find(ch)!=_stopWordList.end())
        {
            continue;
        }
        else
        {
            auto it=queryWords.find(ch);
            if(it!=queryWords.end())
            {
                it->second++;
            }
            else
            {   
                queryWords.insert(make_pair(ch,1));
            }
        }

    }

    /* while(!iss.eof()) */
    /* { */
    /*     iss>>tmpstr; */
    /*     if(_stopWordList.find(tmpstr)!=_stopWordList.end()) */
    /*     { */
    /*         continue; */
    /*     } */
    /*     else */
    /*     { */
    /*         auto it=queryWords.find(tmpstr); */
    /*         if(it!=queryWords.end()) */
    /*         { */
    /*             it->second++; */
    /*         } */
    /*         else */
    /*         { */   
    /*             queryWords.insert(make_pair(tmpstr,1)); */
    /*         } */
    /*     } */
    /* } */
    //防止报错
    if(queryWords.size()==0)
    {
        Json json;
        return json;
    }

    map<int,vector<double>>resultVec;//<文章id，每个查询词的权重>
    if(executeQuery(queryWords,resultVec))
    {
        //2.网页按余弦曲线排序
        vector<double>base=getQueryWeightVector(queryWords);
        multimap<double,int> RecommWebPage;
        for(auto it=resultVec.begin();it!=resultVec.end();++it)
        {
            double XMultY=0,xSqu=0,ySqu=0;
            auto it1=base.begin();
            auto it2=it->second.begin();
            for(;it1!=base.end();++it1,++it2)
            {
                XMultY+=((*it1)*(*it2));
                xSqu+=((*it1)*(*it1));
                ySqu+=((*it2)*(*it2));
            }
            double cos=XMultY/(sqrt(xSqu)*sqrt(ySqu));
            RecommWebPage.insert(make_pair(cos,it->first));
        }
        
        //3.将推荐的文章id插入
        vector<int> docIdVec;
        map<int,double> weightmap;//文章id，推荐词权重和
        for(auto ch:resultVec)
        {
            double weight=0;
            for(auto ah:ch.second)
            {
                weight+=ah;        
            }
            weightmap.insert(make_pair(ch.first,weight));

        }
        insertMaxCosIdToVec(weightmap, RecommWebPage, docIdVec);
        /* if(RecommWebPage.size()>5) */
        /* { */
        /*     int num =5; */
        /*     auto rit =RecommWebPage.rbegin(); */
        /*     while(num--) */
        /*     { */
        /*         docIdVec.push_back(rit->second); */
        /*         rit++; */
        /*     } */
        /* } */   
        /* else */
        /* { */
        /*     for(auto rit =RecommWebPage.rbegin();rit!=RecommWebPage.rend();++rit) */
        /*     { */
        /*         docIdVec.push_back(rit->second); */
        /*     } */
        /* } */


        //4.
        Json retStr;
        Json result=Json::array();
        int result_count=10;
        for(auto & id:docIdVec)
        {
            if(result_count==0)
                break;
            result_count--;
            Json record;
            string content=_pageLib[id].getDocContent();
            string sunmmary=generateSummary(content,queryWords);
            icu::UnicodeString ustr=icu::UnicodeString::fromUTF8(sunmmary);
            string ress;
            ustr.toUTF8String(ress);
            record={
                {"summary",ress},
                {"title",_pageLib[id].getDocTitle()},
                {"url",_pageLib[id].getDocUrl()}
            };
            result.push_back(record);
        }
        retStr["results"]=result;
        return retStr;

    }
    return Json("");

}

bool WebPageQuery::executeQuery(const map<string,int>&queryWords,map<int,vector<double>>&resultVec)
{
    for(auto& elem:queryWords)
    {
        if(_invertIndexTable.find(elem.first)==_invertIndexTable.end())
        {
            return false;
        }
    }

    auto word = queryWords.begin();//map<string,int>
    multimap<int,double> vecTemp;

    for(;word!=queryWords.end();++word)//遍历所有查询词
    {
        auto it=_invertIndexTable.find(word->first);//it是一个map<string,set<int,double>>
        for(auto id=it->second.begin();id!=it->second.end();++id)//遍历 it的第二个元素，也就是集合,遍历所有文章
        {
            vecTemp.insert(make_pair(id->first,id->second));//无序图存储文章id，和权重。
        }
    }

    for(auto re=vecTemp.begin();re!=vecTemp.end();++re)
    {
        resultVec[re->first].push_back(re->second);//multimap<int,double>转map<int,vector<double>>
    }


    /*     for (auto IdAndFreq = it->second.begin(); IdAndFreq != it->second.end();++IdAndFreq)//it->second=set<int,double>,这一步在遍历集合 */
    /* { */
    /*     vecTemp=IdAndFreq->second; */
    /*     resultVec.insert(make_pair(IdAndFreq->first, vecTemp));//搜索结果全部插入resultVec */
    /* } */

    /* { */
    /*     it=_invertIndexTable.find(word->first); */
    /*     for(auto IdAndFreq=it->second.begin();IdAndFreq!=it->second.end();++IdAndFreq) */
    /*     { */
    /*         auto temp= resultVec.find(IdAndFreq->first); */
    /*         if(temp!=resultVec.end()) */
    /*         { */
    /*             temp->second.push_back(IdAndFreq->second); */
    /*         } */
    /*     } */
    /* } */

    /* for(auto it = resultVec.begin();it!=resultVec.end();) */
    /* { */
    /*     cout<<"1:"<<it->second.size()<<" 2:"<<queryWords.size()<<"\n"; */
    /*     if(it->second.size()<queryWords.size()) */
    /*     { */
    /*         it=resultVec.erase(it); */
    /*     } */
    /*     else */
    /*     { */
    /*         ++it; */
    /*     } */
    /* } */

    
    if(resultVec.size()==0)
    {
        return false;
    }
    return true;
}


std::string WebPageQuery::generateSummary(std::string& content, std::map<std::string, int>& queryWords) {
    std::string summary, strTmp;

    for (std::map<std::string, int>::const_iterator it = queryWords.begin(); it != queryWords.end(); ++it) {
        const std::string& keyword = it->first; // 键
        // 检查 summary 中是否已经包含了 keyword
        if (summary.find(keyword) != std::string::npos) {
            continue;
        }

        size_t pos = content.find(keyword);
        if (pos != std::string::npos) {
            size_t beg1 = content.find("。", pos);
            size_t beg2 = content.find("，", pos);
            size_t beg3 = std::max(beg1, beg2);
            size_t end1 = content.find("。", pos);
            size_t end2 = content.find("，", pos);
            size_t end3 = std::max(end1, end2);

            if (beg3 != std::string::npos && end3 != std::string::npos) {
                if(end3-beg3-1<1000)
                    strTmp = content.substr(beg3 + 1, end3 - beg3 - 1);
                else
                    strTmp = content.substr(beg3 + 1, 1000);
            }
        }

        // 处理截断字符
        if (strTmp.size() > 0 && nBytesCode(strTmp[0]) != 1) {
            if (nBytesCode(strTmp[1]) == 1) {
                strTmp.erase(0, 1);
            } else {
                strTmp.erase(0, 2);
            }
        }

        size_t endIdx = strTmp.size() - 1;
        if (endIdx >= 1 && nBytesCode(strTmp[endIdx - 1]) != 1) {
            if (nBytesCode(strTmp[endIdx]) == 1) {
                strTmp.erase(endIdx, 1);
            } else {
                strTmp.erase(endIdx - 1, 2);
            }
        }
        summary ="..."+strTmp+"...";
    }

    return summary;
}
//根据utf-8编码规则判断给定字符的字节数
size_t WebPageQuery::nBytesCode(const char ch) {
    if(ch&(1<<7))
    {
        int nBytesCode=1;
        for(int i=0;i!=6;++i)
        {
            if(ch&(1<<(6-i)))
            {
                ++nBytesCode;
            }
            else
            {
                break;
            }
        }
        return nBytesCode;
    }
    return 1;
}


void WebPageQuery::insertMaxCosIdToVec(const std::map<int, double>& weightmap,
                         const std::multimap<double, int>& RecommWebPage,
                         std::vector<int>& docIdVec)
{
     // 创建一个vector来存储cos和id的对
    std::vector<std::pair<double, int>> sortedPages(RecommWebPage.begin(), RecommWebPage.end());
    
    // 按照cos值降序排序，如果cos相同则按照weight降序排序
    std::sort(sortedPages.begin(), sortedPages.end(), 
              [&weightmap](const std::pair<double, int>& a, const std::pair<double, int>& b) {
                  if (a.first != b.first) {
                      return a.first > b.first; // cos值从大到小
                  } else {
                      return weightmap.at(a.second) > weightmap.at(b.second); // weight从大到小
                  }
              });

    // 将排序后的id依次插入docIdVec
    for (const auto& page : sortedPages) {
        docIdVec.push_back(page.second);
    }
}




