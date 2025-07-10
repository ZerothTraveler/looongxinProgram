#include "hx711.h"
#include "TCP_client.h"
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

extern hx711_t modules[MAX_MODULES];
long temp;


// ===== GPIO 操作函数 =====
int gpio_export(const char* gpio, int is_output) {
    char path[64];
    snprintf(path, sizeof(path), GPIO_PATH "/gpio%s", gpio);
    if (access(path, F_OK) == 0) {
        // 已经导出，设置方向
        snprintf(path, sizeof(path), GPIO_PATH "/gpio%s/direction", gpio);
        int fd = open(path, O_WRONLY);
        if (fd >= 0) {
            write(fd, is_output ? "out" : "in", is_output ? 3 : 2);
            close(fd);
        }
        return 0;
    }

    // 导出 GPIO
    int fd = open(GPIO_PATH "/export", O_WRONLY);
    if (fd < 0)
        return -1;
    write(fd, gpio, strlen(gpio));
    close(fd);

    usleep(100000);

    // 设置方向
    snprintf(path, sizeof(path), GPIO_PATH "/gpio%s/direction", gpio);
    fd = open(path, O_WRONLY);
    if (fd >= 0) {
        write(fd, is_output ? "out" : "in", is_output ? 3 : 2);
        close(fd);
    } else {
        return -1;
    }
    return 0;
}

int gpio_open_fd(const char* gpio, int is_output) {
    char path[64];
    snprintf(path, sizeof(path), GPIO_PATH "/gpio%s/value", gpio);
    printf("打开 GPIO %s 的 value 文件\n", gpio);
    for (int i = 0; i < 100; i++) {
        if (access(path, F_OK) == 0) {
            break;
        }
        usleep(1000);  // 每次等待 1ms，总共最多等 100ms
    }
    int fd = open(path, is_output ? O_WRONLY : O_RDONLY);
        if (fd < 0) {
        perror(path);  // 打印具体错误路径和原因
        fprintf(stderr, "路径是：%s\n", path);
    }
    return fd;
}

void gpio_write(int fd, int val) {
    char ch = val ? '1' : '0';
    lseek(fd, 0, SEEK_SET);
    write(fd, &ch, 1); 
}

int gpio_read(int fd) {
    char ch;
    lseek(fd, 0, SEEK_SET);
    read(fd, &ch, 1);
    return (ch == '1') ? 1 : 0;
}

// ===== HX711 操作函数 =====


uint32_t hx711_read(hx711_t* hx) {

    int32_t count = 0;
    gpio_write(hx->fd_sck, 0);
    while (gpio_read(hx->fd_dout));  // 等待 DOUT 变低

    for (int i = 0; i < 24; i++) {
        gpio_write(hx->fd_sck, 1);
        count = count << 1;
        gpio_write(hx->fd_sck, 0);
        if (gpio_read(hx->fd_dout))
            count++;
    }

    // 第25个脉冲，设增益为128
    gpio_write(hx->fd_sck, 1);
    count ^= 0x800000;
    gpio_write(hx->fd_sck, 0);

    return count;
}

// void measure_all_raw_and_error(long raw_values[MAX_MODULES], long errors[MAX_MODULES]) {    //获取压力传感器误差数据

//     for (int i = 0; i < MAX_MODULES; i++) {
//         long raw = hx711_read(&modules[i]);              // 实际读取值
//         long maopi = modules[i].maopi;                   // 写死的毛皮值
//         raw_values[i] = raw;                             // 存原始值
//         errors[i] = raw - maopi;                         // 误差 = 实测 - 毛皮
//         printf("模块[%d]: 原始值 = %ld, 毛皮 = %ld, 误差 = %ld\n", i, raw, maopi, errors[i]);
//     }
// }
//     long raw_values[MAX_MODULES];
//     long errors[MAX_MODULES];

// void zero_set()
// {
//     measure_all_raw_and_error(raw_values, errors);


// }

double Get_Weight(hx711_t* hx) {    //获取实际重量
    long raw = hx711_read(hx) - hx->maopi;
    if(raw>0)
    {
        return (double)raw/(double)hx->GapValue;
    }
    else
        return 0;
}


/******************
*
*中值滤波
*
*******************/

  double weight_filter(hx711_t* hx) {
      double weight_buffer[40] = {0};

      for (int count = 0; count <40; count++) {
        
          double new_val = Get_Weight(hx);
         int i;
          for (i = count - 1; i >= 0 && weight_buffer[i] > new_val; i--) {
              weight_buffer[i + 1] = weight_buffer[i];
          }
          weight_buffer[i + 1] = new_val;
      }

      return weight_buffer[15];  // 中值滤波
  }



// int compare_double(const void* a, const void* b) {
//     double diff = *(double*)a - *(double*)b;
//     return (diff > 0) - (diff < 0);  // 相当于返回 1 / 0 / -1
// }

// double weight_filter(hx711_t* hx) {     
//     double weight_buffer[40];

//     // 1. 获取40个称重值（不排序）
//     for (int i = 0; i < 40; i++) {
//         weight_buffer[i] = Get_Weight(hx);
//     }

//     // 2. 使用快速排序对整个数组排序
//     qsort(weight_buffer, 40, sizeof(double), compare_double);

//     // 3. 取第10到15位（中间部分）做平均，抗干扰能力强
//     double sum = 0;
//     for (int i = 10; i <= 15; i++) {
//         sum += weight_buffer[i];
//     }

//     return sum / 6.0;
// }



int Get_Maopi(hx711_t* hx) {
    hx->maopi= hx711_read(hx);
    return hx->maopi;
}


int hx711_init(hx711_t* hx) {

    gpio_export(hx->gpio_dout,0);
    gpio_export(hx->gpio_sck,1);

    usleep(10000);  // 等待GPIO sysfs准备好

    hx->fd_dout = gpio_open_fd(hx->gpio_dout, 0);
    hx->fd_sck  = gpio_open_fd(hx->gpio_sck, 1);
    //hx->maopi   = 0;
    printf("fd_dout:%d,fd_sck:%d\n",hx->fd_dout,hx->fd_sck);

    gpio_write(hx->fd_sck, 1);
    return (hx->fd_dout >= 0 && hx->fd_sck >= 0);
}

void hx711_cleanup(hx711_t* hx) {
    if (hx->fd_dout >= 0) {
        close(hx->fd_dout);
        hx->fd_dout = -1;
    }
    if (hx->fd_sck >= 0) {
        close(hx->fd_sck);
        hx->fd_sck = -1;
    }

    int fd = open(GPIO_PATH "/unexport", O_WRONLY);
    if (fd >= 0) {
        write(fd, hx->gpio_dout, strlen(hx->gpio_dout));
        write(fd, hx->gpio_sck, strlen(hx->gpio_sck));
        close(fd);
    }
}

