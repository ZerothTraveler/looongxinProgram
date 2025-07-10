#ifndef __SWITCH_H
#define __SWITCH_H

// ========== 电磁铁输出相关函数 ==========
void gpio_export_switch(void);         // 导出电磁铁 GPIO 并设置为输出
int  gpio_open_switch(void);           // 打开电磁铁 value 文件
void gpio_write_switch(void);          // 控制电磁铁：高电平1秒 -> 低电平

// ========== 输入引脚（反馈）相关函数 ==========
// void gpio_export_tickling(void);       // 导出反馈引脚并设置为输入
// int  gpio_fd_open();               // 打开反馈 value 文件（带 O_NONBLOCK）
// int  gpio_read_value(int fd);          // 读取反馈引脚电平（0 或 1）

#endif
