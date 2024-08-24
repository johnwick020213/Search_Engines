#ifndef __REDIS_H__
#define __REDIS_H__

#include <hiredis/hiredis.h>
#include <iostream>
#include <string>
#include<wfrest/json.hpp>

using Json=nlohmann::json;
using std::cout;
using std::endl;

using std::string;

class Redis
{
public:
    Redis();
    ~Redis();
    bool connect(const string &host, int port);
    void set(const string key, Json value) const;
    Json get(const string key) const;

private:
    redisContext *_pConnect;
    mutable redisReply *_pReply;
};

#endif
