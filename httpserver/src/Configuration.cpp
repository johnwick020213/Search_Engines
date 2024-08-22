#include "../include/Configuration.h"
#include<fstream>
#include<sstream>
#include<iostream>

using std::ifstream;
using std::istringstream;
using std::cerr;
Configuration* Configuration::_pInstance = nullptr;
Configuration*  Configuration::getInstance(const string &filepath)
{
    if(_pInstance==nullptr)
    {
        _pInstance=new Configuration(filepath);
    }

    return _pInstance;
}

 Configuration::Configuration(const string &filepath)
:_filepath(filepath)
{
    put_configMap();
    put_stopWordList();
}

map<string,string> & Configuration::getConfigMap()
{
    return _configMap;
}

 set<string> Configuration::getStopWordList()
{
    return _stopWordList;
}

void Configuration::put_configMap()
{
    ifstream ifs(_filepath);
    if(!ifs.is_open())
    {
        cerr<<"error_in_Configuration::get_configMap:打开配置文件失败!\n";
        exit(1);
    }
    
    string line;
    while(getline(ifs,line))
    {
        if(line.size()==0)
        {
            continue;
        }

        string filename,filepath;
        istringstream iss(line);
        iss>>filename;
        iss>>filepath;
        if(filepath.size()>0&&filename.size()>0)
        {
            _configMap.insert(make_pair(filename,filepath));
        }
        else
        {
            break;
        }

        iss.clear();
    }

    ifs.close();

    
}


void Configuration::put_stopWordList()
{
    string stopWordList_path=_configMap["STOP_WORDS_LIST_CN_PATH"];
    ifstream ifs(stopWordList_path);
    if(!ifs.is_open())
    {
        cerr<<"error_in_Configuration::get_stopWordList:打开停用词文件失败！\n";
        exit(1);
    }


    string word;
    while(ifs>>word)
    {
        _stopWordList.insert(word);
    }

    ifs.close();
}

