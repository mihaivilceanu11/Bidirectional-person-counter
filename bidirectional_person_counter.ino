#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "ThingSpeak.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "Adafruit_SGP30.h"


#define senzorIntrare D5 // in sensor
#define senzorIesire D6 // out sensor
//Bot telegram
#define botToken "****************************************************" //telegram bot token
#define myChat "************" //your telegram chat ID

//WiFi
const char* wifi_name = "******"; //your wifi name
const char* wifi_password = "*******";  // your wi fi password
WiFiClientSecure  client_telegram; 
WiFiClient client_thingspeak;


unsigned int channel_ts = ;//thingspeak channel number
const char * write_apiKey = "***********"; // your telegram API Key

unsigned long t; // timer for IoT upload
int nrPersoane = 0; //person counter

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
UniversalTelegramBot bot(botToken, client_telegram);

Adafruit_SSD1306 display(128, 64, &Wire, -1);//Display


Adafruit_SGP30 senzor_tvoc;
int co2 = 0;
int tvoc = 0;

void startWifi(){

  if(WiFi.status() != WL_CONNECTED){

      Serial.print("Connecting...");
      while(WiFi.status() != WL_CONNECTED){
          WiFi.begin(wifi_name, wifi_password); 
          delay(5000);     
      } 
    Serial.println("\nConnected");
    bot.sendMessage(myChat, "System online!", "");//telegram message after connecting
  }

}
void setup(){

  nrPersoane = 0; //person counter
  co2 = 0;
  tvoc = 0;
  
  t = millis();
 
  pinMode(senzorIntrare, INPUT);
  pinMode(senzorIesire, INPUT);

  Serial.begin(115200);

  senzor_tvoc.begin(&Wire);

  senzor_tvoc.setIAQBaseline(0x8CD9, 0x8940); //setting sensor baseline (optional)

  configTime(0, 0, "pool.ntp.org");      
  client_telegram.setTrustAnchors(&cert); 

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
 	display.println("Camera");
  display.println("este");
  display.println("goala.");
  display.display();

	WiFi.mode(WIFI_STA);   
  
  ThingSpeak.begin(client_thingspeak);  

  startWifi();
  
	delay(2000);
  display.clearDisplay();
}

void loop() {
  
  display.setCursor(0,0);

      if(digitalRead(senzorIntrare) == LOW){
          delay(150);

          if(digitalRead(senzorIesire) == LOW){
              nrPersoane++;    
              delay(500);  
          }     
      }
      else
          if(digitalRead(senzorIesire) == LOW){
              delay(150);      

              if(digitalRead(senzorIntrare) == LOW){
                nrPersoane--; 
                delay(500); 
              }           
          }
       
      if(nrPersoane < 0)
        nrPersoane = 0;

        senzor_tvoc.IAQmeasure();
        co2 = senzor_tvoc.eCO2;
        tvoc = senzor_tvoc.TVOC;

      if((millis() - t) > 60000){

          Serial.println(nrPersoane);
          
          Serial.println(String(co2) + " " + String(tvoc));

         
          
          if(WiFi.status() != WL_CONNECTED)
              startWifi();
              
//Write multiple channels at once --------------------------------------------------------------------------------------------------------------------------
            ThingSpeak.setField(1, nrPersoane);
            ThingSpeak.setField(2, co2);
            ThingSpeak.setField(3, tvoc);
            int x = ThingSpeak.writeFields(channel_ts, write_apiKey);
//---------------------------------------------------------------------------------------------------------------------------------------------------
  
          if(x == 200){
            Serial.println("Datele au fost scrise cu succes.");
          }
          else{
                Serial.println("Nu s-a putut actualiza. Cod eroare: " + String(x));
              }

          t = millis();
      }

      if(nrPersoane == 0){
        display.println("Camera");
        display.println("este");
        display.println("goala.");
      }
      else
          if(nrPersoane == 1){
            display.println("In camera");
            display.println("este");
            display.println("o persoana");
          }
          else
              if(nrPersoane == 2){
                display.println("In camera");
                display.println("sunt");
                display.println("doua");
                display.println("persoane");
              }
              else 
                  if(nrPersoane < 10){
                    display.println("In camera");
                    display.println("sunt");
                    display.print(nrPersoane);
                    display.print(" persoane");
                  }
                  else{
                    display.println("In camera");
                    display.println("sunt");
                    display.println(nrPersoane);
                    display.println("persoane");
                  }                  
      
      display.display();
      display.clearDisplay();

      delay(1);
}
