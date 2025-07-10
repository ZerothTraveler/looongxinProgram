#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "camera.h"

int camera_open=0;                     
pthread_mutex_t camera_lock= PTHREAD_MUTEX_INITIALIZER;

void *camera_use(void *arg)
{
    int last_state = -1;
    while (1) {
        pthread_mutex_lock(&camera_lock);
        int local_camera_open = camera_open;
        pthread_mutex_unlock(&camera_lock);

        if (local_camera_open != last_state) {
            last_state = local_camera_open;

            char cmd[512];
            if (local_camera_open == 0) {
                snprintf(cmd, sizeof(cmd),
                    "ffmpeg -y -f lavfi -i color=black:size=800x480:rate=1 -vframes 1 -pix_fmt bgra -f fbdev /dev/fb0");
            } else {
                snprintf(cmd, sizeof(cmd),
                    "ffmpeg -y -f v4l2 -input_format yuyv422 -video_size 800x480 -i /dev/video0 -vf vflip -pix_fmt bgra -f fbdev /dev/fb0");
            }

            printf("执行命令: %s\n", cmd);
            system(cmd);
        }

        usleep(100 * 1000);  // 100ms 检查一次状态
    }
    return NULL;
}
