#include"../include/Configuration.h"
#include<iostream>
#include<string>
#include<map>
#include<fstream>
#include<stdlib.h>
#include<sstream>
#include<unistd.h>
#include<stdio.h>

using std::ifstream;
using std::istringstream;
using std::string;
using std::map;
using std::ifstream;
using std::cout;
using std::make_pair;


Configuration::Configuration(const string& filepath)
    :_filepath(filepath)
{
    ifstream ifs(_filepath,std::ios::in);
    
    //打开失败报错，三秒后退出
    if(!ifs)
    {
        sleep(1);
        for(int i=3;i>=0;i--)
        {
            cout<<"error_in_Configuration::Configuration:打开"<<_filepath<<"时出现以下错误：\n";
            perror("fopen");
            printf("\n系统将在%d秒后退出！\n",i);
            sleep(1);

        }
        exit(1);
    }

    string line,key,_path;
    while(getline(ifs,line))
    {
        istringstream istr(line);
        istr>>key;
        istr>>_path;
        _configMap.insert(make_pair(key,_path));
    }

}

map<string,string>& Configuration::getConfigMap()
{
    return _configMap;
}

