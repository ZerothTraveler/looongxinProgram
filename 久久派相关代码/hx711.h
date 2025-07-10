#ifndef __HX711_H
#define __HX711_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

#define GPIO_PATH "/sys/class/gpio"

typedef struct {
    const char* gpio_dout;
    int fd_dout;
    const char* gpio_sck;
    int fd_sck;
    unsigned long maopi;
    int GapValue;
} hx711_t;



int hx711_init(hx711_t* hx);
uint32_t hx711_read(hx711_t* hx);
int Get_Maopi(hx711_t* hx);
double Get_Weight(hx711_t* hx);
double weight_filter(hx711_t* hx);
void hx711_cleanup(hx711_t* hx);
#endif
