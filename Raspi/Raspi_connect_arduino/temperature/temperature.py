# coding=utf-8
import serial
import json
import time
import paho.mqtt.publish as publish


host = "139.224.9.212"
# 访问令牌
id = " "
topic = "v1/devices/me/telemetry"
user ={"username": id, "password": ''}

# 串口地址
serial_address = '/dev/ttyACM0' 

# 创建串口实例 
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
if not ser.isOpen():
    ser.open()

# 获取温度值
def temperature():

    response = ser.readline()
    re = response[:-2].decode("utf-8")
    if re == "":
        re = -1000
    time.sleep(0.5)
    return re

# 处理15秒收到的数值
def date():
    print()
    tem = []
    T1 = time.time()
    while True:
        a = float(temperature())
        if not a == -1000 or "":
            tem.append(a)
        T2 = time.time()
        if T2 - T1 > 15:
            break
    print (tem)
    Tem_avg = sum(tem) / len(tem)
    tem.clear()
    return float('%.2f' % Tem_avg)

# 发送数据
def send_date():
    Paylod = {"Temperature": date()}
    status = json.dumps(Paylod)
    publish.single(topic, status, 0, False, host, 1883, id, auth=user)

if __name__ == "__main__":
    try:
        while 1:
            try:
                t = date()
                print("15秒的平均数值： " + str(t))
            except OSError:
                print("Network error,please wait!")
    except KeyboardInterrupt:
        ser.close()
