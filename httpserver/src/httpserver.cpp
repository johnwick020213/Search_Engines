#include<signal.h>
#include<workflow/WFFacilities.h>
#include<wfrest/HttpServer.h>
#include<iostream>
#include<wfrest/json.hpp>
#include"../include/KeyRecommander.h"
#include<cctype>
#include<iomanip>
#include"../include/Dictionary.h"
#include"../include/WebPageQuery.h"


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

// 判断字符串是否包含 URL 编码
bool containsUrlEncoding(const std::string& str) {
    return str.find('%') != std::string::npos;
}


// 将 URL 编码字符串转换为原始字符串
std::string decodeURIComponent(const std::string& encoded) {
    std::string decoded;
    decoded.reserve(encoded.size());

    for (size_t i = 0; i < encoded.length(); ++i) {
        if (encoded[i] == '%') {
            if (i + 2 < encoded.length()) {
                int value;
                std::istringstream hex_stream(encoded.substr(i + 1, 2));
                hex_stream >> std::hex >> value;
                decoded += static_cast<char>(value);
                i += 2;
            }
        } else {
            decoded += encoded[i];
        }
    }
    return decoded;
}

// 将双重编码的 URL 编码字符串转换为中文
std::string decodeAndConvertToChinese(const std::string& double_encoded) {
    std::string first_decode = decodeURIComponent(double_encoded);
    std::string final_decode = decodeURIComponent(first_decode);
    return final_decode;
}

int main()
{
    signal(SIGINT, sighandler);
    wfrest::HttpServer server;
    //预热
    Dictionary *dict=Dictionary::createInstance();
    
    WebPageQuery* query=WebPageQuery::getInstance();

    //----------------------------------
    //业务代码

    //1.搜索
    server.GET("/search/{text}", [query](const wfrest::HttpReq *req, wfrest::HttpResp *resp){
        // 获取查询字符串(双重编码的中文)
        const string& usr_input_ori=req->param("text");
        //解码
        string tmpinput;
        if (containsUrlEncoding(usr_input_ori)) {
           tmpinput = decodeAndConvertToChinese(usr_input_ori);
        }
        else
        {
            tmpinput=usr_input_ori;
        }
        const string usr_input=tmpinput;
        
        
        /* Json retStr={ */
        /*     {"results", { */
        /*         {{"summary", "...你好..."}, {"title", "测试标题1"}, {"url", "http://www.baidu.com"}}, */
        /*         {{"summary", "...再见..."}, {"title", "测试标题2"}, {"url", "http://www.google.com"}}, */
        /*         {{"summary", "...再见..."}, {"title", "测试标题2"}, {"url", "http://www.google.com"}} */

        /*     }} */
        /* }; */
        /* resp->Json(retStr); */


        Json json_object=query->doQuery(usr_input);
        resp->headers["Access-Control-Allow-Origin"] = "*";  // 允许跨域请求
        resp->Json(json_object);
    });

    //2.推荐
    server.GET("/advice/{text}", [dict](const wfrest::HttpReq *req, wfrest::HttpResp *resp) {
        // 获取查询字符串(双重编码的中文)
        const string& usr_input_ori=req->param("text");
        //解码
        string tmpinput;
        if (containsUrlEncoding(usr_input_ori)) {
           tmpinput = decodeAndConvertToChinese(usr_input_ori);
        }
        else
        {
            tmpinput=usr_input_ori;
        }
        
        const string usr_input=tmpinput;
 
        //查询 
        KeyRecommander kr(usr_input,dict);
        // 接收结果打包成json发给客户端
        Json json_object=kr.get_result();

        resp->Json(json_object);
        resp->headers["Access-Control-Allow-Origin"] = "*";  // 允许跨域请求
    });

    //----------------------------------
    //服务器开启
    if (server.start(8086) == 0)
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
