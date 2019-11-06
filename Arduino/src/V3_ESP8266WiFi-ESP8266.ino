#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "shuge-Xiaomi_2.4G";
const char* password = "yuhan123456";
const char* mqtt_server = "192.168.3.212";

WiFiClient espClient;
PubSubClient client(espClient);

String _databuffer="";
long lastMsg = 0;
char msg[30];
int value = 0;

void setup_wifi() {
      delay(10);
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
           delay(500);
           }
      randomSeed(micros());
    }

void callback(char* topic, byte* payload, unsigned int length) {
      for (int i = 0; i < length; i++) {
          //Serial.print((char)payload[i]);  //串口打印出接收到的数据让你看看
          }
      if ((char)payload[0] == '1') {   //如果收到的数据是1
          digitalWrite(BUILTIN_LED, LOW);   
      } else {
          digitalWrite(BUILTIN_LED, HIGH); 
      }
}
 
void reconnect() {        
     while (!client.connected()) {                                             
           String clientId = "ESP8266-V3.1-";
           clientId += String(random(0xffff), HEX);// Create a random client ID
           if (client.connect(clientId.c_str(),"admin","public")) {
               client.subscribe("mySub2");
               delay(20);
           } else {
               delay(5000);
               }
           }
     }

void setup() {
     pinMode(BUILTIN_LED, OUTPUT);    
     Serial.begin(9600);
     setup_wifi();
     client.setServer(mqtt_server, 1883);
     client.setCallback(callback);
      //while(Serial.read()>=0){};其存在与否无影响
     }

void loop() {
       if (!client.connected()) {
           reconnect();//确保连上服务器，否则一直等待
           }  
       client.loop();//MUC接收数据的主循环函数
       long now = millis();
       if (now - lastMsg >1000) {   //每隔1秒发一次信号
           lastMsg = now;     //刷新上一次发送数据的时间
           ++value; 
  /*******************************************debug1*********************************************
    *  while(Serial.available()){
    *      _data00buffer=Serial.readString();
    *      delay(10);
    *  }
    *   client.publish("myPub1",  _data00buffer);
  ****************************************************************************************/    
         
  /********************************************debug2********************************************
      *     String  payload=Serial.readString();     
      *     char attributes[100];
      *     payload.toCharArray(attributes,100);
      *     client.publish("myPub2 ", attributes);
   ****************************************************************************************/

     while(Serial.available()){
        char dataread=(char)Serial.read();
        _databuffer+=dataread;
        }
     //client.publish("myPub",_databuffer.c_str());    
     snprintf (msg, 75, "%s#%d", _databuffer.c_str(),value); //string转char型/*此处函数变量的_databuffer.c_str()为字符串转字串类型，输出使用"%s"与"%c"的影响：
     client.publish("myPub", msg); 
      _databuffer=""; 
     //_databuffer=String("");
           }      
      }
