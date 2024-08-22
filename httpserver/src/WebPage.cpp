#include "../include/WebPage.h"
#include"../include/tinyxml2.h"
#include<sstream>


using std::ostringstream;

 WebPage::WebPage(string &docStr)
{
    tinyxml2::XMLDocument doc;
    doc.Parse(docStr.c_str());

    tinyxml2::XMLElement *rootElement=doc.RootElement();
    if(rootElement==NULL)
    {
        _docId=-1;
        _docTitle="";
        _docUrl="";
        _docContent="";
    }
    else
    {
        string id = rootElement->FirstChildElement("docid")->GetText();
        _docId=stoi(id);

        string url = rootElement->FirstChildElement("url")->GetText();
        _docUrl=string(url);

        string title=rootElement->FirstChildElement("title")->GetText();
        _docTitle=string(title);

        const char* content=rootElement->FirstChildElement("content")->GetText();
        if(content==nullptr)
        {
            _docContent=string(" ");
        }
        else
        {
            _docContent=string(content);
        }
    }
}

void WebPage::setNewId(int id)
{
    _docId=id;
}

void WebPage::setAll(int id,string url,string title,string content) {
    _docId=id;
    _docUrl=url;
    _docTitle=title;
    _docContent=content;
}

void WebPage::buildWordsMap(Configuration*conf,SplitTool* jieba) {
    set<string>stopWords=conf->getStopWordList();
 
    vector<string> vec=jieba->cut(_docContent);
    for(auto&elem :vec)
    {  
        if(stopWords.count(elem)==0)
        {   
            ++_wordsMap[elem];
        }
    }

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

unordered_map<string,int>& WebPage::getWordsMap() {
    return _wordsMap;
}

void WebPage::calcSimhashValue(simhash::Simhasher& simhasher)
{
    size_t topN=30;
    uint64_t u64=0;
    simhasher.make(_docContent,topN,u64);
    _simhashValue=u64;
}


bool operator==(const WebPage &lhs,const WebPage &rhs) {
    return simhash::Simhasher::isEqual(lhs._simhashValue,rhs._simhashValue);
}


