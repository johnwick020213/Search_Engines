#include "../include/Dictionary.h"
#include<fstream>
#include<iostream>
#include<sstream>


using std::cerr;
using std::ifstream;
using std::istringstream;
Dictionary* Dictionary::_pInstance = nullptr;


Dictionary* Dictionary::createInstance() {
    if(_pInstance==nullptr)
    {
        _pInstance=new Dictionary();
    }

    return _pInstance;
}

vector<pair<string,int>>& Dictionary::get_enDict() {
    return _en_dict;
}

vector<pair<string,int>>& Dictionary::get_cnDict() {
    return _cn_dict;
}

map<string,set<int>>& Dictionary::getIndex() {
    return _index_table;
}

 Dictionary::Dictionary()
{
    readDict("data/english.dat",_en_dict);
    readDict("data/english.dat",_cn_dict);
    readIndex("data/index.dat",_index_table);
}

void Dictionary::readDict(const string& dict_path,vector<pair<string,int>>& dict) {
    ifstream ifs(dict_path);
    if(!ifs)
    {
        cerr<<"error_in_Dictionary::readDict:打开字典失败\n";
        exit(-1);
    }

    string lines;
    string word;
    int counts;
    

    while(getline(ifs,lines))
    {
        istringstream iss(lines);
        iss>>word;
        iss>>counts;
        dict.push_back({word,counts});
        
    }
    ifs.close();
}

void Dictionary::readIndex(const string & index_path,map<string,set<int>>& index_table)
{
    ifstream ifs(index_path);
    if(!ifs)
    {
        cerr<<"error_in_Dictionary::readIndex:打开索引失败\n";
        exit(-1);
    }

    string lines;
    string word;
    set<int> counts;
    

    while(getline(ifs,lines))
    {
        istringstream iss(lines);
        
        iss>>word;
            
        auto p=_index_table.insert({word,set<int>()});
        if(p.second)
        {
            int idx;
            while(iss>>idx)
            {
                p.first->second.insert(idx);
            }
        }
        else
        {
            cerr<<"error_in_Dictionary::readDictAndIndex:插入失败\n";
        }

    }
    ifs.close();
}
