from libs.PipeLine import PipeLine, ScopedTiming
from libs.YOLO import YOLOv5
import os, sys, gc, urandom
import ulab.numpy as np
import image
from media.sensor import *
from media.display import *
from media.media import *
from machine import UART
from machine import FPIOA
import time

turn = 0

def serial(num,middle_x,middle_y,turn):
    global uart
    str_num = str(num)
    num_map = {
        '0': 2,
        '1': 3,
        '2': 1,
        '3': 0
    }

    # 映射处理：如果 num 在映射表中，就替换；否则保持原值
    mapped_num = num_map.get(str_num, str_num)

    data = str('a')+' '+str(mapped_num)+' '+str(middle_x) +' '+ str(middle_y)+' '+str(turn)+' '+str('b')+str('\n')
    uart.write(data)
    print(f"mddule_num:{data}")



def modules(res,image):
        global turn
        roi_x,roi_y,roi_w,roi_h = 635,335,600,300
        print(res)
        for i in res[0]:
            x1,y1,w1,h1= i
            print(x1,y1,w1,h1)
            print(res[1][0])
            cx=int(w1/2)+x1
            cy=int(h1/2)+y1
            module_num=res[1][0]
            if roi_x<cx<roi_x+roi_w and roi_y<=cy<=roi_y+roi_h:
                if w1>h1:
                    h_left=int(w1/2)+x1
                    h_right=(y1+h1)
                    middle_x=int(w1/2)+x1
                    middle_y=int(h1/2)+y1
                    keypoints=[(h_left, y1),(h_left, h_right),(middle_x,middle_y)]
                    print(keypoints)
                    image.draw_circle(h_left,y1 ,5, color=(0, 0, 255), thickness=2)
                    image.draw_circle(h_left,h_right ,5, color=(0, 0, 255), thickness=2)
                    image.draw_cross(middle_x, middle_y, color=(255, 0, 0), size=10, thickness=1)
                    turn=0
                elif w1<h1:
                    h_left=int(h1/2)+y1
                    h_right = (x1+w1)
                    middle_x=int(w1/2)+x1
                    middle_y=int(h1/2)+y1
                    image.draw_circle(x1,h_left ,5, color=(0, 0, 255), thickness=2)
                    image.draw_circle(h_right,h_left ,5, color=(0, 0, 255), thickness=2)
                    image.draw_cross(middle_x, middle_y, color=(255, 0, 0), size=10, thickness=1)
                    turn=1
                serial(module_num,middle_x,middle_y,turn)
            else:
                uart.write("aendb")
def modules_name(res):
    if len(res[1])>0:
        for l in range(len(res[1])):
            print(res[1][l])

if __name__ == "__main__":

    m=0
    #串口初始化
    fpioa=FPIOA()

    fpioa.set_function(11,FPIOA.UART2_TXD)
    fpioa.set_function(12,FPIOA.UART2_RXD)
    uart=UART(UART.UART2,baudrate=115200,bits=UART.EIGHTBITS,parity=UART.PARITY_NONE,stop=UART.STOPBITS_ONE)    #波特率必须115200
    uart.init()

    # 配置参数
    display_mode = "hdmi"  # "hdmi" 或 "lcd"
    rgb888p_size = [1280, 720]
    display_size = [1920, 1080] if display_mode == "hdmi" else [800, 480]

    # 模型配置
    kmodel_path = "data/model/module2.kmodel"
    labels = ["STM32", "tube", "flame", "CT", "relay"]
    confidence_threshold = 0.4
    nms_threshold = 0.45
    model_input_size = [640, 640]

    # 初始化
    pl = PipeLine(
        rgb888p_size=rgb888p_size,
        display_size=display_size,
        display_mode=display_mode
    )
    pl.create()

    yolo = YOLOv5(
        task_type="detect",
        mode="video",
        kmodel_path=kmodel_path,
        labels=labels,
        rgb888p_size=rgb888p_size,
        model_input_size=model_input_size,
        display_size=display_size,
        conf_thresh=confidence_threshold,
        nms_thresh=nms_threshold,
        max_boxes_num=50,
        debug_mode=0
    )
    yolo.config_preprocess()
  # 绘制绿色矩形
    try:
        while True:
            read_data = uart.read()
            if read_data :
                uart.write(b"ok")
                img = pl.get_frame()
                yolo.run(img)
                if b'start' in read_data:
                    os.exitpoint()
                    with ScopedTiming("total", 1):
                    # 获取帧并推理
                    # 处理结果
                        for m in range(5):
                            res = yolo.run(img)
                            time.sleep(0.5)
                            print(m)
                            if len(res[0])>0:
                                print("判断长度")
                                yolo.draw_result(res, pl.osd_img)
                                time.sleep(0.5)
                                modules(res,pl.osd_img)
                                pl.show_image()
                                gc.collect()  # 内存回收
                                break;
                            elif m==4:
                                uart.write(b"aendb\n")
    except Exception as e:
        print("Exception occurred:", e)
    finally:
        yolo.deinit()
        pl.destroy()
