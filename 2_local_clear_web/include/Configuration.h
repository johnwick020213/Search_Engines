#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <iostream>
#include <string>
#include<set>
#include<map>

using std::string;
using std::map;
using std::set;

//单例类
class Configuration
{
public:
    static Configuration* getInstance(const string &filepath);
private:
    Configuration(const string &filepath);
    map<string,string> & getConfigMap();
    set<string>getStopWordList();
    void put_configMap();
    void put_stopWordList();

    string _filepath;
    map<string,string> _configMap;
    set<string>_stopWordList;
    //单例类的指针
    static Configuration* _pInstance;

};
   

#endif 

