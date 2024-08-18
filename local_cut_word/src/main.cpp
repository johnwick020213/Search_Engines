#include"../include/DictProducer.h"
#include"../include/Configuration.h"
#include"../include/SplitCppJieba.h"
#include"../include/index.h"

#include<iostream>

using namespace std;

int main()
{
    string config_file_path;
    config_file_path="../conf/yuliao.conf";
    map<string,string>data_dir(Configuration(config_file_path).getConfigMap());
    string eng_store_path=data_dir["data_dir:"]+"/";
    DictProducer eng_dictproducer(data_dir["eng_dir:"]);
    eng_dictproducer.storeDict(eng_store_path.append("english.dat"));

    SplitToolCppJieba jieba;
    DictProducer cn_dictproducer(data_dir["cn_dir:"],&jieba);
    string cn_store_path=data_dir["data_dir:"]+"/";
    cn_dictproducer.storeDict(cn_store_path.append("chinese.dat"));

    
    string index_store_path=data_dir["data_dir:"]+"/";
    Index index;
    index.init(eng_store_path,cn_store_path);
    index.store_index(index_store_path.append("index.dat"));

    return 0;
}

