#include <OneWire.h>  
#include <ArduinoJson.h>                 


int DS18S20_Pin = 2;
OneWire ds(DS18S20_Pin);


void setup() {
  Serial.begin(9600);
  delay(100);
  }
  
  
void loop() { 
    float temperature = getTemp(); 
      Serial.print("{\"Temperature\":");
      Serial.print(temperature);
      Serial.println("}");
     delay(1000);
}
/***********************************************************************
                         温度处理函数
 ***********************************************************************/
float getTemp(){
  byte data[12];
  byte addr[8];
  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
      }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
      }
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
      }
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end
  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  ds.reset_search();
  byte MSB = data[1];
  byte LSB = data[0];
  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  return TemperatureSum;
}
