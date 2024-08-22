#ifndef __MYSQL_H__
#define __MYSQL_H__

#include<mysql/mysql.h>
#include <iostream>
#include <string>
#include<vector>

using std::string;
using std::cout;
using std::endl;
using std::vector;

using result = vector<vector<string>>;

class Mysql
{
public:
    Mysql();
    ~Mysql();
    result query(const char* sql);
    int create_table();
    int insert(const char*sql);

private:
    MYSQL* _mysql;
    MYSQL_RES* _res;
};

   

#endif 

