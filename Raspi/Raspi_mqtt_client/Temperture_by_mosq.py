#! -*- coding: utf-8 -*-
import os
import json

# thingsboard ip
host = "192.168.3.216"

topic = "v1/devices/me/telemetry"

# 设备访问令牌
username = "DwhCq5vxuiglgjvLS4pp"

#温度传感器地址
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
                        tem = round(read(),2)
                date = {"temperature": tem}
                date = json.dumps(date)
                os.system(r'mosquitto_pub -d -h "'+host+'" -t "'+topic+'" -u "'+username+'" -m "'+date+'"')
                

def destroy():
	pass

if __name__ == '__main__':
	try:
		#setup()
		loop()
	except KeyboardInterrupt:
		destroy()
