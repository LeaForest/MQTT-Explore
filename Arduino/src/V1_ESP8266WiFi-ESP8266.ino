/****************************************************************************************************************/
/********************************************====ESP CODE====****************************************************/
/****************************************************************************************************************/
/*
 * 1:线路连接：
 * RX(8266)-TX(TTL);
 * TX(8266)-RX(TTL);
 * GPIO0(8266)-GND(UNO);
 * VCC(8266)-3.3V(UNO);
 * PD_(8266)-3.3v(UNO);
 * GPIO2(8266)-S(DHT11);
 * GND(DHT11)-GND(UNO);
 * VCC(DHT11)-3.3V(UNO);
 * 2：Arduino IDE选择TTL端口和“ESP8266 Module”开发板 
 * 3：重新上电（UNO重新上电）,选择TTL端口进行烧写；
 * 4：本代码，主要利用8266作控制器，采集DHT11温湿度数据，进行MQTT协议下消息的订阅和发布；
 */
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DHTPIN 2                               //定义DHT11的信号引脚为ESP8266的GPIO2口
#define DHTTYPE DHT11
const char* ssid = "shuge-Xiaomi_2.4G";        //定义存储连接的局域网SSID的字符常量（自行更改）
const char* password = "yuhan123456";          //定义存储局域网密码的字符常量（自行更改）
const char* mqtt_server = "192.168.3.212";     //定义所连接的MQTT代理ID的字符常量（自行更改）

WiFiClient espClient;                         //初始化以太网客户端对象 -- WiFiEspClient.h
PubSubClient client(espClient);               //初始化MQTT库PubSubClient.h的对象，定义PubSubClient的实例
DHT dht(DHTPIN, DHTTYPE);                       //初始化DHT11传感器
long lastMsg = 0;                                //记录上一次发送信息的时长 
char msg[50];                                    //定义一个50字节长度的数组空间存储发布数据
int num = 0;                                     //定义计数变量并初始化

void setup_wifi() {                              //WIFI连接函数
      delay(10);
      /*Serial.print("Connecting to ");          //串口显示所连接的WIFISSID
      //Serial.println(ssid);*/
      WiFi.begin(ssid, password);                //进行连接
      while (WiFi.status() != WL_CONNECTED) {    //串口显示状态，直至连接至网络
           delay(500);
           Serial.print(".");
           }
      randomSeed(micros());                       //micros()函数返回以微秒表示的时间并作为随机数
      /*Serial.println("");              
      Serial.println("WiFi connected");           //串口打印显示已连接
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());*/           //调用WiFi.localIP()函数，串口打印获取的IP
    }



void callback(char* topic, byte* payload, unsigned int length) {//消息订阅的回调函数
      Serial.print("Sub Message:");                            //串口显示订阅的主题
      /*Serial.print(topic);*/
      for (int i = 0; i < length; i++) {                        //自适应字串长度
          Serial.print((char)payload[i]);                      //串口打印出接收到的数据
          }
      Serial.println();
      if ((char)payload[0] == '1') {                            //如果收到的数据是1
          digitalWrite(BUILTIN_LED, LOW);                       //8266指示灯低电平
      } else {                                                  //否则高电平
          digitalWrite(BUILTIN_LED, HIGH); 
      }
}



void reconnect() {                                                //MQTT服务重连函数
     while (!client.connected()) {                                //当未连接到MQTT服务时
           /* Serial.print("Attempting MQTT connection...");*/                                                 
           String clientId = "ESP8266-1";                          //设置客户端ID（自行更改）
           clientId += String(random(0xffff), HEX);                // 生成10进制的随机数作为客户端ID后缀
           if (client.connect(clientId.c_str(),"admin","public")) { //当客户端连至当前指定的代理时
               /*Serial.println("connected");*/                     //串口显示已连接
               client.subscribe("mySub1");                          //客户端订阅当前主题的消息（自行更改）
               delay(20);
           } else {                                                 //否则
               /*Serial.print("failed, rc=");                       //串口进行显示
               //Serial.print(client.state());                      //调用函数，监测连接错误代码：
               //Serial.println(" try again in 5 seconds");*/       //5秒后进行从新连接
               delay(5000);                                         //延时5秒=5000ms
               }
           }
     }



void setup() {
     pinMode(BUILTIN_LED, OUTPUT);                                     //设置Led输出
     Serial.begin(9600);                                              //初始化串口
     setup_wifi();                                                     //调用WiFi连接函数
     client.setServer(mqtt_server, 1883);                              //设定MQTT服务器与使用的端口，1883是默认的MQTT端口
     client.setCallback(callback);                                     //设定MQTT服务器回调函数
     }



void loop() {
       float hum = dht.readHumidity();                                   //获取湿度
       float tem = dht.readTemperature();                                //获取温度
                   
       delay(500); 
       /*Serial.print("temperature");                                    //串口打印温湿度值
       Serial.print(tem);
       Serial.print("humidity");
       Serial.println(hum);*/

       if (!client.connected()) {                                         //如果MQTT客户端断线
           reconnect();                                                   //调用MQTT重连函数，确保连上服务器
           }  
       client.loop();                                                     //MCU接收数据的主循环函数
       long now = millis();                                               //millis()函数从程序开始处返回毫秒
       if (now - lastMsg >1000) {                                         //每隔1秒发一次信号
          lastMsg = now;                                                  //刷新上一次发送数据的时间
          ++num;                                                           //进行编号累加
          snprintf (msg, 75, "{Temperature:%.2f; Humidity:%.2f; num #%ld}",tem,hum,num);//将字符串存储至字符变量msg中
          Serial.print("Pub Message: ");                                  //串口打印发布的信息
          Serial.println(msg);
         client.publish("myPub1", msg);                                     //发布当前主题的消息
         }
      }
