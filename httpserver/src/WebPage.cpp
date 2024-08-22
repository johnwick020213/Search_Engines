#include "../include/WebPage.h"
#include "../include/tinyxml2.h"
#include <sstream>
#include<cstring>
using std::ostringstream;

WebPage::WebPage(string &docStr)
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.Parse(docStr.c_str());

    if (error != tinyxml2::XML_SUCCESS) {
        _docId = -1;
        _docTitle = "";
        _docUrl = "";
        _docContent = "";
        return;
    }

    tinyxml2::XMLElement *rootElement = doc.RootElement();
    if (rootElement == nullptr)
    {
        _docId = -1;
        _docTitle = "";
        _docUrl = "";
        _docContent = "";
        return;
    }

    tinyxml2::XMLElement *docIdElement = rootElement->FirstChildElement("docid");
    if (docIdElement != nullptr) {
        const char* idText = docIdElement->GetText();
        _docId = idText ? stoi(string(idText)) : -1;
    } else {
        _docId = -1;
    }

    tinyxml2::XMLElement *urlElement = rootElement->FirstChildElement("url");
    _docUrl = (urlElement && urlElement->GetText()) ? urlElement->GetText() : "";

    tinyxml2::XMLElement *titleElement = rootElement->FirstChildElement("title");
    _docTitle = (titleElement && titleElement->GetText()) ? titleElement->GetText() : "";

    tinyxml2::XMLElement *contentElement = rootElement->FirstChildElement("content");
    const char* contentText = (contentElement && contentElement->GetText()) ? contentElement->GetText() : " ";
    _docContent = contentText;
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

