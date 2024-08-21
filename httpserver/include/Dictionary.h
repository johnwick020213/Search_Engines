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
    static Dictionary* createInstance();

    vector<pair<string,int>>& get_enDict();
    vector<pair<string,int>>& get_cnDict();
    map<string,set<int>>& getIndex();

private:
    Dictionary();
    void readDict(const string& dict_path,vector<pair<string,int>>& dict);
    void readIndex(const string & index_path,map<string,set<int>>& index_table);
    
    vector<pair<string,int>> _cn_dict;
    vector<pair<string,int>> _en_dict;
    map<string,set<int>> _index_table;
    
    static Dictionary * _pInstance;
};
   

#endif 

