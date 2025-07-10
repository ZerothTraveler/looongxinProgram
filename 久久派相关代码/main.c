#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include "hx711.h"
#include "camera.h"
#include "TCP_client.h"
#include "pthread.h"
#include "switch.h"

hx711_t modules[MAX_MODULES] = {
    {.gpio_dout = "64", .gpio_sck ="60", .GapValue = 720}, //毛皮：8873850
    {.gpio_dout = "67",.gpio_sck="63", .GapValue = 490},//毛皮：8022050
    {.gpio_dout = "65",.gpio_sck="61",.GapValue = 650},  //毛皮：8227020
    {.gpio_dout = "66", .gpio_sck="62" ,.GapValue = 520},//毛皮：8792450
};
int module_count =4;       //模块的个数

// ===== 主函数 =====
int main() {

    system("dmesg -n 1");  // 降低内核日志级别
    system("echo 0 > /sys/class/graphics/fbcon/cursor_blink"); // 关闭光标
    
    gpio_export_switch();                   // 导出 GPIO 并设置为输出
    int fd = gpio_open_switch();            // 打开 value 文件
    write(fd, "1", 1);                      // 设置为高电平
    close(fd);

    pthread_t pid_read, pid_camera;
    int sockfd = tcp_client_init(); 
    gpio_export_switch();
    //led_init();
    if (sockfd < 0) {
        fprintf(stderr, "TCP client init failed.\n");
        return 1;
    }

    for (int i = 0; i < module_count; i++) {
        hx711_init(&modules[i]);
        // if (hx711_init(&modules[i]) == 0) {
        //     fprintf(stderr, "HX711[%d] init failed,[%d].\n", i,hx711_init(&modules[i]));
        //     // 清理已经初始化的模块
        //     for (int j = 0; j < i; j++) {
        //         hx711_cleanup(&modules[j]);
        //     }
        //     close(sockfd);
        //     return 1;
        // }
    }

    //   for (int i = 0; i < module_count; i++) {
    //       Get_Maopi(&modules[i]);  
    //  }




    // 创建线程用于收消息
    pthread_create(&pid_read, NULL, read_from_server, (void*)&sockfd);
    pthread_create(&pid_camera, NULL, camera_use, NULL);

    // 阻塞主线程等待子线程结束
    pthread_join(pid_read, NULL);
    pthread_join(pid_camera, NULL);

    printf("释放资源\n");
    close(sockfd);

    for (int i = 0; i < module_count; i++) {
        hx711_cleanup(&modules[i]);
    }

    return 0;
}
