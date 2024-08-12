#include "NonCopyable.h"

NonCopyable(const NonCopyable&)=delete {

}

NonCopyable &operator=(const NonCopyable &)=delete  {

}

