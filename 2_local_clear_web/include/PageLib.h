#ifndef __PAGESCANNER_H__
#define __PAGESCANNER_H__

#include"Configuration.h"
#include"FileProcessor.h"
#include"DirScanner.h"
#include <iostream>
#include <string>
#include<vector>
#include<map>

using std::string;
using std::map;
using std::vector;
using std::pair;
class PageLib
{
public:
    PageLib(Configuration& conf,DirScanner& dirScanner,FileProcessor& fileProcessor);
    void create();
    void store();

private:
    DirScanner &_dirScanner;
    vector<string>_files;
    map<int,pair<int,int>>_offsetLib;
};
   

#endif 

