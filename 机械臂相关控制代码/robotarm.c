#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>

//毫秒延时函数
void sleep_ms(int time)
{
    usleep(time * 1000);
}
/*********************************串口函数start********************************************/

// 串口文件描述符
int fd_ttyS1 = -1;
int fd_ttyS2 = -1;

// 初始化串口函数
int init_serial(const char *device, int baud_rate) {
    int fd = open(device, O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror("Error opening serial port");
        return -1;
    }

    // 获取当前串口设置
    struct termios options;
    tcgetattr(fd, &options);

    // 设置波特率
    speed_t speed;
    switch (baud_rate) {
        case 9600:   speed = B9600;   break;
        case 19200:  speed = B19200;  break;
        case 38400:  speed = B38400;  break;
        case 57600:  speed = B57600;  break;
        case 115200: speed = B115200; break;
        default:    speed = B9600;   break; // 默认9600
    }
    cfsetispeed(&options, speed);
    cfsetospeed(&options, speed);

    // 设置串口参数
    options.c_cflag |= (CLOCAL | CREAD); // 本地连接，启用接收
    options.c_cflag &= ~PARENB;          // 无奇偶校验
    options.c_cflag &= ~CSTOPB;          // 1位停止位
    options.c_cflag &= ~CSIZE;           // 清除数据位掩码
    options.c_cflag |= CS8;              // 8位数据位

    // 原始输入模式
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    // 原始输出模式
    options.c_oflag &= ~OPOST;

    // 设置超时：阻塞读取，直到满足最小字符数
    options.c_cc[VMIN]  = 1;   // 最小读取字符数
    options.c_cc[VTIME] = 0;   // 无限等待

    // 应用设置
    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd, TCSANOW, &options) != 0) {
        perror("Error configuring serial port");
        close(fd);
        return -1;
    }

    return fd;
}

// ttyS1发送字符串
void ttyS1_send(const char *data) {
    if (fd_ttyS1 < 0) {
        fprintf(stderr, "ttyS1 not initialized!\n");
        return;
    }
    ssize_t len = strlen(data);
    ssize_t n = write(fd_ttyS1, data, len);
    if (n < len) {
        perror("ttyS1 write incomplete");
    }
}

// ttyS2发送字符串
void ttyS2_send(const char *data) {
    if (fd_ttyS2 < 0) {
        fprintf(stderr, "ttyS2 not initialized!\n");
        return;
    }
    ssize_t len = strlen(data);
    ssize_t n = write(fd_ttyS2, data, len);
    if (n < len) {
        perror("ttyS2 write incomplete");
    }
}

// ttyS2接收数据包（阻塞式，包头'a'，包尾'b'）
char *ttyS2_receive_packet() {
    if (fd_ttyS2 < 0) {
        fprintf(stderr, "ttyS2 not initialized!\n");
        return NULL;
    }

    char buffer[1024];
    int idx = 0;
    char ch;
    int state = 0; // 0:等待包头 1:接收数据 2:完成

    while (state != 2) {
        ssize_t n = read(fd_ttyS2, &ch, 1);
        if (n <= 0) {
            perror("Error reading from ttyS2");
            return NULL;
        }

        switch (state) {
            case 0: // 等待包头
                if (ch == 'a') {
                    state = 1; // 进入数据接收状态
                }
                break;
                
            case 1: // 接收数据
                if (ch == 'b') {
                    state = 2; // 包结束
                } else {
                    if (idx < sizeof(buffer) - 1) {
                        buffer[idx++] = ch;
                    } else {
                        fprintf(stderr, "Packet too long! Truncated.\n");
                        state = 2;
                    }
                }
                break;
        }
    }

    buffer[idx] = '\0'; // 字符串结束符
    return strdup(buffer); // 返回动态分配的字符串副本
}

/*********************************串口函数end********************************************/

/*********************************压力传感器GPIOstart************************************/

#define GRAB_GPIO_PIN  88 //定义压力传感器引脚


// 导出 GPIO（使其出现在 /sys/class/gpio/ 目录下）
int gpio_export(int pin) {
    int fd;
    char buf[64];

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0) {
        perror("无法打开 export 文件");
        return -1;
    }

    snprintf(buf, sizeof(buf), "%d", pin);
    write(fd, buf, strlen(buf));
    close(fd);

    return 0;
}

// 设置 GPIO 方向为输入
int gpio_set_direction_input(int pin) {
    int fd;
    char buf[64];

    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", pin);

    fd = open(buf, O_WRONLY);
    if (fd < 0) {
        perror("can't open direction file");
        return -1;
    }

    write(fd, "in", 2);
    close(fd);

    return 0;
}

// 读取 GPIO 的值（返回 0 或 1）
int gpio_read(int pin) {
    int fd;
    char buf[64];
    char value;

    snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", pin);

    fd = open(buf, O_RDONLY);
    if (fd < 0) {
        perror("无法打开 value 文件");
        return -1;
    }

    if (read(fd, &value, 1) != 1) {
        perror("读取 value 文件失败");
        close(fd);
        return -1;
    }

    close(fd);

    return (value == '1') ? 1 : 0;
}

/*********************************压力传感器GPIOend**************************************/
/*********************************机械臂控制函数start************************************/

//机械臂开机的初始化
void robotarm_Init()
{
    ttyS1_send("{G0001#000P1500T1500!#001P1538T1500!#002P1528T1500!#003P1065T1500!#004P1500T1500!#005P1500T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0002#000P1500T1500!#001P1743T1500!#002P2234T1500!#003P0841T1500!#004P1500T1500!#005P1500T1500!}");
    sleep_ms(1500);
}

//返回到等待下一个物体位置的函数
void rewait_action()
{
    ttyS1_send("{G0006#000P1480T1500!#001P1410T1500!#002P1980T1500!#003P2315T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0005#000P1451T1500!#001P1804T1500!#002P1980T1500!#003P1708T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0004#000P1451T1500!#001P1804T1500!#002P1980T1500!#003P0792T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0003#000P1451T1500!#001P1420T1500!#002P1980T1500!#003P0792T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0002#000P1500T1500!#001P1743T1500!#002P2234T1500!#003P0841T1500!#004P1500T1500!#005P1500T1500!}");
    sleep_ms(1500);
}

//机械爪放置函数
void place_object()
{
    ttyS1_send("#005P1300T0600!");
    sleep(2);
    ttyS1_send("#005P1600T1000!");
    sleep(1);
}

//将物块放置到0号格子
void move_action_0()
{
    ttyS1_send("{G0011#000P1235T1500!#001P1013T1500!#002P1191T1500!#003P2092T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0012#000P1235T1500!#001P1461T1500!#002P1359T1500!#003P1839T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0013#000P1344T1500!#001P1536T1500!#002P1370T1500!#003P1766T1500!}");
    sleep_ms(1500);
    place_object(); //放置物体
    ttyS1_send("{G0012#000P1235T1500!#001P1461T1500!#002P1359T1500!#003P1839T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0011#000P1235T1500!#001P1013T1500!#002P1191T1500!#003P2092T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0007#000P1524T1500!#001P1028T1500!#002P1191T1500!#003P2315T1500!}");
    sleep_ms(1500);
    rewait_action();
}

//将物块放置到1号格子
void move_action_1()
{
    ttyS1_send("{G0008#000P1751T1500!#001P1110T1500!#002P1271T1500!#003P2164T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0009#000P1751T1500!#001P1373T1500!#002P1271T1500!#003P1863T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0010#000P1701T1500!#001P1478T1500!#002P1271T1500!#003P1720T1500!}");
    sleep_ms(1500);
    place_object(); //放置物体
    ttyS1_send("{G0009#000P1751T1500!#001P1373T1500!#002P1271T1500!#003P1863T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0008#000P1751T1500!#001P1110T1500!#002P1271T1500!#003P2164T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0007#000P1524T1500!#001P1028T1500!#002P1191T1500!#003P2315T1500!}");
    sleep_ms(1500);
    rewait_action();
}

//将物块放置到2号格子
void move_action_2()
{
    ttyS1_send("{G0014#000P1298T1500!#001P0803T1500!#002P0902T1500!#003P2200T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0015#000P1351T1500!#001P1262T1500!#002P0620T1500!#003P1588T1500!}");
    sleep_ms(1500);
    place_object(); //放置物体
    ttyS1_send("{G0014#000P1298T1500!#001P0803T1500!#002P0902T1500!#003P2200T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0007#000P1524T1500!#001P1028T1500!#002P1191T1500!#003P2315T1500!}");
    sleep_ms(1500);
    rewait_action();
}

//将物块放置到3号格子
void move_action_3()
{
    ttyS1_send("{G0016#000P1749T1500!#001P0905T1500!#002P0899T1500!#003P2178T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0017#000P1749T1500!#001P1262T1500!#002P0699T1500!#003P1656T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0018#000P1646T1500!#001P1262T1500!#002P0699T1500!#003P1656T1500!}");
    sleep_ms(1500);
    place_object(); //放置物体
    ttyS1_send("{G0017#000P1749T1500!#001P1262T1500!#002P0699T1500!#003P1656T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0016#000P1749T1500!#001P0905T1500!#002P0899T1500!#003P2178T1500!}");
    sleep_ms(1500);
    ttyS1_send("{G0007#000P1524T1500!#001P1028T1500!#002P1191T1500!#003P2315T1500!}");
    sleep_ms(1500);
    rewait_action();
}

//机械抓抓取物体的函数
void grab_object()
{
    int force = 1300; //爪子初始力
    char command[64];
    int grab_flag = 1; //用于记录是否抓到物体
    printf("开始抓取\n");
    while(grab_flag == 1){
        grab_flag = gpio_read(GRAB_GPIO_PIN);
        force = force + 30;
        if(force > 1700){
            force = 1700;
        }
        snprintf(command, sizeof(command),"#005P%dT0600!", force);
        ttyS1_send(command);
        sleep_ms(600);
    }
    printf("已抓取到物体\n");
}

//摄像头到机械臂的坐标换算
typedef struct {
    int x;
    int y;
} Point;

Point camera_transform(float X, float Y)
{
    Point result;
    result.x = (int)((-0.2167 * X) + 217.583); // y = -13/60x + 2605/12
    result.y = (int)(0.1667*Y + 64.1667); // y = 1/6x + 64.1667
    return result;
}

//处理摄像头传来的指令字符串的函数
int parse_three_ints(char *str, int out[4]) {
    return sscanf(str, "%d %d %d %d", &out[0], &out[1], &out[2],&out[3]) == 4;
}

//机械臂追踪对应点模块的函数
void tarce_object()
{
    int commodand[3] = {0,0,0};
    ttyS2_send("start");
    printf("等待camera传来数据\n");
    char *packet = ttyS2_receive_packet();
    printf("-->接收到camera传来的未处理数据是%s<--\n",packet);
    printf("比较字符串的结果是：%d\n",strcmp(packet, "end"));
    if(strcmp(packet, "end") == 0){//检测到传来的是结束命令
        printf("已全部抓取完成\n");
        exit(0);
    }else{//检测传来的是其他命令
        printf("camera传来的数据是：%s\n",packet);
        if(parse_three_ints(packet,commodand)){
            printf("命令字符串处理错误\n");
        }
        for(int i = 0;i < 4;i ++){
            printf("得到的命令[%d],内容是：%d\n",i,commodand[i]);
        }

        Point P = camera_transform((float)commodand[1],(float)commodand[2]);

        printf("得到的命令X坐标是：%d,Y坐标是%d\n",P.x,P.y);
        char location_command[128];

        if(commodand[3] == 0){
            ttyS1_send("#004P0800T0800!");//将机械爪角度摆好
            sleep_ms(800);
        }
        ttyS1_send("#005P1300T0600!");//将机械爪宽度摆好
        sleep_ms(600);
        snprintf(location_command, sizeof(location_command),"$KMS:%d,%d,50,1500!", P.x,P.y);
        printf("开始移动置预抓取位置\n");
        ttyS1_send(location_command);
        sleep_ms(1500);
        snprintf(location_command, sizeof(location_command),"$KMS:%d,%d,-8,1500!", P.x,P.y);
        printf("开始移动置抓取位置\n");
        ttyS1_send(location_command);
        sleep_ms(1500);
        grab_object();
        snprintf(location_command, sizeof(location_command),"$KMS:%d,%d,50,1500!", P.x,P.y);
        printf("开始抬机械臂\n");
        ttyS1_send(location_command);
        sleep_ms(1500);
        if(commodand[3] == 0){
            ttyS1_send("#004P1500T0800!");//将机械爪角度摆好
            sleep_ms(800);
        }
        printf("开始转换到预放置格子姿态\n");
        ttyS1_send("{G0003#000P1451T1500!#001P1420T1500!#002P1980T1500!#003P0792T1500!}");
        sleep_ms(1500);
        ttyS1_send("{G0004#000P1451T1500!#001P1804T1500!#002P1980T1500!#003P0792T1500!}");
        sleep_ms(1500);
        ttyS1_send("{G0005#000P1451T1500!#001P1804T1500!#002P1980T1500!#003P1708T1500!}");
        sleep_ms(1500);
        ttyS1_send("{G0006#000P1480T1500!#001P1410T1500!#002P1980T1500!#003P2315T1500!}");
        sleep_ms(1500);
        ttyS1_send("{G0007#000P1524T1500!#001P1028T1500!#002P1191T1500!#003P2315T1500!}");
        sleep_ms(1500);
        switch(commodand[0])
        {
            case 0:move_action_0();break;

            case 1:move_action_1();break;

            case 2:move_action_2();break;

            case 3:move_action_3();break;
            default:
                printf("物块格子位置信息获取错误");break;
        }  
        
    }
}
/*********************************机械臂控制函数end************************************/
int main() {
    // 初始化串口 (115200波特率)
    printf("开始运行\n");
    fd_ttyS1 = init_serial("/dev/ttyS1", 115200);
    fd_ttyS2 = init_serial("/dev/ttyS2", 115200);
    printf("串口初始化完毕\n");

    if (fd_ttyS1 < 0 || fd_ttyS2 < 0) {
        fprintf(stderr, "Failed to initialize serial ports\n");
        return EXIT_FAILURE;
    }

    // 压力传感器GPIO初始化
    gpio_export(GRAB_GPIO_PIN);
    printf("导出压力传感器GPIO\n");
    gpio_set_direction_input(GRAB_GPIO_PIN);
    printf("将压力传感器GPIO调节为输入模式\n");

    
    robotarm_Init();
    while (1) {
        printf("运行函数\n");
        tarce_object();
    }
    
    // 关闭串口
    close(fd_ttyS1);
    close(fd_ttyS2);
    
    return EXIT_SUCCESS;
    
}

    /*ttyS1_send("Hello from ttyS1!\n");
    ttyS2_send("Hello from ttyS2!\n");*/

    /*printf("Waiting for packet on ttyS2...\n");
    char *packet = ttyS2_receive_packet();
    if (packet) {
        printf("Received packet: %s\n", packet);
        free(packet); // 释放动态分配的内存
    }*/