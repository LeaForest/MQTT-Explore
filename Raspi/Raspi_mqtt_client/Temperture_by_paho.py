# coding=utf-8
import paho.mqtt.publish as publish
import json
import os

# Thingsboard主机地址
host = "192.168.3.216"

# 设备俄访问令牌
id = "DwhCq5vxuiglgjvLS4pp"


ds18b20 = '28-0318402e42ff'


def setup():
    global ds18b20
    for i in os.listdir('/sys/bus/w1/devices'):
        if i != 'w1_bus_master2':
            ds18b20 = i


def read():
    #	global ds18b20
    location = '/sys/bus/w1/devices/' + ds18b20 + '/w1_slave'
    tfile = open(location)
    text = tfile.read()
    tfile.close()
    secondline = text.split("\n")[1]
    temperaturedata = secondline.split(" ")[9]
    temperature = float(temperaturedata[2:])
    temperature = temperature / 1000
    return temperature


def loop():
    while True:
        if read() != None:
            print "Current temperature : %0.3f C" % read()
            tem = round(read(), 2)
        status = {"temperature": tem}
        status = json.dumps(status)
        try:
            publish.single("v1/devices/me/telemetry", status, 0, False, host, 1883, id, auth={"username": id, "password": ''})
        except IOError:
            print "Network error,please wait"
def destroy():
    pass


if __name__ == '__main__':
    try:
        # setup()
        loop()
    except KeyboardInterrupt:
        destroy()
