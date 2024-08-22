#include  "../include/WebPageQuery.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
//#include<pair>

using std::map;
using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::make_pair;
using std::multimap;
using std::pair;

Mysql mysql;

WebPageQuery* WebPageQuery::_pInstance=nullptr;
WebPageQuery::AutoRelease WebPageQuery::_ar;

WebPageQuery::WebPageQuery() {
    loadLibrary();
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
    ifstream ifs1("data/offset_new.dat");
    if(!ifs1.good())
    {
        cerr<<"error_in_WebPageQuery::loadLibrary:offset_new.dat打开失败\n";
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
    ifstream ifs2("data/ripepage_new.dat");
    if(!ifs2.good())
    {
        cerr<<"error_in_WebPageQuery::loadLibrary:ripepage_new.dat打开失败\n";
        return;
    } 

    char *buff =new char[100000];

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
    ifstream ifs3("data/invertIndex.dat");
    if(!ifs3.good())
    {
        cerr<<"error_in_WebPageQuery::loadLibrary:invertIndex.dat打开失败\n";
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
        cerr<<"error_in_WebPageQuery::loadLibrary:stop_words_eng.dat打开失败\n";
        return;
    }

    while(getline(ifs4,line))
    {
        istringstream iss(line);
        iss>>word;
        _stopWordList.insert(word);
    }
    ifs4.close();

    

    ifstream ifs5("data/stop_words_zh.tst");
    if(!ifs5.good())
    {
        cerr<<"error_in_WebPageQuery::loadLibrary:stop_words_zh.dat打开失败\n";
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
    istringstream iss(str);
    string tmpstr;
    while(!iss.eof())
    {
        iss>>tmpstr;

        if(_stopWordList.find(tmpstr)!=_stopWordList.end())
        {
            continue;
        }
        else
        {
            auto it=queryWords.find(tmpstr);
            if(it!=queryWords.end())
            {
                it->second++;
            }
            else
            {   
                queryWords.insert(make_pair(tmpstr,1));
            }
        }
    }

    //防止报错
    if(queryWords.size()==0)
    {
        return Json(" ");
    }

        map<int,vector<double>>resultVec;
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
                XMultY+=(*it1)*(*it2);
                xSqu+=(*it1)*(*it1);
                ySqu+=(*it2)*(*it2);
            }
            double cos=XMultY/(sqrt(xSqu)*sqrt(ySqu));
            RecommWebPage.insert(make_pair(cos,it->first));
        }
        
        
        //3.将推荐的文章id插入
        vector<int> docIdVec;
        if(RecommWebPage.size()>5)
        {
            int num =5;
            auto rit =RecommWebPage.rbegin();
            while(num--)
            {
                docIdVec.push_back(rit->second);
                rit++;
            }
        }   
        else
        {
            for(auto rit =RecommWebPage.begin();rit!=RecommWebPage.end();++rit)
            {
                docIdVec.push_back(rit->second);
            }
        }


        //4.
        Json retStr=Json("");
        for(auto & id:docIdVec)
        {
            int i=0;
            string content=_pageLib[id].getDocContent();
            string sunmmary=generateSummary(content,queryWords);
            retStr[i]["title"]=_pageLib[id].getDocTitle();
            retStr[i]["url"]=_pageLib[id].getDocUrl();
            retStr[i]["summary"]=sunmmary;
            retStr[i]["content"]=content;
            i++;
        }
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
    auto word = queryWords.begin();
    auto it = _invertIndexTable.find(word->first);
    vector<double> vecTemp;
    for (auto IdAndFreq = it->second.begin(); IdAndFreq != it->second.end();++IdAndFreq)
    {
        resultVec.insert(make_pair(IdAndFreq->first, vecTemp));
    }

    for(;word!=queryWords.end();++word)
    {
        it=_invertIndexTable.find(word->first);
        for(auto IdAndFreq=it->second.begin();IdAndFreq!=it->second.end();++IdAndFreq)
        {
            auto temp= resultVec.find(IdAndFreq->first);
            if(temp!=resultVec.end())
            {
                temp->second.push_back(IdAndFreq->second);
            }
        }
    }

    for(auto it = resultVec.begin();it!=resultVec.end();)
    {
        if(it->second.size()<queryWords.size())
        {
            it=resultVec.erase(it);
        }
        else
        {
            ++it;
        }
    }

    for(auto it =resultVec.begin();it!=resultVec.end();)
    {
        if(it->second.size()<queryWords.size())
        {
            it=resultVec.erase(it);
        }
        else
        {
            ++it;
        }
    }

    if(resultVec.size()==0)
    {
        return false;
    }
    return true;
}


string WebPageQuery::generateSummary(string& content,map<string,int>&queryWords)
{
    string summary,strTmp;
    for(auto it = queryWords.begin();it!=queryWords.end();++it)
    {
        if(summary.find(it->first)!=string::npos)
        {
            continue;
        }

        size_t pos=content.find(it->first);
        if(pos!=string::npos)
        {
            size_t beg1=content.find("。",pos);
            size_t beg2=content.find("，",pos);
            size_t beg3=beg1>beg2?beg1:beg2;
            size_t end1=content.find("。",pos);
            size_t end2=content.find("，",pos);
            size_t end3=end1>end2?end1:end2;
            strTmp=content.substr(beg3+3,end3-beg3-3);
        }

        //处理接断字符
        if(nBytesCode(strTmp[0]!=3))
        {
            if(nBytesCode(strTmp[1])==3)
            {
                strTmp.erase(0,1);
            }
            else
            {
                strTmp.erase(0,2);
            }
        }

        size_t endIdx=strTmp.size()-1;
        if(nBytesCode(strTmp[endIdx-2])!=3)
        {
            if(nBytesCode(strTmp[endIdx]==3))
            {
                strTmp.erase(endIdx,1);
            }
            else
            {
                strTmp.erase(endIdx-1,2);
            }
        }
        summary="..."+strTmp+"...";
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

