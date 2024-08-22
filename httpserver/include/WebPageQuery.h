#ifndef __WEBPAGEQUERY_H__
#define __WEBPAGEQUERY_H__

#include <set>
#include <string>
#include <vector>
#include<map>
//#include<pair>
#include <wfrest/json.hpp>
#include "WebPage.h"
#include "Mysql.h"
#include "SplitCppJieba.h"
using Json=nlohmann::json;

using std::pair;
using std::set;
using std::string;
using std::map;
using std::vector;

class WebPageQuery
{
public:
    class AutoRelease
    {
    public:
        AutoRelease(){}
        ~AutoRelease(){
            if(_pInstance)
            {
                delete _pInstance;
                _pInstance = nullptr;
            }
        }
    };
    
public:
    static WebPageQuery* getInstance();//创造单例对象，并自动释放
    Json doQuery(const string&str,SplitToolCppJieba *jieba);//接收查询词条，执行查询，返回结果；

private:
    WebPageQuery();
    ~WebPageQuery();

    void loadLibrary();//加载文件
    vector<double> getQueryWeightVector(map<string,int>queryWords);
    bool executeQuery(const map<string,int>&queryWords,map<int,vector<double>>&reultVec);//执行查询
    string generateSummary(string& content,map<string,int>&queryWords);
    size_t nBytesCode(const char ch);
    


private:
    map<int,WebPage> _pageLib;//网页库
    map<int,pair<int,int>> _offsetLib;//偏移库
    map<string,vector<pair<int,double>>>  _invertIndexTable;//倒排索引库
    set<string>  _stopWordList;//停用词集

    SplitToolCppJieba* _jieba;
    static AutoRelease _ar;
    static WebPageQuery* _pInstance;
};
   

#endif 

