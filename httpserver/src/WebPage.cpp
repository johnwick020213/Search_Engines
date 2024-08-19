#include "../include/WebPage.h"

 WebPage::WebPage(){} {

}

 WebPage::WebPage(string &docStr) {

}

 WebPage::~WebPage(){} {

}

void WebPage::setNewId(int id) {

}

void WebPage::setAll(int id,string url,string title,string content) {

}

void WebPage::buildWordsMap(Configuration*conf,SplitTool* jieba) {

}

string WebPage::getDoc() {

}

int WebPage::getDocId() {

}

 WebPage::map<string,int>&getWordsMap() {

}

void WebPage::calcTopK(vector<string>&wordsVec,int k,set<string>&stopWordList) {

}

friend bool WebPage::operator==(const WebPage &lhs,const WebPage &rhs) {

}

friend bool WebPage::operator<(const WebPage &lhs,const WebPage &rhs) {

}

