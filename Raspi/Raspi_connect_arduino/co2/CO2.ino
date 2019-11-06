/************************************************************************************
本模块采用的MG-811探头属于电化学传感器，为得到更精确的测量值，请先对传感器进行标定操作。
给本模块提供稳定的电源，探头工作后会进行加热。请将本模块放在空气清新的地方，连续加热48小
时。然后测量本模块的输出电压，将输出电压值(单位：V)除以8.5，得到的数值填入代码中的宏定义：
#define         ZERO_POINT_VOLTAGE           (此处改成：电压值(V)/8.5)
例如，用万用表测量CO2模块输出的电压为2.4V，那么2.4/8.5=0.282, 则修改如下： 
#define         ZERO_POINT_VOLTAGE           (0.282)
***********************硬件接口宏定义**********************************************/
#define         MG_PIN                       (A0)    //
//#define         BOOL_PIN                     (2)     //电平输入引脚
#define         DC_GAIN                      (8.5)   //电源放大器增益(电压的放大倍数)
/***********************软件接口宏定义************************************/ 
#define         READ_SAMPLE_INTERVAL         (50)    //定义在正常操作中要取多少样品
#define         READ_SAMPLE_TIMES            (5)     //定义每个样本之间的时间间隔(单位为毫秒)
/*********************应用相关宏定义**********************************/
//These two values differ from sensor to sensor. user should derermine this value.
#define         ZERO_POINT_VOLTAGE           (0.084) //当二氧化碳浓度为400PPM时，用伏特定义传感器的输出(V测/8.5)
#define         REACTION_VOLTGAE             (0.030) //定义传感器从空气中移动到1000ppm CO2时的[电压降(电流流过负载，负载两端的电势差)]

float CO2Curve[3]  =  {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTGAE/(2.602-3))};  //全局变：CO2数组{2.602,伏特传感器输,电压降/(2.302-3)}
  //曲线上取出两，形成了一条与原始曲线“近似相等”的直线。数据格式:{x, y, slope};
                                                      //slope = ( reaction voltage ) / (log400 –log1000) 
                                                      //point1:(lg400, 0.324) point2:(lg4000, 0.280)
void setup()
{
    Serial.begin(9600);                    
             
}

void loop()
{
    int percentage;                             
    float volts;                                     
                                                  
    volts = MGRead(MG_PIN);                           //引用MGRead函数将模拟输入值转换为电压值


    percentage = MGGetPercentage(volts,CO2Curve);      //通过MGGetPercentage函数将电压值和数组转换成CO2 ppm值
    //Serial.print("CO2:");
    if (percentage == -1) {                                   
        Serial.println("400");
    } else {
       Serial.println(percentage);                     //输出CO2值
    }
    
    delay(1000);
}
float MGRead(int mg_pin){
    int i;
    float v=0;
    for (i=0;i<READ_SAMPLE_TIMES;i++) {      //   i<数据样本间隔(5ms)
        v += analogRead(mg_pin);             
        delay(READ_SAMPLE_INTERVAL);          //50个数据样本的间隔时间50*5/1000s
        }
    v = (v/READ_SAMPLE_TIMES) *5/1024 ;       //电压=
    return v;  
}
int  MGGetPercentage(float volts, float *pcurve){
   if ((volts/DC_GAIN )>=ZERO_POINT_VOLTAGE) {
    // (输出电压/电压增益)>=(V测/8.5)时
      return -1;
   } else { 
      return pow(10, ((volts/DC_GAIN)-pcurve[1])/pcurve[2]+pcurve[0]); 
      //return CO2的PPM值 = 10的[(输出电压/电压增益）-(V测/8.5）/(电压降/(2.302-3)+2.602)]次方
   }
}
