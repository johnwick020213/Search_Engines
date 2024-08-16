#ifndef __SPLITTOOLCPPJIEBA_H__
#define __SPLITTOOLCPPJIEBA_H__

#include"SplitTool.h"

class SplitToolCppJieba
:public SplitTool
{
public:
    SplitToolCppJieba();
    virtual ~SplitToolCppJieba();
    virtual vector<string> cut(const string & sentence=0);

private:
    Configuration &_conf;
};
   

#endif 

