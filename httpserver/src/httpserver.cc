#include<signal.h>
#include<workflow/WFFacilities.h>
#include<wfrest/HttpServer.h>
#include<iostream>
#include<wfrest/json.hpp>
#include"../include/KeyRecommander.h"

using Json = nlohmann::json;

using std::cout;
using std::map;
using std::string;

static WFFacilities::WaitGroup waitGroup(1);

void sighandler(int signum)
{
    cout << "\ndone\n";
    waitGroup.done();
}

int main()
{
    signal(SIGINT, sighandler);
    wfrest::HttpServer server;

    //----------------------------------
    //业务代码

    //1.搜索
    server.GET("/search/{text}", [](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        cout << "hello\n";
        resp->String("<html>Hello</html>");
        resp->headers["Content-Type"] = "text/html";
        resp->headers["Access-Control-Allow-Origin"] = "*";  // 允许跨域请求
    });

    //2.推荐
    server.GET("/advice", [](const wfrest::HttpReq *req, wfrest::HttpResp *resp) {
        // 获取查询字符串
        Json req_json=Json::parse(req->body());
        //客户端json=[input:用户输入]
        string usr_input=req_json["input"];

        KeyRecommander kr(usr_input);
        // 接收结果打包成json发给客户端
        Json json_object=kr.get_result();

        // 添加推荐词到 json 对象中
        json_object.push_back(usr_input);
        json_object.push_back("推荐词2");
        json_object.push_back("推荐词3");
        json_object.push_back("推荐词4");
        json_object.push_back("推荐词5");
        json_object.push_back("推荐词6");
        json_object.push_back("推荐词7");
        json_object.push_back("推荐词8");
        json_object.push_back("推荐词9");
        json_object.push_back(usr_input);

        resp->Json(json_object);
        resp->headers["Access-Control-Allow-Origin"] = "*";  // 允许跨域请求
    });

    //----------------------------------
    //服务器开启
    if (server.track().start(8086) == 0)
    {
        server.list_routes();
        waitGroup.wait();
        cout << "finished\n";
        server.stop();
        return 0;
    }
    else
    {
        perror("HttpServer start fail!");
        return -1;
    }

    return 0;
}
