#ifndef __WEBPAGE_H__
#define __WEBPAGE_H__

#include"SplitCppJieba.h"
#include"Configuration.h"
#include"simhash/Simhasher.hpp"
#include"SplitTool.h"
#include <string>
#include<vector>
#include<map>
#include<unordered_map>
using std::string;
using std::unordered_map;
using std::vector;
    


class WebPage
{
public:
    WebPage(){};
    WebPage(string &docStr);
    ~WebPage(){};

    void setNewId(int id);
    void setAll(int id,string url,string title,string content);
    void buildWordsMap(Configuration*conf,SplitTool* jieba);

    string getDoc();
    int getDocId();
    string & getDocTitle();
    string & getDocUrl();
    string & getDocContent();
    unordered_map<string,int>&getWordsMap();

    void calcSimhashValue(simhash::Simhasher& Simhasher);

    friend bool operator==(const WebPage &lhs,const WebPage &rhs);

private:
    
    int _docId;
    string _docTitle;
    string _docUrl;
    string _docContent;

    unordered_map<string,int> _wordsMap;

    uint64_t _simhashValue;
};
   

#endif 

