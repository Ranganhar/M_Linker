# 编译器
CXX = g++
# 编译选项
CXXFLAGS = -Wall -Wextra -std=c++17 -g
# 可执行文件名
TARGET = ld

TESTS := $(wildcard test/*.sh)

# 查找所有的 .c .cpp 文件
SRCS := $(shell find . -type f \( -name "*.c" -o -name "*.cpp" \))

# 默认目标
build: $(TARGET)

# 直接编译并链接成可执行文件
$(TARGET):
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

$(TESTS):
	@echo 'testing' $@
	@./$@
	@printf '\e[32mOK\e[0m\n'

test: build
	$(MAKE) $(TESTS)
	@printf '\e[32mPassed All Tests!\e[0m\n'
# 清理生成的文件
clean:
	rm -f $(TARGET)
	rm -rf out/
all: clean test

# .PHONY 表示这些目标不是文件
.PHONY: build clean test $(TESTS) all