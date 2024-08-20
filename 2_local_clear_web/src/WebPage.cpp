#include "../include/WebPage.h"
#include<sstream>

using std::ostringstream;

 WebPage::WebPage(string &docStr,Configuration & config,SplitToolCppJieba &jieba)
{

}

void WebPage::setNewId(int id) {
    _docId=id;
}

void WebPage::setAll(int id,string url,string title,string content) {
    _docId=id;
    _docUrl=url;
    _docTitle=title;
    _docContent=content;
}

void WebPage::buildWordsMap(Configuration*conf,SplitTool* jieba) {
    
}

string WebPage::getDoc() {
    ostringstream doc;
    doc<<"<doc>\n"
        <<"\t<docid>"<<_docId<<"</docid>\n"
        <<"\t<url>"<<_docUrl<<"</url>\n"
        <<"\t<title>"<<_docTitle<<"</title>\n"
        <<"\t<content>"<<_docContent<<"</content>\n"
        <<"</doc>|t\n";

    return doc.str();
}

int WebPage::getDocId() {
    return _docId;
}

map<string,int>& WebPage::getWordsMap() {
    return _wordsMap;
}

void WebPage::calcTopK(vector<string>&wordsVec,int k,set<string>&stopWordList) {

}

bool operator==(const WebPage &lhs,const WebPage &rhs) {

}

bool operator<(const WebPage &lhs,const WebPage &rhs) {

}

