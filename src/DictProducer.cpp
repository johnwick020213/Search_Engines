#include "../include/DictProducer.h"
#include<dirent.h>
#include<string.h> 

DictProducer::DictProducer(const string &dir)
:_dir(dir)
{
    getFiles();
    buildEnDict();
}

 DictProducer::DictProducer(const string& dir,SplitTool *splitTool) 
:_dir(dir),_splitTool(splitTool)
{
    getFiles();
    buildCnDict();
}

void DictProducer::buildEnDict() {

}

void DictProducer::buildCnDict() {

}

void DictProducer::storeDict(const char *filepath) {

}

void DictProducer::getFiles()
{
    DIR *dir;
    struct dirent *ptr;
    dir=opendir(_dir.c_str());
    string path(_dir+"/");//拷贝构造

    while((ptr=readdir(dir))!=NULL)
    {
        if(!strcmp(ptr->d_name,".")||!strcmp(ptr->d_name,".."))//相同返回假
            continue;
        _files.push_back(path+ptr->d_name);
    }   
    closedir(dir);
    return;
}

void DictProducer::pushDict(const string & word) {

}

