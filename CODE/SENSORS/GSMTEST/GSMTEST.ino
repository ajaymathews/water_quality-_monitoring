#include<SoftwareSerial.h>
SoftwareSerial mySerial(10,11); //SIM800L Tx & Rx is connected to Arduino #3 & #2
int _timeout;
String _buffer;
String number = "+91XXXXXXXXXX";
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing...");
  delay(1000);
  digitalWrite(2,LOW);

  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  mySerial.println("AT+CSCS?"); //Check whether it has registered in the network
  updateSerial();
}


void loop()
{
  //updateSerial();
  if (digitalRead(2)==HIGH)
  {
    Serial.println("GSM STARTED");
     SendMessage();
     callNumber();
     delay(40000);
  }  
}
  void callNumber() {
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  
  mySerial.print (F("ATD"));
  mySerial.print (number);
  mySerial.print (F(";\r\n"));
  
  _buffer = _readSerial();
  Serial.println(_buffer);
}
String _readSerial() {
  _timeout = 0;
  while  (!mySerial.available() && _timeout < 1000  )
  {
    delay(13);
    _timeout++;
  }
  
  if (mySerial.available()) {
    Serial.println("Available");
    return mySerial.readString();
  }
}
void hangNumber() {
  mySerial.print (F("ATH"));
  mySerial.print (number);
  mySerial.print (F(";\r\n"));
  
  _buffer = _readSerial();
  Serial.println(_buffer);}
  
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
void SendMessage()
{
  Serial.println ("Sending Message");
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println ("Set SMS Number");
  mySerial.println("AT+COPS=0,2");
  mySerial.println("AT+COPS?");
  mySerial.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(1000);
  String SMS = "Alert!!  Excess Temperature in fish Tank";
  mySerial.println(SMS);
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println("_________");
  _buffer = _readSerial();
  Serial.println("Message sent");
}
