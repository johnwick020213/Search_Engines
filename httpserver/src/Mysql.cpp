#include "../include/Mysql.h"

#include<set>
#include<utility>
#include<string>

using namespace std;


 Mysql::Mysql() {
    _mysql=mysql_init(NULL);
    if(_mysql==NULL)
    {
        fprintf(stderr,"mysql_init failed\n");
        return;
    }

    MYSQL*ret = mysql_real_connect(_mysql,"192.168.7.78","root","020213","test",0,NULL,0);
    if(ret==NULL)
    {
        fprintf(stderr,"mysql_real_connect:%s\n",mysql_error(_mysql));
        mysql_close(_mysql);
        _mysql=NULL;
    }
    else
    {
        cout<<"mysql_real_connect success\n";
    }
}

 Mysql::~Mysql() {
    if(_mysql!=nullptr)
    {
        mysql_close(_mysql);
        _mysql=nullptr;
    }
}

result Mysql::query(const char* sql) {
    vector<vector<string>>result;


    if(mysql_query(_mysql,sql)!=0)
    {
        return result;
    }

    _res=mysql_store_result(_mysql);

    if(_res==nullptr)
    {
        return result;
    }

    int num_fields=mysql_num_fields(_res);

    MYSQL_ROW row;
    while((row=mysql_fetch_row(_res)))
    {
        vector<string>row_data;
        for(int i=0;i<num_fields;++i)
        {
            row_data.push_back(row[i]?row[i]:" ");
        }

        result.push_back(row_data);
    }

    mysql_free_result(_res);

    return result;
}

int Mysql::create_table() {
    const char* sql = "CREATE TABLE IF NOT EXISTS dict_english (word VARCHAR(255), frequency INT, INDEX idx_word (word));"
                      "CREATE TABLE IF NOT EXISTS index_english (word VARCHAR(255), index_value INT,INDEX idx_word (word));"
                      "CREATE TABLE IF NOT EXISTS dict_chinese (word VARCHAR(255) PRIMARY KEY, frequency INT,INDEX idx_word (word));"
                      "CREATE TABLE IF NOT EXISTS index_chinese (word VARCHAR(255), index_value INT,INDEX idx_word (word));"
                      "CREATE TABLE IF NOT EXISTS invertIndex (word VARCHAR(255), docid INT, weight DOUBLE,INDEX idx_word (word));"//倒排索引库
                      "CREATE TABLE IF NOT EXISTS ripepage (docid INT PRIMARY KEY, url VARCHAR(255), title VARCHAR(255), content LONGTEXT BINARY NOT NULL, doc LONGTEXT BINARY NOT NULL);"//网页库
                      "CREATE TABLE IF NOT EXISTS offset (docid INT PRIMARY KEY, beginpos INT, doclength INT);";//便宜库

    if (_mysql == nullptr) {
        fprintf(stderr, "Not connected to MySQL server.\n");
        return -1;
    }

    if (mysql_query(_mysql, sql) != 0) {
        fprintf(stderr, "Failed to execute MySQL query: %s\n", mysql_error(_mysql));
        return -1;
    }

    return 0;
}

int Mysql::insert(const char*sql) {
    if(_mysql==nullptr)
    {
        return -1;
    }

    if(mysql_query(_mysql,sql)!=0)
    {
        return -2;
    }

    int num=mysql_affected_rows(_mysql);
    if(num>0)
    {
        cout<<"成功插入："<<num<<"行\n";
    }

    return num;
}

