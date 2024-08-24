# 定义编译器和编译选项
CXX = g++ 
CXXFLAGS = -std=c++11 -Wall -g
LDFLAGS = -lwfrest -lmysqlclient -licuuc -licudata -licui18n -lhiredis

# 定义目录
INCLUDE_DIR = include
SRC_DIR = src
BIN_DIR = bin

# 定义源文件和头文件
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRCS))

# 定义最终生成的可执行文件
TARGET = ./httpserver

# 规则：生成目标文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# 规则：生成.o文件
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# 伪目标：清理生成文件
.PHONY: clean
clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET)

