#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "TCP_client.h"
#include "hx711.h"

#define switch_pin "73" //电磁铁
//#define tickling_pin "89" //反馈


void gpio_export_switch() {
    int fd;
    char path[64];
    snprintf(path, sizeof(path), GPIO_PATH "/gpio%s", switch_pin);
    if (access(path, F_OK) == 0) {

        return;
    }

    fd = open(GPIO_PATH "/export", O_WRONLY);
    if (fd >= 0) {
        if(write(fd,switch_pin, strlen(switch_pin))<0)
        {
            perror("Failed to export GPIO");            
        }
        close(fd);
    }
    snprintf(path, sizeof(path), GPIO_PATH "/gpio%s/direction", switch_pin);
    fd = open(path, O_WRONLY);
    write(fd, "out", 3);
    close(fd);

}
int gpio_open_switch()
{
    char path[64];
    snprintf(path,sizeof(path),GPIO_PATH"/gpio%s/value",switch_pin);
    return open(path,O_WRONLY);
}

void gpio_write_switch()
{
    gpio_export_switch();
    int fd;
    fd = gpio_open_switch();
    if (fd >= 0) {
    if (write(fd, "0", 1) != 1) {
        perror("写入低电平失败");
    }    
    printf("准备打开电磁铁...,{%d}\n",fd); 
    usleep(1500000);
    if (write(fd, "1", 1) != 1) {
        perror("写入高电平失败");
    }
    close(fd);    
    }else {
        perror("打开GPIO失败");
    }

}

//反馈
// void gpio_export_tickling() {

//     int fd;
//     char path[64];
//     snprintf(path, sizeof(path), GPIO_PATH "/gpio%s", tickling_pin);
//     if (access(path, F_OK) == 0) {
//         return;
//     }

//     fd = open(GPIO_PATH "/export", O_WRONLY);
//     if (fd >= 0) {
//        if(write(fd,tickling_pin, strlen(tickling_pin))<0)
//        {
//             perror("Failed to export GPIO");            
//        }
//         close(fd);
//     }

//     snprintf(path, sizeof(path), GPIO_PATH "/gpio%s/direction", tickling_pin);
//     fd = open(path, O_WRONLY);
//     write(fd, "in", 2);
//     close(fd);
// }

// int gpio_fd_open() {
//     int fd;
//     char buf[64];
//     snprintf(buf, sizeof(buf), GPIO_PATH"/gpio%s/value",tickling_pin);
//     fd = open(buf, O_RDONLY | O_NONBLOCK);
//     if (fd < 0) {
//         perror("无法打开 value 文件");
//     }
//     return fd;
// }
// int gpio_read_value(int fd) {

//     char value;
//     lseek(fd, 0, SEEK_SET);         //回到文件开头
//     if (read(fd, &value, 1) != 1) {
//         perror("读取 value 失败");
//         return -1;
//     }
//     return (value == '1') ? 1 : 0;
// }





