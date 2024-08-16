#ifndef __DIRCTPRODUCER_H__
#define __DIRCTPRODUCER_H__

#include<vector>
#include<string>
#include<map>
#include<set>

#include"SplitTool.h"

using std::set;
using std::map;
using std::string;
using std::vector;
using std::pair;

class DictProducer
{
public:
    DictProducer(const string &dir);
    DictProducer(const string& dir,SplitTool *splitTool);

    void buildEnDict();
    void buildCnDict();
    void storeDict(const char *filepath);

    void getFiles();
    void pushDict(const string & word);
private:
    vector<string> _files;
    vector<pair<string,int>> _dict;
    SplitTool *_splitTool;
    map<string,set<int>> _index;
};
   

#endif 

