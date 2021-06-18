# 指定编译器
CC := gcc
# 指定编译器 gcc 的编译参数
CFLAGS := -Wall -g -Os
# 指定共享目录，里面存放的是共用的代码文件
SHDIR := ../common
# 中间文件
OBJS = ftserve.o $(SHDIR)/common.o
# 编译最终生成目标
all: ftserve
# 通过中间文件编译生成最终目标文件
ftserve: $(OBJS)
	@$(CC) -o ftserve $(CFLAGS) $(OBJS)
# 编译生成中间目标的方式，通过所有的 .c 文件生成对应的 .o 文件
$(OBJS) : %.o: %.c 
	@$(CC) -c $(CFLAGS) $< -o $@
# 伪目标，清理中间文件，并在命令行中回显清理完成消息
.PHONY:
clean:
	@rm -f *.o ftserve
	@rm -f ../common/*.o
	@echo Done cleaning
