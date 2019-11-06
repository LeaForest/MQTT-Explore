#include "DHT.h"
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <WiFiEspClient.h>
#include <PubSubClient.h>
#include "SoftwareSerial.h"

#define DHTPIN 4
#define DHTTYPE DHT11
const char* ssid = "shuge-Xiaomi_2.4G";
const char* password = "yuhan123456";
char MqttServer[] = "192.168.3.212";

WiFiEspClient espClient;       //初始化以太网客户端对象 -- WiFiEspClient.h
PubSubClient client(espClient);//初始化MQTT库PubSubClient.h的对象
DHT dht(DHTPIN, DHTTYPE);       //初始化DHT11传感器
SoftwareSerial soft(7, 8);     //RX, TX
long lastSend=0;
char msg[50];
int value=0;
int status=WL_IDLE_STATUS;


 void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Sub Message:");
    for (int i = 0; i < length; i++) {
     Serial.print((char)payload[i]); // 打印主题内容 
    } 
    Serial.println(); 
    if ((char)payload[0] == '1') {
          digitalWrite(13, HIGH); // 亮灯 
    } else {
          digitalWrite(13, LOW); // 熄灯 
          }
   }

 void reconnect() {        
     while (!client.connected()) {                                               
           String clientId = "ESP8266-Esp-";
           clientId += String(random(0xffff), HEX);// Create a random client ID
           if (client.connect(clientId.c_str(),"admin","public")) {
               client.subscribe("mySub");
               delay(20);
           } else {
               Serial.print("failed, rc=");
               Serial.print(client.state());
               /*Serial.println(" try again in 5 seconds");*/
               delay(3000);
               }
           }
     }

     
void setup() {
/**********************串口初始化***********************************/
       pinMode(13, OUTPUT);
        Serial.begin(9600);
        //dht.begin();
       soft.begin(9600);                        //初始化软串口，软串口连接ESP模块 
       WiFi.init(&soft);                        //初始化ESP模块
 /*********************WIFI模块监测与连接********************************/                                      
       delay(10);
       while ( status != WL_CONNECTED) {           //检测到WIFI模块时
            delay(500);
            status = WiFi.begin(ssid, password);//开始进行连接
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
            }
       randomSeed(micros());
       client.setServer( MqttServer, 1883 );      //连接WiFi之后，连接MQTT服务器的地址和端口
       client.setCallback(callback);
  }


void loop() {
        delay(500);
/**********************判断服务器连接状态*******************/
        if ( !client.connected() ) {               //如果MQTT客户端断线
             reconnect();//确保连上服务器，否则一直等待
                 }
          client.loop();
 /******************************数据定时发送**************************************/
   long now = millis();
        if ( now - lastSend > 1000 ) { // 用于定时1秒钟发送一次数据
          lastSend = now;     //刷新上一次发送数据的时间
          ++value;
          snprintf (msg, 75, "{Hello World!; num #%ld}",value);
          Serial.print("Pub Message: ");
          Serial.println(msg);
          client.publish("myPub", msg);
        }
      }


