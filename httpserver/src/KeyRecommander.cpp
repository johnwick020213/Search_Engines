#include "../include/KeyRecommander.h"

 KeyRecommander::KeyRecommander(const string word) {
    execut();
}

Json KeyRecommander::get_result() {
    Json tmp;

    for(int i=0;i<10;i++)
    {
        tmp.push_back(_resultQue.pop());
    }
}

void KeyRecommander::execut() {

}

void KeyRecommander::queryIndexTable() {

}

void KeyRecommander::statistic(set<int>& iset) {

}

