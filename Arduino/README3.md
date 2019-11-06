# 1.项目说明：
&nbsp; &nbsp; &nbsp; &nbsp;本项目采用了UNO开发板进行传感器数据的采集，并直接对ESP8266-01WiFi芯片进行烧写使其作为MQTT客户端控制器，从而实现在缺少Wifi开发板的情况下或扩展板的情况下也能进行MQTT协议下传感数据的发布与订阅。两者之间使用串口通信，基于主从机的关系，实现数据从采集处理到发布的目的。相较于2.1版本，解决了不稳定的网络环境下容易与MQTT代理和局域网断开连接的问题，对网络的适应性更强，数据传输与发布更加稳定，同时也实现了UNO与ESP8266的主从机式分工，两者之间通过串口进行的数据通信。

# 2.文件说明:
&nbsp;&nbsp;&nbsp;&nbsp;项目文件主要包含了库文件、ESP8266部分和UNO部分：
1. 库文件为"ESP8266-master"(ESP8266网络配置库文件)、"OneWire-master"(温度传感器库文件)和"pubsubclient-master"(MQTT协议下消息发布订阅库)；
2. ESP8266部分包含了代码ino文件、ESP8266-01引脚图jpg文件、烧写时进行线路连接的png连接图和电路fzz文件；
3. UNO部分则包含了主代码ino、连接图png和电路fzz文件；

# 3.项目准备：
1. 硬件材料：Arduino UNO、ESP8266-01、面包板、TTL-USB串口模块、DSP18B20温度传感器、杜邦线若干；
2. 软件材料：Arduino IDE、Fritzing、MQTTBox（MQTT客户端测试工具）、任意MQTT代理服务器或物联网平台（如EMQ服务搭建请参见:http://www.emqtt.com/docs/v2/install.html）；
3. 注意：本项目需要对ESP8266进行代码编写，除了引用文件包含的头文件外，还需引入"Generic ESP8266 Module"第三方开发板库，具体安装方式请参见：<https://blog.csdn.net/u013810296/article/details/54862447>,或自行Baidu;

# 4.项目过程：
1. 首先根据以下图片进行连接：<br></br>
 TX(ESP)-RX(TTL);&nbsp; &nbsp; &nbsp;  RX(ESP)-TX(TTL);&nbsp; &nbsp; &nbsp;  PD(ESP)-GND(TTL); &nbsp; &nbsp; &nbsp;  VCC(ESP)-3.3V(TTL);&nbsp; &nbsp; &nbsp;  GND(ESP)-GND(TTL);&nbsp; &nbsp; &nbsp; GPIO0(ESP)-GND(TTL); <br></br>
![img] (V3.1_ESP8266-Code/V3.1_ESP8266.png)
2. 确定连接正确后，打开IDE和代码并选择TTL-USB串口，再选择：工具->开发板->Generic ESP8266 Module，底下的设置为默认即可;(请安装第三方开发板库文件后再进行此步骤)
3. 开始编译，编译正确后开始进行烧写，烧写完成后，将GPIO0口置空；（注意：烧写时若报错，则请将串口重新插拔即可）
4. 再根据以下方式进行与UNO的连接： <br></br>
  A（DSP）-D2(UNO);&nbsp;&nbsp;&nbsp;VCC(DSP)-3.3V(UNO);&nbsp; &nbsp; &nbsp;GND(DSP)-GND(UNO);&nbsp; &nbsp; &nbsp;RX(ESP)-TX(UNO);&nbsp; &nbsp; &nbsp;TX(ESP)-RX(UNO)&nbsp; &nbsp; &nbsp;PD(ESP)-3.3V(UNO);&nbsp; &nbsp; &nbsp;VCC(ESP)-3.3V(UNO);&nbsp; &nbsp; &nbsp;GND(ESP)-GND(UNO);
![img] (V3.1-UNO-Code/V3.1_UNO.png)
5. 打开IDE重新选择正确的串口后，再烧入代码；（注意：在烧入代码前，必须将硬串口的RX、TX引脚断开，烧入完成后再接回引脚）
6. 使用MQTTBox进行消息订阅或连接ThingsBoard；(具体连接参数设置请参见代码)