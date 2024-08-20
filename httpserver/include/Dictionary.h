#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <string>
#include<map>
#include<vector>
#include<set>

using std::set;
using std::string;
using std::map;
using std::vector;
using std::pair;

class Dictionary
{
public:
    Dictionary():_isInited(false){}

    void readDictAndIndex(const string& dict_path,const string& index_path);
    vector<pair<string,int>>& getDict();
    map<string,set<int>>& getIndex();

private:

    vector<pair<string,int>> _dict;
    map<string,set<int>> _index_table;
    bool _isInited;
};
   

class EnDicitionary:public Dictionary
{
public:
    static Dictionary* createInstance();
private:
    static EnDicitionary* _pInstance1;
};

class CnDictionary:public Dictionary
{
public:
    static Dictionary*createInstance();
private:
    static CnDictionary*_pInstance2;
};

#endif 

