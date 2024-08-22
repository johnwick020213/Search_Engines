#include "../include/SplitCppJieba.h"
#include "../include/Configuration.h"
 
#include<map>

using std::string;
using std::vector;
using std::map;

const char* const DICT_PATH = "../dict/jieba.dict.utf8";
const char* const HMM_PATH = "../dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../dict/user.dict.utf8";
const char* const IDF_PATH = "../dict/idf.utf8";
const char* const STOP_WORD_PATH = "../dict/stop_words.utf8";

SplitToolCppJieba::SplitToolCppJieba()
:_jieba(DICT_PATH,
        HMM_PATH,
        USER_DICT_PATH
        )
{
    
}


vector<string> SplitToolCppJieba::cut(const string & sentence)
{
    vector<string>words;
    _jieba.Cut(sentence,words,true);
    return words;
}

