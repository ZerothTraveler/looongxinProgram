#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "TCP_client.h"
#include "hx711.h"
#include "switch.h"
#include <sys/time.h>

// 全局变量
extern int module_count;
extern hx711_t modules[MAX_MODULES];
extern int camera_open;                       // 来自其他模块或主程序
extern pthread_mutex_t camera_lock;

//#define SERVER_IP   "192.168.100.54"        //对方
#define SERVER_IP   "192.168.4.1"    //linux
#define SERVER_PORT 1001
#define BUFFER_SIZE 1024


// 接收线程函数
void *read_from_server(void *arg) {
    int client_fd = *(int *)arg;
    char read_buf[BUFFER_SIZE] = {0};
    //time_t last_ping = time(NULL);
   
    while (1) {

        ssize_t count = recv(client_fd, read_buf, sizeof(read_buf) - 1, 0);

        if (count > 0) {
            read_buf[count] = '\0'; // 确保是字符串
            printf("接收到命令: %s\n", read_buf);
            
            // 简单命令：一个字符 '0' ~ '5'
            if (read_buf[0] >= '0' && read_buf[0] <= '4') {
                send_weight_by_index(client_fd, read_buf[0] - '0');
            } else if(read_buf[0] >= '5' && read_buf[0] <= '9'){
                send_weight_by_init(client_fd, read_buf[0] - '0');

            }else if (strncmp(read_buf, "over", 4) == 0) {
                //发送全部字符
                send_all_weights(client_fd);
            }else if(strncmp(read_buf, "open_lock", 9) == 0) 
            {
                gpio_write_switch();        //开门
             

            }else if(strncmp(read_buf, "reco_start", 10) == 0)     //打开摄像头
            {
                    pthread_mutex_lock(&camera_lock);
                    camera_open=1;
                    pthread_mutex_unlock(&camera_lock);

            }else if(strncmp(read_buf, "reco_close", 10) == 0)     //关闭摄像头
            {
                    pthread_mutex_lock(&camera_lock);
                    camera_open=0;
                    pthread_mutex_unlock(&camera_lock);

            }else if(strncmp(read_buf, "ID", 2) == 0)     //打开摄像头
            {
                    const char* confirm = "mcu\n";
                    ssize_t sent = send(client_fd, confirm, strlen(confirm), 0);
                     printf("信息: %ld", sent);
                    if (sent < 0) {
                        perror("send failed");
                    } else {
                        printf("连接成功，已发送确认信息: %s\n", confirm);


                }
            }else if (strncmp(read_buf, "MCUWE", 5) == 0)
            {
                    char buf[256] = {0};
                    int offset = 0;

                    // 添加命令前缀
                    offset += snprintf(buf + offset, sizeof(buf) - offset, "WE ");

                    // 拼接每个模块的毛皮值
                    for (int i = 0; i < module_count; i++) {
                        offset += snprintf(buf + offset, sizeof(buf) - offset, "%d ", Get_Maopi(&modules[i]));
                    }

                    // 可选：去掉末尾多余空格，替换为空字符（更整洁）
                    if (offset > 0 && buf[offset - 1] == ' ') {
                        buf[offset - 1] = '\0';
                        offset--;
                    }

                    // 发送给服务端
                    send(client_fd, buf, offset, 0);
                    printf("已发送原始数据: %s\n", buf);  // 调试用


            }else if (strncmp(read_buf, "WE", 2) == 0) {    //接收发送原始数据
                // 使用 sscanf 拆分命令 + 参数
                char command[16] = {0};
                char params[BUFFER_SIZE - 16] = {0};
                sscanf(read_buf, "%15s %[^\n]", command, params);  // eg: WE 8012345 8009876 ...

                int index = 0;
                char *token = strtok(params, " ");
                while (token != NULL && index < MAX_MODULES) {
                    modules[index].maopi = strtoul(token, NULL, 10);
                    index++;
                    token = strtok(NULL, " ");
                }

                printf("接收并更新 %d 个模块毛皮值\n", index);
            }else {
                fprintf(stderr, "未知命令: %s\n", read_buf);
            }
        } else if (count == 0) {
            printf("服务器关闭连接\n");
            break;
        } else {
            if (errno == EINTR) continue;
            perror("recv\n");
            break;
        }
    }
    close(client_fd);
    return NULL;
}
// 发送某个模块的原始数据
int send_weight_by_init(int client_fd, int index) {

    char buffer[64];
    double weight = hx711_read(&modules[index]);
    printf("weight:%f,index:%d",weight,index);
    // 格式化数据并加上换行符，便于接收端识别
    int len = snprintf(buffer, sizeof(buffer), "%.1f\n", weight);
    if (len < 0 || len >= sizeof(buffer)) {
        fprintf(stderr, "格式化发送数据失败：weight = %.1f\n", weight);
        return -1;
    }
    int total_sent = 0;
    while (total_sent < len) {
        ssize_t sent = send(client_fd, buffer + total_sent, len - total_sent, MSG_NOSIGNAL);
        if (sent < 0) {
            perror("send");
            return -1;
        }
        total_sent += sent;
    }

    printf("已发送模块 %d 数据: %.1f\n", index, weight);
    return 0;
}


// 发送某个模块的重量数据
int send_weight_by_index(int client_fd, int index) {
    if (index < 0 || index >= module_count) {
        fprintf(stderr, "模块索引 %d 超出范围\n", index);
        return -1;
    }

    char buffer[64];
    double weight = weight_filter(&modules[index]);
    printf("weight:%f,index:%d",weight,index);
    // 格式化数据并加上换行符，便于接收端识别
    int len = snprintf(buffer, sizeof(buffer), "%.1f\n", weight);
    if (len < 0 || len >= sizeof(buffer)) {
        fprintf(stderr, "格式化发送数据失败：weight = %.1f\n", weight);
        return -1;
    }

    // 确保数据全部发送出去（防止部分发送）
    int total_sent = 0;
    while (total_sent < len) {
        ssize_t sent = send(client_fd, buffer + total_sent, len - total_sent, MSG_NOSIGNAL);
        if (sent < 0) {
            perror("send");
            return -1;
        }
        total_sent += sent;
    }

    printf("已发送模块 %d 数据: %.1f\n", index, weight);
    return 0;
}



// 发送所有模块的重量数据
int send_all_weights(int client_fd) {
    char buffer[BUFFER_SIZE];
    int offset = 0;

    //buffer[offset++] = 'a';
    //buffer[offset++] = 'a';
    //buffer[offset++] = ',';
    for (int i = 0; i < module_count && i < MAX_MODULES; ++i) {
        
        double weight = weight_filter(&modules[i]);
        int written = snprintf(buffer + offset, sizeof(buffer) - offset,
                               "%.1f ",  weight);
        if (written < 0 || written >= (int)(sizeof(buffer) - offset)) {
            fprintf(stderr, "数据写入溢出\n");
            return -1;
        }
        offset += written;
    }

    // if (offset > 0 && buffer[offset - 1] == ',') {
    //     buffer[offset - 1] = 0; // 去掉最后一个逗号
    // }

    //buffer[offset++] = 'b';
    //buffer[offset++] = 'b';
    buffer[offset++] = '\n';
    printf("模块重量：%s",buffer);
    ssize_t sent = send(client_fd, buffer, offset, MSG_NOSIGNAL);
    if (sent < 0) {
        perror("send");
        return -1;
    }

    printf("已发送所有模块数据：%s\n",buffer);
    return 0;
}

// TCP客户端初始化函数
int tcp_client_init() {
    int sockfd;
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    server_addr.sin_port = htons(SERVER_PORT);

    while (1) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("socket");
            sleep(1);
            continue;
        }
        if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
            printf("成功连接到服务端 %s:%d\n", SERVER_IP, SERVER_PORT);
            struct timeval timeout;
            timeout.tv_sec = 3600; //接收定时唤醒
            timeout.tv_usec = 0;
            setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
            break;
        }

        perror("连接失败，重试中");
        close(sockfd);
        sleep(2);
    }

    return sockfd;
}
