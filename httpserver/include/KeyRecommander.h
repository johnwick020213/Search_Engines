#ifndef __KEYRECOMMANDER_H__
#define __KEYRECOMMANDER_H__

#include"Dictionary.h"
#include<wfrest/json.hpp>
#include <string>
#include<queue>
#include<set>
using namespace std;

using Json=nlohmann::json;
int distance(const string & lhs,const string &rhs);

class KeyRecommander
{
public:
    KeyRecommander(const string word,Dictionary* dict);
       
    Json get_result();
private:
    void execut();
    void queryIndexTable();
    void statistic(set<int>& iset);

    // 仿函数：用于根据编辑距离排序
    struct CompareByDistance
    {
        CompareByDistance(const string& queryWord) : _queryWord(queryWord) {}

        // 比较函数，使用 _queryWord 来计算编辑距离
        bool operator()(const string& lhs, const string& rhs) const
        {
            return distance(_queryWord, lhs) > distance(_queryWord, rhs);
        }

        const string& _queryWord;  // 保存查询词的引用
    };
    string _queryWord;
    set<int> _sameword;
    Dictionary * _dict;
    priority_queue<string,vector<string>,CompareByDistance> _resultQue;
    
   
};

#endif 

