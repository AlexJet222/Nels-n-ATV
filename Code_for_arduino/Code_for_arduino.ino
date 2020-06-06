#include <SoftwareSerial.h>
SoftwareSerial esp8266(2,3); //Pin 2 & 3 of Arduino as RX and TX. Connect TX and RX of ESP8266 respectively.
#define DEBUG true
#define MOTOR_R_forward 11 //LED is connected to Pin 11 of Arduino
#define MOTOR_R_backward 10
#define MOTOR_L_forward 13
#define MOTOR_L_backward 12

void InitMotorR()
{
  pinMode(MOTOR_R_forward,OUTPUT);
  digitalWrite(MOTOR_R_forward,LOW);
  pinMode( MOTOR_R_backward,OUTPUT);
  digitalWrite(MOTOR_R_backward,LOW);
}
void InitMotorL()
{
  pinMode(MOTOR_L_forward,OUTPUT);
  digitalWrite(MOTOR_L_forward,LOW);
  pinMode(MOTOR_L_backward,OUTPUT);
  digitalWrite(MOTOR_L_backward,LOW);
}

void MotorLForward()
{
  digitalWrite(MOTOR_L_backward,LOW);
  digitalWrite(MOTOR_L_forward,HIGH);
}
void MotorRForward()
{
  digitalWrite(MOTOR_R_backward,LOW);
  digitalWrite(MOTOR_R_forward,HIGH);
}

void MotorLBackward ()
{
  digitalWrite(MOTOR_L_forward,LOW);
  digitalWrite(MOTOR_L_backward,HIGH);
}
void MotorRBackward()
{
 digitalWrite(MOTOR_R_forward,LOW);
 digitalWrite(MOTOR_R_backward,HIGH);
}


void MotorLStop()
{
  digitalWrite(MOTOR_L_backward,LOW);
  digitalWrite(MOTOR_L_forward,LOW);
}

void MotorRStop()
{
  digitalWrite(MOTOR_R_backward,LOW);
  digitalWrite(MOTOR_R_forward,LOW);
}

void setup()
  {
    InitMotorR();
    InitMotorL();
    Serial.begin(9600);
    esp8266.begin(9600); //Baud rate for communicating with ESP8266. Your's might be different.
    esp8266Serial("AT+RST\r\n", 5000, DEBUG); // Reset the ESP8266
    Serial.println ("Starting....");
    esp8266Serial("AT+CWMODE=1\r\n", 5000, DEBUG); //Set station mode Operation   s
   esp8266Serial("AT+CWJAP_DEF=\"Tuborg 68\",\"12345\"\r\n", 5000, DEBUG);//Enter your WiFi network's SSID and Password.
                                
    /*while(!esp8266.find("OK")) 
    {
      } */
      Serial.print ("CIFSR");
    esp8266Serial("AT+CIFSR\r\n", 5000, DEBUG);//You will get the IP Address of the ESP8266 from this command. 
    esp8266Serial("AT+CIPMUX=1\r\n", 5000, DEBUG);
    esp8266Serial("AT+CIPSERVER=1,80\r\n", 5000, DEBUG);
  }

void loop()
  {Serial.println ("Did i pass?");
    if (esp8266.available())
      {
        Serial.print ("Yes");
        if (esp8266.find("+IPD,"))
          {
            String msg;
            esp8266.find("?");
            msg = esp8266.readStringUntil(' ');
            String command1 = msg.substring(0,3);
            String command2 = msg.substring(4);           
            if (DEBUG) 
              {
                Serial.println(command1);//Must print "led"
                Serial.println(command2);//Must print "ON" or "OFF"
              }
            delay(100);

              if (command2 == "MLF") 
                    {
                      MotorLForward();
                    }
                   
              if (command2 == "MRF") 
              {
                 MotorRForward();
              }

              if(command2 == "MLS")
              {
                MotorLStop();
              }
              if(command2 == "MRS")
              {
                MotorRStop();
              }
              if(command2 == "MLB")
              {
                MotorLBackward ();
              }
              if (command2 == "MRB")
              {
                MotorRBackward();
              }
          }
      }
  }
   
String esp8266Serial(String command, const int timeout, boolean debug)
  {
    String response = "";
    esp8266.print(command);
    long int time = millis();
    while ( (time + timeout) > millis())
      {
        while (esp8266.available())
          {
            char c = esp8266.read();
            response += c;
          }
      }
    if (debug)
      {
        Serial.print(response);
      }
    return response;
  }
