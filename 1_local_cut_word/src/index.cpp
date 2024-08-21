#include "../include/index.h"
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<unordered_map>
#
using std::cerr;
using std::stringstream;
using std::ifstream;
using std::string;
using std::getline;
using std::unordered_map;
using std::set;
using std::vector;
using std::make_pair;
using std::ofstream;

void Index::init(const std::string& dictEnPath,const string& dictCnPath)
{
    read_enDict(dictEnPath);

    for(size_t idx=0;idx!=_en_dict.size();++idx)
    {
        create_en_index(idx);
    }
    read_cnDict(dictEnPath);

    for(size_t idx=0;idx!=_cn_dict.size();++idx)
    {
        create_cn_index(idx);
    }

}

void Index::read_enDict(const string&_path)
{
    ifstream ifs(_path);
    if(!ifs)
    {
        cerr<<"error_in_Index::read_Dict:打开"<<_path<<"失败\n";
        exit(1);
    }

    string line;
    while(getline(ifs,line))
    {
        stringstream ss(line);
        string key;
        int value;
        ss>>key>>value;
        _en_dict.push_back(make_pair(key,value));
    }
    ifs.close();
}

void Index::read_cnDict(const string&_path)
{
    ifstream ifs(_path);
    if(!ifs)
    {
        cerr<<"error_in_Index::read_Dict:打开"<<_path<<"失败\n";
        exit(1);
    }

    string line;
    while(getline(ifs,line))
    {
        stringstream ss(line);
        string key;
        int value;
        ss>>key>>value;
        _cn_dict.push_back(make_pair(key,value));
    }
    ifs.close();
}


void Index::create_cn_index(int index)
{
     string key;
     string word=_cn_dict[index].first;
     for(size_t idx=0;idx!=word.size();++idx)
     {
         char ch=word[idx];
         if(ch&(1<<7))//大于128的编码
         {
             if((ch&0xF0)==0xC0||(ch&0xF0)==0xD0)//两个字节
             {
                 key=word.substr(idx,2);
                 ++idx;
             }
             else if((ch&0xF0)==0xE0)//三个字节
             {
                 key=word.substr(idx,3);
                 idx+=2;
             }
             else if((ch&0xFF)>=0xF0&&(ch&0xFF)<=0xF7)//四字节
             {
                 key=word.substr(idx,4);
                 idx+=3;
             }
             else if((ch&0xFF)>=0xF8&&(ch&0xFF)<=0xFB)//五个字节
             {
                 key=word.substr(idx,5);
                 idx+=4;
             }
             else if((ch&0xFF)==0xFC)
             {
                 key=word.substr(idx,6);
                 idx+=5;
             }

         }
         else//英文
         {
            continue;  
         }
         _index[key].insert(index);
     }
}


void Index::create_en_index(int index)
{
     string key;
     string word=_en_dict[index].first;
     for(size_t idx=0;idx!=word.size();++idx)
     {
         key=word.substr(idx,1);   
        _index[key].insert(index);
     }
}


 


void Index::store_index(const string&index_path) 
{
    ofstream ofs(index_path);
    if(!ofs)
    {
        cerr<<"error_in_Index::store_index:打开"<<index_path<<"时出错!\n" ;
        exit(1);
    }
    for(auto it=_index.begin();it!=_index.end();++it)
    {
        ofs<<it->first<<"\t";
        for(auto set_int:it->second)
        {
            ofs<<set_int<<" ";
        }
        ofs<<"\n";
    }
    ofs.close();
}

