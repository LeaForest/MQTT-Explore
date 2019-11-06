# coding=utf-8
import serial
import time
# 串口地址
serial_address = '/dev/ttyACM0'

# 创建串口实例
ser = serial.Serial(serial_address, 9600, timeout=1)

# 判断串口是否打开
if not ser.isOpen():
    ser.open()

try:
    while 1:
        # 读取串口一行的数据 注：读出的数据是byte类型
        response = ser.readline()
        # 串口读取需要0.5秒延迟
        time.sleep(0.5)
        print(response)
except KeyboardInterrupt:
    ser.close()