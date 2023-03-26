/* Blynk credentials */
char auth[] = "69d567d9fe1647b5a4dfeabb666f1c34";
char ssid[] = "SSID";
char pass[] = "12345689";


#define BLYNK_PRINT Serial
#define ONE_WIRE_BUS 2
#include <Servo.h> 
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(ONE_WIRE_BUS);  
DallasTemperature sensors(&oneWire);
BlynkTimer timer;
Servo myservo;
int alerts,alert_status,pos,del,deviceCount=0,count=0,motor_time=53,servopin=13;
float tempC, threshold_temp=40.0;

void sendSensor()
{
  if(count<motor_time)
  {
  sensors.requestTemperatures();  
  for (int i = 0;  i < 2;  i++)
  {

    Serial.print(i+1);
    Serial.print(" : ");
    tempC = sensors.getTempCByIndex(i);
    Serial.print(tempC);
    Serial.print((char)176);
    Serial.print("C");
    if(i==0)
    {
      Blynk.virtualWrite(V5, tempC);
      if(tempC>threshold_temp)
      {
        alerts++;
        Serial.print("alert triggered");
        digitalWrite(D0,HIGH);
        delay(500);
      }
    }
    if(i==1)
    {   
   Blynk.virtualWrite(V6, tempC);
   if(tempC>threshold_temp)
   {
    alerts++;
    Serial.print("alert triggered");
     digitalWrite(D0,HIGH);
     delay(500);
   }
    }
  } 
  }
  else if(count>motor_time)
  {
    motor();
  }
  count++;
  Serial.println("");
  delay(1000);
}
void motor(void)
{
  for(pos = 0; pos <= 45; pos += 1) 
  {                                  
    myservo.write(pos);              
    delay(15);                       
  }
  Serial.println("FEEDER ACTIVATED");
  digitalWrite(D1,HIGH);
  digitalWrite(D3,LOW);
  delay(10000);
  digitalWrite(D1,LOW);
  digitalWrite(D3,LOW);
  for(pos =45; pos>=0; pos-=1)     
  {                                
    myservo.write(pos);              
    delay(15);                      
  }
  Serial.println("FEEDER SHUTTING DOWN");
  delay(1000);
  count=0;
}

BLYNK_WRITE(V0)
{
  del=param.asInt();
  motor_time=del*5;//*5 means in each 10 sec   //delay *30 -> a minute
  Serial.print("Delay");
  Serial.println(del);
}
BLYNK_WRITE(V1)
{
  alert_status=param.asInt();
if(alert_status==1)
{
  alerts=0;
  Serial.print("Alerts Reset");
}
}

void setup(void)
{
  sensors.begin();  // Start up the library
  Serial.begin(9600);
  myservo.attach(servopin);
  Blynk.begin(auth, ssid, pass);
  pinMode(D0,OUTPUT);
  pinMode(D1,OUTPUT);
  pinMode(D3,OUTPUT);
  pinMode(D6,OUTPUT); 
  digitalWrite(D6,HIGH);
  digitalWrite(D0,LOW);
  timer.setInterval(1000L, sendSensor);
}

void loop(void)
{ 

  Blynk.run();
  timer.run();
  
}
