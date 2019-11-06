## Sendclient_by_mosq.py, Sendclient_by_paho.py 代码说明
这两个程序是发送数据到thngsboard的两种客户端。

1. 环境

```
# Sendclient_by_paho.py 使用如下库依赖
pip install paho-mqtt

# Sendclient_by_mosq.py 使用如下库依赖
sudo apt-get install mosquitto-clients
```

## Temperture_by_mosq.py, Temperture_by_paho.py 代码说明
这两个程序是对客户端的使用案例。
        
### 案例使用：

1. 查询温度传感器在Raspi上的地址

将查询到的设备地址替换到代码中的ds18b20变量

```
sudo modprobe w1-gpio
sudo modprobe w1-therm
cd /sys/bus/w1/devices/
ls
#在此查看设备名称，eg:28-XXXXXXXXX

```

> 注：以上代码都为python2.7环境下编写，在树莓派上使用，在终端使用python进行代码的运行。网络异常抛出:python2是IOError, python3是OSError。