#include "../include/WebPage.h"
#include "../include/tinyxml2.h"
#include <sstream>
#include<cstring>
#include<iostream>
#include<regex>
using std::regex;
using std::cout;
using std::ostringstream;
using namespace tinyxml2;
WebPage::WebPage(string &docStr)
{
     tinyxml2::XMLDocument doc;
    doc.Parse(docStr.c_str());

    XMLElement * docid = doc.FirstChildElement("doc")->FirstChildElement("docid");
    if(docid)
    {
        _docId =atoi(docid->GetText());
    }

    XMLElement * title = doc.FirstChildElement("doc")->FirstChildElement("title");
    if(title)
    {
        _docTitle = title->GetText();
    }

    XMLElement *link = doc.FirstChildElement("doc")->FirstChildElement("link");
    if (link)
    {
        _docUrl=link->GetText();
    }

    XMLElement *content=doc.FirstChildElement("doc")->FirstChildElement("description");
    if(content)
    {
        _docContent=content->GetText();
    }
    
    /* else{ */
    /*     while (rootElement) */
    /*     { */
    /*         //还可以在此处判断指针是不是空指针，然后在赋值给string字符串 */
    /*         XMLElement * title = rootElement->FirstChildElement("title"); */
    /*         if(title) */
    /*         { */
    /*             _docContent += title->GetText(); */
    /*             _docContent += "\n"; */
    /*         } */

    /*         XMLElement * link = rootElement->FirstChildElement("link"); */
    /*         if(link) */
    /*         { */
    /*             _docContent += link->GetText(); */
    /*             _docContent += "\n"; */
    /*         } */

    /*         regex reg("<[^>]+>");//通用正则表达式(主要研究一下正则表达式的规则) */
    /*         XMLElement * description = rootElement->FirstChildElement("description"); */
    /*         if(description) */
    /*         { */
    /*             string str = description->GetText(); */
    /*             str = regex_replace(str, reg, "");//将HTML的标签去掉 */
    /*             _docContent += str + "\n"; */
    /*         } */
    /*         XMLElement * content = rootElement->FirstChildElement("content"); */
    /*         if(content) */
    /*         { */
    /*             string str = content->GetText(); */
    /*             str = regex_replace(str, reg, "");//将HTML的标签去掉 */
    /*             _docContent += str + "\n"; */
    /*         } */

    /*         rootElement = rootElement->NextSiblingElement("item");//找到下一篇item */
    /*     } */
    /* } */

}
void WebPage::setNewId(int id)
{
    _docId = id;
}

void WebPage::setAll(int id, string url, string title, string content)
{
    _docId = id;
    _docUrl = url;
    _docTitle = title;
    _docContent = content;
}

void WebPage::buildWordsMap(Configuration* conf, SplitTool* jieba)
{
    set<string> stopWords = conf->getStopWordList();
    vector<string> vec = jieba->cut(_docContent);

    for (auto& elem : vec)
    {
        if (stopWords.count(elem) == 0)
        {
            ++_wordsMap[elem];
        }
    }
}

string WebPage::getDoc()
{
    ostringstream doc;
    doc << "<doc>\n"
        << "\t<docid>" << _docId << "</docid>\n"
        << "\t<url>" << _docUrl << "</url>\n"
        << "\t<title>" << _docTitle << "</title>\n"
        << "\t<content>" << _docContent << "</content>\n"
        << "</doc>\n";

    return doc.str();
}

int WebPage::getDocId()
{
    return _docId;
}

unordered_map<string, int>& WebPage::getWordsMap()
{
    return _wordsMap;
}

void WebPage::calcSimhashValue(simhash::Simhasher& simhasher)
{
    size_t topN = 30;
    uint64_t u64 = 0;
    simhasher.make(_docContent, topN, u64);
    _simhashValue = u64;
}

string& WebPage::getDocTitle()
{
    return _docTitle;
}

string& WebPage::getDocUrl()
{
    return _docUrl;
}

string& WebPage::getDocContent()
{
    return _docContent;
}

bool operator==(const WebPage &lhs, const WebPage &rhs)
{
    return simhash::Simhasher::isEqual(lhs._simhashValue, rhs._simhashValue);
}

