#! -*- coding: utf-8 -*-
import os
import json
# thingsboard ip
host = "192.168.3.216"

# 发布的主题
topic = "v1/devices/me/telemetry"

# 访问令牌
username = "sXrZXR3FtDdpVKCvTPTN"

# 发送的消息内容
date = {"temperature": 98, "humidity": 12.0}

date = json.dumps(date)
try:
    os.system(r'mosquitto_pub -d -h "'+host+'" -t "'+topic+'" -u "'+username+'" -m "'+date+'"')
except IOError:
    print "network error"
