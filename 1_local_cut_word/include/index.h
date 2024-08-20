#ifndef __INDEX_H__
#define __INDEX_H__

#include <vector>
#include <string>
#include<set>
#include<unordered_map>

using std::vector;
using std::string;
using std::unordered_map;
using std::set;
using std::pair;

class Index{
public:
    void init(const std::string& dictEnPath,const string& dictCnPath);
    void read_Dict(const string&_path);
    void create_index(int index);
    void store_index(const string&index_path);
private:
    vector<pair<string,int>>_dict;
    unordered_map<string,set<int>>_index;
};

   

#endif 

