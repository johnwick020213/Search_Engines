#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__


#include <map>
#include <string>

using std::string;
using std::map;

class Configuration
{
public:
    Configuration(const string&filepath);
    
    map<string,string>&getConfigMap();
private:
    string _filepath;
    map<string,string>_configMap;
    
};

   

#endif 

