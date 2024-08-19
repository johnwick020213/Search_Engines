#ifndef __PAGELIBPREPROCESSOR_H__
#define __PAGELIBPREPROCESSOR_H__

#include"WebPage.h"
#include"Configuration.h"
#include"SplitCppJieba.h"
#include <string>

class PageLibPreprocessor
{
public:
    PageLibPreprocessor(Configuration &conf);
    void doProcess();
    void readInfoFromFile();
    void cutRedundantPages();
    void buildInvertIndexTable();
    void storeOnDisk();

private:
    SplitToolCppJieba _jieba;
    vector<WebPage> _pageLib;
    unordered_map<int,pair<int,int>>_offsetLib;
    unordered_map<string,vector<pair<int,double>>_inverIndexTable;

};

#endif 

