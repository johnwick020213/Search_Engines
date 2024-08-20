#ifndef __DIRSCANNER_H__
#define __DIRSCANNER_H__

#include <iostream>
#include <string>
#include<vector>

using std::vector;
using std::string;

class DirScanner
{
public:
    DirScanner();
    void operator()(string &dirname);
    vector<string>files();
    void traverse(const string &dirname);

private:
    vector<string> _files;
};

   

#endif 

