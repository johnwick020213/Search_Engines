#ifndef __WEBPAGE_H__
#define __WEBPAGE_H__

#include"SplitCppJieba.h"
#include"Configuration.h"
#include <string>
#include<vector>
#include<map>

using std::string;
using std::map;
using std::vector;


class WebPage
{
public:
    WebPage(){};
    WebPage(string &docStr,Configuration &config,SplitToolCppJieba &jieba);
    ~WebPage(){};

    void setNewId(int id);
    void setAll(int id,string url,string title,string content);
    void buildWordsMap(Configuration*conf,SplitTool* jieba);

    string getDoc();
    int getDocId();
    map<string,int>&getWordsMap();

    void calcTopK(vector<string>&wordsVec,int k,set<string>&stopWordList);

    friend bool operator==(const WebPage &lhs,const WebPage &rhs);
    friend bool operator<(const WebPage &lhs,const WebPage &rhs);

private:
    string _doc;
    int _docId;
    string _docTitle;
    string _docUrl;
    string _docContent;
    map<string,int> _wordsMap;
};
   

#endif 

