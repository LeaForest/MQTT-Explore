# 基于树莓派与Arduino的主从机数据交互
树莓派作为主机，Arduino作为从机，将采集的模拟数据通过usb串口发送到主机，主机将数据处理后发送到服务器。例如：Thingsboard，kafka。

## 材料

1. 树莓派一个
2. Arduino一个
3. Arduino USB数据线一个

## Arduino所需操作

通过Arduino IDE将代码Demo_A.ino烧进arduino中，烧写前记得选号端口。将Arduino通过usb连到树莓派。

## 树莓派所需操作

1. 树莓派库依赖：

```
sudo apt-get install python-serial

```

2. 查找串口地址：

```
ls /dev/ttyACM*

```

>注：如果没有,说明usb串口没有识别,重新拔插一次。

将查到的地址替换到代码Demo_pi.py中的serial_address，将该代码烧到树莓派中。

## 执行
 运行树莓派里的Demo_pi.py文件。显示“Hi pi,i am Arduino”,说明你成功啦！
 

## 延伸

 CO2， temperatrue两个文件里的内容是对该方式的案例使用，如有其他传感器数据可以参考其代码改造。