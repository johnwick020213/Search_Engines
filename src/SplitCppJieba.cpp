#include "../include/SplitCppJieba.h"
#include "../include/Configuration.h"

#include<map>

using std::string;
using std::vector;
using std::map;

SplitToolCppJieba::SplitToolCppJieba(map<string,string>&_path)
:_jieba(_path["DICT_PATH="],
       _path["HMM_PATH="],
       _path["USER_DICT_PATH="],
       _path["STOP_WORD_PATH="])
{
    
}

vector<string> SplitToolCppJieba::cut(const string & sentence)
{
    vector<string>words;
    _jieba.Cut(sentence,words,true);
    return words;
}

