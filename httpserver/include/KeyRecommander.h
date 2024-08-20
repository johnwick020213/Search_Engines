#ifndef __KEYRECOMMANDER_H__
#define __KEYRECOMMANDER_H__
#include<wfrest/json.hpp>
#include <string>
#include<queue>
#include<set>
using namespace std;

using Json=nlohmann::json;

class KeyRecommander
{
public:
    KeyRecommander(const string word);
       
    Json get_result();
private:
    void execut();
    void queryIndexTable();
    void statistic(set<int>& iset);
    int distance(const string & rhs);//计算最小编辑距离。


    priority_queue<string> _resultQue;
    string _queryWord;
    
};
   

#endif 

