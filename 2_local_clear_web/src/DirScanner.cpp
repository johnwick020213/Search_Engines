#include "../include/DirScanner.h"
#include<dirent.h>
#include<iostream>
#include<string.h>

using std::cerr;

 DirScanner::DirScanner() {}

void DirScanner::operator()(string &dir_name) {
    traverse(dir_name);
}

 vector<string>DirScanner::files() {
    return _files;
}

void DirScanner::traverse(const string &dirname) {
   DIR *dir;
   struct dirent *ptr;
   dir=opendir(dirname.c_str());
   if(dir==nullptr)
   {
       cerr<<"error_in_DirScanner::traverse:打开文件失败！\n";
       exit(1);
   }

    string _path = dirname+"/";

    while((ptr=readdir(dir))!=NULL)
    {
        if(!strcmp(ptr->d_name,".")||!strcmp(ptr->d_name,".."))
           continue;
        _files.push_back(_path+ptr->d_name);
    }
    closedir(dir);
    return;

}

