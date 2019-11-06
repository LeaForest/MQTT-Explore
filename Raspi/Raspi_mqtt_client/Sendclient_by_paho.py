# coding=utf-8
import paho.mqtt.publish as publish
import json

# Thingsboard主机地址
host = "192.168.3.216"

# 设备访问令牌
id = " "

# 主题
topic = ""

user ={"username": id, "password": ''}

# 消息体
paylod = ""

status = json.dumps(paylod)
try:
    publish.single(topic, status, 0, False, host, 1883, id, auth=user)
except IOError:
    print "network error,please wait"