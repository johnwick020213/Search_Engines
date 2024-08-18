#include "../include/DictProducer.h"
#include<dirent.h>
#include<string.h> 
#include<iostream>
#include<fstream>
#include<sstream>

using std::ifstream;
using std::istringstream;
using std::ofstream;


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

void DictProducer::buildEnDict()
{
    for(auto filename:_files)
    {
        ifstream ifs(filename);
        if(!ifs)
        {
            std::cerr<<"error_in_DictProducer::buildEnDict:文件打开失败\n";
        }

        string line;
        string word;
        while(getline(ifs,line))
        {
            istringstream iss(line);
            while(iss>>word)   
            {
                clearWord(word);

                if(word!=string())
                {
                    pushDict(word);
                }
            }
        }
        ifs.close();
    }
}

void DictProducer::buildCnDict()
{
    for(auto filename:_files)
    {
        ifstream ifs(filename);
        if(!ifs)
        {
            std::cerr<<"error_in_DictProducer::buildCnDict:文件打开失败\n";
            return;
        }

        string line;
        string lines;//用一个字符串存储整个文章
        vector<string>words;//jieba返回的是一个容器
        while(getline(ifs,line))
        {
            //存储整篇文章
            lines.append(line);//为了去除'\n'
        }

        words=_splitTool->cut(lines);
        for(auto word:words)
        {
            pushDict(word);    
        }
        ifs.close();

    }
    return;
}

void DictProducer::storeDict(const char *filepath) {
    ofstream oss(filepath);
    if(!oss)
    {    
        std::cerr<<"error_in_DictProducer::storeDict:存储文件打开失败\n";
        return;
    }

    map<string,int>::iterator it=_dict.begin();
    for(;it!=_dict.end();++it)
    {
        oss<<it->first<<" "<<it->second<<"\n";
    }

    oss.close();
}

void DictProducer::getFiles()
{
    DIR *dir;
    struct dirent *ptr;
    dir=opendir(_dir.c_str());
    if (dir == nullptr) {
        std::cerr << "error_in_DictProducer::getFiles:无法打开目录: " << _dir << "\n";
        return;
    }

    string path=_dir+"/";//拷贝构造

    while((ptr=readdir(dir))!=NULL)//readdir会返回下一项地址，直到读完返回nullptr
    {
        if(!strcmp(ptr->d_name,".")||!strcmp(ptr->d_name,".."))//相同返回假
            continue;
        _files.push_back(path+ptr->d_name);
    }   
    closedir(dir);
    return;
}

void DictProducer::pushDict(const string & word) {
    _dict[word]++;
    return;
}

void clearWord(string &word)
{
    auto it=word.begin();
    while(it!=word.end())
    {
        if(!isalpha(*it))
        {
            it=word.erase(it);
        }
        else if(*it>'A'&&*it<'Z')
        {
            *it+=32;
            ++it;
        }
        else
        {
            ++it;
        }
    }
}
