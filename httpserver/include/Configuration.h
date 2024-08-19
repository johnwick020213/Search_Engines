#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <iostream>
#include <string>
#include<set>
#include<map>

using std::string;
using std::map;
using std::set;

class Configuration
{
public:
    Configuration(const string &filepath);
    map<string,string> & getConfigMap();
    set<string>getStopWordList();

private:
    string _filepath;
    map<string,string> _configMap;
    set<string>_stopWordList;

    void put_configMap();
    void put_stopWordList();
};
   

#endif 

