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

    void storeDict(string filepath);

   private:
    void getFiles();
    void pushDict(const string & word);
    void clearWord(string &word);
    
    string _dir;//语料库目录
    vector<string> _files;//语料文件的绝对路径集合
    SplitTool *_splitTool;
    map<string,int> _dict;
    };
   

#endif 

