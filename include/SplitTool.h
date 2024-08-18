#ifndef __SPLITTOOL_H__
#define __SPLITTOOL_H__

#include<vector>
#include<string>

using std::string;
using std::vector;

class SplitTool
{
public:
    SplitTool();
    
    virtual ~SplitTool();
    virtual vector<string> cut(const string & sentence)=0;
};
   

#endif 

