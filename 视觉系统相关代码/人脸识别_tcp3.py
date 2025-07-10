# -----------------------------------------------
#  Face-Detection 主流程（MaixPy, K210/Maix-II）
# -----------------------------------------------
from maix import network,err,nn, camera, display, image
import socket
import os
import math
import time

# === 全局状态 ===
name             = None            # 当前正在录入的人名
last_send_time   = {}              # 记录上次发送时间 {label: t}
temp_faces       = []              # 录入阶段临时缓存
camera_mode      = False           # 是否开启识别
recording        = False           # 是否处于录入模式
deleting         = False           # 是否处于删除模式
num              = 0               # 录入张数计数
last_faces       = []              # 最近一次识别到的人脸列表
client_socket    = None            # 在 tcp_client() 里初始化
connected_time  = 0 
camera_rog_mode = False
#ip = "192.168.191.54"
#ip = "192.168.183.54"
ip = "192.168.4.1"
port = 1001


def tcp_connect():
    w = network.wifi.Wifi()
    print("ip:",w.get_ip())

    SSID = "Redmi K40"
    PASSWORD = "dgeUEHR12"
    #SSID = "REDMI K80 Pro"
    #PASSWORD = "7TANAf46fb"


    print("connect to",SSID)

    e = w.connect(SSID,PASSWORD,wait=True,timeout = 360)
    err.check_raise(e , "connect wifi failed")
    print("ip",w.get_ip())

def tcp_client(ip,port):
    global client_socket 
    while True:
        try:
            print(f"尝试连接服务器 {ip}:{port} ...")
            client_socket = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
            server_address = (ip,port)
            client_socket.connect(server_address)
            print("已成功连接到服务器！")
            break
        except Exception as e:
            print(f"连接失败：{e},3秒后重试...")
            time.sleep(3) 

def load_all_faces(recognizer):
    face_dir = "/root/person"
    if not os.path.exists(face_dir):
        os.makedirs(face_dir)
    for fname in os.listdir(face_dir):
        if fname.endswith(".bin"):
            path = os.path.join(face_dir, fname)
            print(f"加载人脸库: {path}")
            recognizer.load_faces(path)
        else:
            print("没有人脸库")



# ------------------------------------------------
def handle_command(cmd):
    "根据收到的指令切换状态 / 执行动作"
    global camera_mode, recording, deleting, name, num, temp_faces,client_socket
    global camera_rog_mode
    if cmd == "camera_start":
        camera_mode = True
        print(">>> 识别已开启")
    elif cmd == "camera_stop":
        camera_mode = False
        print(">>> 识别已关闭")

    # ============ 录入流程 ============
    elif cmd == "record":
        recording  = True
        name       = None
        temp_faces = []
        print(">>> 进入录入模式，请发送人名")

    elif recording and name is None and cmd not in ["true", "cancel", "finish", "None"]and not cmd.startswith("AT+"):
        name = cmd
        print(f"人名已设置为：{name}，对准摄像头后发送 'true' 拍照")

    elif cmd == "true":
        if recording and name and last_faces:
            # 取最大的人脸对象
            obj = max(last_faces, key=lambda o: o.w * o.h)
            recognizer.add_face(obj, name)
            temp_faces.append(obj)
            num += 1
            print(f"录入成功：{name}{num}  (共 {len(temp_faces)} 张)")
            client_socket.sendall("record success".encode())
        else:
            print("未检测到人脸，无法录入")

    elif cmd == "cancel":
        recording  = False
        name       = None
        temp_faces = []
        print("录入已取消")
        client_socket.sendall("cancel success".encode())

    elif cmd == "finish":
        if recording and name and temp_faces:
            save_path = f"/root/person/{name}.bin"
            if os.path.exists(save_path):
                recognizer.load_faces(save_path)             
            recognizer.save_faces(save_path)
            print(f"人脸库已保存：{save_path}")
            client_socket.sendall("save success".encode())
        recording  = False
        name       = None
        temp_faces = []
        num        = 1
    elif cmd == "camera_log":           #开启验证
        camera_rog_mode = True
        print(">>> 开启上报识别结果（camera_rog_mode = True）")
    elif cmd == "camera_log_ok":         #关闭验证
        camera_rog_mode = False
        print(">>> 开启上报识别结果（camera_rog_mode = False")
    # ============ 删除人脸 ============
    elif cmd == "del":
        deleting = True
        print(">>> 进入删除模式，请发送要删除的人名")

    elif deleting and cmd not in ["true", "cancel", "finish", "None"]:
        del_name = cmd
        del_path = f"/root/person/{del_name}.bin"
        if os.path.exists(del_path):
            os.remove(del_path)
            print(f"已删除人脸库文件：{del_path}")
        else:
            print(f"文件不存在：{del_path}")
        deleting = False

    # ============ 设备自报 ID ============
    elif cmd == "ID":
        try:
            client_socket.sendall("vision\r\n".encode())
            print("发送成功")
        except Exception as e:
            print("发送失败(ID):", e)
            client_socket.close()
            client_socket = None
    else:
        # 忽略未知或空指令
        pass
# ------------------------------------------------

def face_detection():
    global recognizer, last_faces
    global client_socket
    global camera_rog_mode

    # ---------- 初始化模型与硬件 ----------
    recognizer = nn.FaceRecognizer(
        detect_model   = "/root/models/retinaface.mud",
        feature_model  = "/root/models/face_feature.mud",
        dual_buff      = True
    )
    load_all_faces(recognizer)

    cam  = camera.Camera(recognizer.input_width(),
                         recognizer.input_height(),
                         recognizer.input_format())
    disp = display.Display()
    send_count = {}
    # ---------- 主循环 ----------
    while True:
        
        if client_socket is None:
            print(">>> 尝试重新连接服务器...")
            try:
                tcp_client(ip, port)
            except Exception as e:
                print("重连失败：", e)
                time.sleep(3)
                continue

        img = cam.read()

        # 1) ===== 接收并处理 TCP 指令 (不阻塞) =====
        try:
            client_socket.settimeout(0.01)          # 10 ms
            data = client_socket.recv(1024)
            if data:
                cmd = data.decode().strip()
                handle_command(cmd)
        except socket.timeout:
            pass
        except Exception as e:
            print("TCP 异常：", e)

        # 2) ===== 只有在 camera_mode 下才做识别 =====
        last_faces = []
        if camera_mode:
            try:
                last_faces = recognizer.recognize(img, 0.5, 0.45, 0.8, recording)
            except Exception as e:
                print("识别异常：", e)
                last_faces = []

            # --- 绘制框 & 发送识别结果 ---
            for obj in last_faces:
                # 画框
                img.draw_rect(obj.x, obj.y, obj.w, obj.h, color=image.COLOR_RED)
                label = recognizer.labels[obj.class_id]
                msg   = f"{label}:{obj.score:.2f}"
                img.draw_string(obj.x, obj.y, msg, color=image.COLOR_RED)

                # 非录入状态下 → 发送识别到的名字
                if (not recording) and label != "unknown"and camera_rog_mode:
                    now = time.time()
                    if label not in last_send_time or now - last_send_time[label] > 2:
                        try:
                            if send_count.get(label, 0) < 2:
                                client_socket.sendall((label).encode())
                                last_send_time[label] = now
                                print("已发送：", label)
                                send_count[label] = send_count.get(label, 0) + 1
                                camera_rog_mode=False
                        except Exception as e:
                            print("发送失败：", e)

        # 3) ===== 显示画面 =====
        disp.show(img)

# ------------------------------------------------
#  程序入口（保持不变）
# ------------------------------------------------
if __name__ == "__main__":
    tcp_connect()
    tcp_client(ip, port)
    face_detection()
