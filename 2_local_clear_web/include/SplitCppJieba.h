#ifndef __SPLITTOOLCPPJIEBA_H__
#define __SPLITTOOLCPPJIEBA_H__

#include"SplitTool.h"
#include"./simhash/deps/cppjieba/Jieba.hpp"
#include<string>
#include<vector>
#include<map>

using std::string;
using std::vector;
using std::map;

class SplitToolCppJieba
:public SplitTool
{
public:
    SplitToolCppJieba();
    ~SplitToolCppJieba(){};

    vector<string> cut(const string & sentence);

private:
    cppjieba::Jieba _jieba;
};
   

#endif 

