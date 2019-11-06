# coding=utf-8
import serial
import json
import time
import paho.mqtt.publish as publish


host = "139.224.9.212"
# 访问令牌
id = "vyNZqQ3qZf5dyB5lqiDK"
topic = "v1/devices/me/telemetry"
user = {"username": id, "password": ''}

# 串口地址
serial_address = '/dev/ttyACM0' 

#创建串口实例
ser = serial.Serial(serial_address, 9600, timeout=1)
if not ser.isOpen():
    ser.open()

# 获取CO2值
def co2():
    response = ser.readline()
    re = response[:-2].decode("utf-8")
    time.sleep(0.5)
    return re


def send_date():
    co = co2()
    Paylod = {"CO2": co}
    status = json.dumps(Paylod)
    publish.single(topic, status, 0, False, host, 1883, id, auth=user)

if __name__ == "__main__":
    try:
        while 1:
            try:
                send_date()
            except OSError:
                print("network error")
    except KeyboardInterrupt:
        ser.close()
