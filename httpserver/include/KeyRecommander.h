#ifndef __KEYRECOMMANDER_H__
#define __KEYRECOMMANDER_H__

#include <string>

using namespace std;

class KeyRecommander
{
public:
    KeyRecommander(const string word);
    
private:
    void queryIndexTable();
    

    priority_queue<string result> _resultQue;
    string _queryWord;
    
};
   

#endif 

