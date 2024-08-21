# 编译器
CXX = g++
# C++ 标准
CXXFLAGS = -std=c++11 -I./include -I/path/to/wfrest/include -g
# 链接选项
LDFLAGS = -L/path/to/wfrest/lib -lworkflow -lwfrest -lpthread -g 

# 源文件
SRCDIR = src
INCLUDEDIR = include
SOURCES = $(SRCDIR)/Dictionary.cpp $(SRCDIR)/KeyRecommander.cpp $(SRCDIR)/httpserver.cc
OBJECTS = $(SOURCES:.cpp=.o)

# 目标可执行文件
TARGET = httpserver

# 编译规则
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# 清理
clean:
	rm -f $(OBJECTS) $(TARGET)

# 伪目标
.PHONY: all clean

