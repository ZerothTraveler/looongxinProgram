CC = loongarch64-linux-gnu-gcc                # 指定编译器
CFLAGS = -Wall -g      # 编译选项：开启所有警告并生成调试信息
LDFLAGS = -lpthread -lm
SRCS = main.c hx711.c TCP_client.c camera.c switch.c  # 所有源文件
OBJS = $(SRCS:.c=.o)         # 源文件对应的目标文件（.o）
TARGET = app            # 生成的可执行文件名

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean

