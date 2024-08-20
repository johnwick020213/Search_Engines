#include "../include/Dictionary.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<set>
using namespace std;


void Dictionary::readDictAndIndex(const string& dict_path,const string& index_path) {
    if(_isInited)
    {
        return;
    }
    
    ifstream ifs_dict(dict_path);
    if(!ifs_dict.good())
    {
        cerr<<"error_in_Dictionary::readDictAndIndex:文件打开失败\n";
        return;
    }
    string line;
    while(getline(ifs_dict,line))
    {
        istringstream iss(line);
        string word;
        int count;
        iss>>word;
        iss>>count;
        _dict.push_back({word,count});
    }

    ifs_dict.close();

    ifstream ifs_index(index_path);
    if(!ifs_index)
    {
        cerr<<"error_in_Dictionary::readDictAndIndex:打开索引文件失败\n";
        return;
    }
    while(getline(ifs_index,line))
    {
        istringstream iss(line);
        string word;
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
    ifs_index.close();
    _isInited=true;
}

vector<pair<string,int>>& Dictionary::getDict()
{
    return _dict;
}

map<string,set<int>>& Dictionary::getIndex()
{
    return _index_table;
}

EnDicitionary *EnDicitionary::_pInstance1=nullptr;
Dictionary* EnDicitionary::createInstance()
{
    if(_pInstance1==nullptr)
    {
        _pInstance1=new EnDicitionary();
    }
    return _pInstance1;
}

CnDictionary *CnDictionary::_pInstance2=nullptr;
 Dictionary*CnDictionary::createInstance() {
    if(_pInstance2==nullptr)
    {
        _pInstance2=new CnDictionary();
    }
    return _pInstance2;
}

