#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 5
 

const int analogInPin = A0; 
int relay = D8;
int relay2 = D4;
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;
float phValue, suhu;
char auth[] = "aa627b3f42d34432a1a7e733d306e427";

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "not for free!!!";
char pass[] = "masihsama";

BlynkTimer timer;

void myTimerEvent()
{
  Blynk.virtualWrite(V2, suhu);
  Blynk.virtualWrite(V5, phValue);
}

BLYNK_CONNECTED() {
    Blynk.syncAll();
}

void setup() {
 Serial.begin(9600);
 Blynk.begin(auth, ssid, pass);
 sensors.begin();
 timer.setInterval(1000L, myTimerEvent);
}
 
void loop() {
 Blynk.run();
 timer.run();
 suhu = sensors.getTempCByIndex(0);
 sensors.requestTemperatures();
 Serial.println(analogRead(A0));
 for(int i=0;i<10;i++) 
 { 
  buf[i]=analogRead(analogInPin);
  Blynk_Delay(10);
 }
 for(int i=0;i<9;i++)
 {
  for(int j=i+1;j<10;j++)
  {
   if(buf[i]>buf[j])
   {
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];

 //Kalibrasi PH nang kene
 //Nek isik bermasalah kemungkinan probe e rusak
 float pHVol=(float)avgValue*3.3/1024/6;
 phValue = -5.70 * pHVol + 21.34;
 Serial.print("sensor = ");
 Serial.println(avgValue);
 Serial.println(phValue);

 if(suhu<20){
   digitalWrite(relay,LOW);
 }else if(suhu>22){
   digitalWrite(relay,HIGH); 
 }
 Blynk_Delay(20);
}

void Blynk_Delay(int milli){
   int end_time = millis() + milli;
   while(millis() < end_time){
       Blynk.run();
       timer.run();
       sensors.requestTemperatures();
       yield();
   }
}
