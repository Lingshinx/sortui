# Makefile

# 默认目标：编译项目
all:
	cmake -S . -B build
	cmake --build build

# 运行目标：编译并运行项目
run: all
	./bin/sortui

# 清理目标：删除生成的文件
clean:
	rm -rf build
