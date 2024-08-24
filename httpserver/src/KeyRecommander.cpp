#include "../include/KeyRecommander.h"
#include<iostream>
KeyRecommander::KeyRecommander(const string word,Dictionary *dict)
    :_queryWord(word),_dict(dict), _resultQue(CompareByDistance(_queryWord))
{
    //cout<<_queryWord<<"\n";
    execut();
}

Json KeyRecommander::get_result() {
    Json tmp;

    tmp.push_back(_queryWord);
    for(int i=0;i<9;i++)
    {
        if(!_resultQue.empty())
        {
            if(_resultQue.top()==_queryWord)
            {
                _resultQue.pop();
                i--;
                continue;
            }
                
            tmp.push_back(_resultQue.top());
            _resultQue.pop();
        }
    }
    return tmp;

}

void KeyRecommander::execut() {
    queryIndexTable();
    statistic(_sameword);
}

void KeyRecommander::queryIndexTable()
{
    map<string,set<int>>tmp=_dict->getIndex();
    for(size_t i=0;i<_queryWord.size();)
    {
        if((_queryWord[i]&0x80)==0)//Ó¢ÎÄ
        {
            string key(1,_queryWord[i]);

            if(tmp.find(key)!=tmp.end())
            {
                const set<int>& indices=tmp[key];
                _sameword.insert(indices.begin(), indices.end());
            }
            i++;
        }
        else//¿ÉÄÜÖÐÎÄ
        {
            string key=_queryWord.substr(i,3);
            //cout<<"key:"<<key<<"\n";
            if(tmp.find(key)!=tmp.end())
            {
                //cout<<"key´æÔÚ\n";
                const set<int>& indices=tmp[key];
                //cout<<*indices.begin()<<"\n";
                _sameword.insert(indices.begin(),indices.end());
                //cout<<*_sameword.begin()<<" "<<*_sameword.end()<<"\n";
            }
            i+=3;
        }
    }
    
}


void KeyRecommander::statistic(set<int>& iset) {
    auto it=_sameword.begin();
    for(;it!=_sameword.end();++it)
    { 
        string result;
        if(_dict->get_cnDict().size()<=*it)
        {
            if(_dict->get_enDict().size()<=*it)
            {
                continue;
            }
            else
            {
                result=_dict->get_enDict()[*it].first;
                _resultQue.push(result);
            }
        }
        else 
        {
            result=_dict->get_cnDict()[*it].first;
            _resultQue.push(result);


            if(_dict->get_enDict().size()<=*it)
            {
                continue;
            }
            else
            {
              result=_dict->get_enDict()[*it].first;
                _resultQue.push(result);
            }

        }
    }
}


size_t nBytesCode(const char ch)
{
    if(ch & (1 << 7))
    {
        int nBytes = 1;
        for(int idx = 0; idx != 6; ++idx)
        {
            if(ch & (1 << (6 - idx)))
            {
                ++nBytes;
            }
            else
                break;
        }
        return nBytes;
    }
    return 1;
}


std::size_t length(const std::string &str)
{
    std::size_t ilen = 0;
    for(std::size_t idx = 0; idx != str.size(); ++idx)
    {
        int nBytes = nBytesCode(str[idx]);
        idx += (nBytes - 1);
        ++ilen;
    }
    return ilen;
}


int triple_min(const int &a, const int &b, const int &c)
{
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int editDistance(const std::string & lhs, const std::string &rhs)
{
    //¼ÆËã×îÐ¡±à¼­¾àÀë-°üÀ¨´¦ÀíÖÐÓ¢ÎÄ
    size_t lhs_len = length(lhs);
    size_t rhs_len = length(rhs);
    int editDist[lhs_len + 1][rhs_len + 1];
    for(size_t idx = 0; idx <= lhs_len; ++idx)
    {
        editDist[idx][0] = idx;
    }
    for(size_t idx = 0; idx <= rhs_len; ++idx)
    {
        editDist[0][idx] = idx;
    }
    std::string sublhs, subrhs;
    for(std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i,
        ++lhs_idx)
    {
        size_t nBytes = nBytesCode(lhs[lhs_idx]);
        sublhs = lhs.substr(lhs_idx, nBytes);
        lhs_idx += (nBytes - 1);
        for(std::size_t dist_j = 1, rhs_idx = 0;
            dist_j <= rhs_len; ++dist_j, ++rhs_idx)
        {
            nBytes = nBytesCode(rhs[rhs_idx]);
            subrhs = rhs.substr(rhs_idx, nBytes);
            rhs_idx += (nBytes - 1);
            if(sublhs == subrhs)
            {
                editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j -
                    1];
            }
            else
            {
                editDist[dist_i][dist_j] =
                    triple_min(editDist[dist_i][dist_j - 1] + 1,
                               editDist[dist_i - 1][dist_j] + 1,
                               editDist[dist_i - 1][dist_j - 1] + 1);
            }
        }
    }
    return editDist[lhs_len][rhs_len];
}

int distance(const std::string & lhs, const std::string &rhs)
{
    return editDistance(lhs,rhs);
}

