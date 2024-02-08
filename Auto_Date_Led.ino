/**************************************************************************
 *                                                                       *
     NeoPixel / WS2812b advent Calendar
     created by Tim! / 54696d21

	 rev 1 10.12.2017
 *                                                                      *
 **************************************************************************/


// benötigte Bibilotheken
#include <ESP8266WiFi.h>	//standard library
#include <TimeLib.h>		//TimeLib.h by Paul Stoffregen
#include <Wire.h>       //I2C
#include <NTPClient.h>  //NTP Zeit
#include <WiFiUdp.h>    //Network protocol for NTP
#include <Adafruit_NeoPixel.h> //LED driver


#define LED_INPUT_DATA D1	//Data pin for leds
#define BRIGHTNESS 10 // brightness
#define PIXELS 31		//Number of LEDs
const long utcOffsetInSeconds = -21600;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
WiFiUDP ntpUDP;               //Set protocol for NTP to UDP
NTPClient timeClient(ntpUDP, "192.168.4.1", utcOffsetInSeconds); //specify the NTP settings
//NTPClient timeClient(ntpUDP, "time.apple.com", utcOffsetInSeconds); //specify the NTP settings

const char *ssid     = "esptime";					//WLAN data to set the clock via NTP
const char *password = "esp123456";
//const char *ssid     = "Big";					//WLAN data to set the clock via NTP
//const char *password = "patrick5";

Adafruit_NeoPixel leds = Adafruit_NeoPixel(PIXELS, LED_INPUT_DATA, NEO_GRB + NEO_KHZ800); // led driver definitions



void setup () {
  Serial.begin(9600); //Initialize serial interface for text output
  Serial.println("\n\nAlive.");
  WiFi.begin(ssid, password); //establishing the WLAN connection
  Wire.begin();   // I2C  initialize

  delay(5000);

  pinMode(LED_INPUT_DATA, OUTPUT); //defining the data pin of the NeoPixel
  leds.begin();						//initialize leds
  leds.setBrightness(BRIGHTNESS); // Set the LEDs to the brightness value defined above
  leds.setPixelColor(PIXELS, leds.Color(0,0,0)); //Black level
  leds.show();
  timeClient.update();  //Time management via NTP
  setTime(timeClient.getEpochTime()); //NTP time passed for management in a human-readable format
  delay(1000);
  timeClient.update();  //Time management via NTP
  setTime(timeClient.getEpochTime()); //NTP time passed for management in a human-readable format
  Serial.println("\n\nAll Black. Starting with a clean slate.");
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());


}

void loop () {
  timeClient.update();  //Time management via NTP
  setTime(timeClient.getEpochTime()); //NTP time passed for management in a human-readable format
  Serial.print("Current day of the month is ");
  Serial.print(day());
  Serial.println(". Filling LEDs");

  for(int i=0;i<=(day()-1);i++){	//Switch on LEDs pixel by pixel until the respective day is reached
    //Serial.print("loop turn on: ");
    //Serial.println(i); //Output the number of the current LED
    leds.setPixelColor(i, leds.Color(0,150,0)); // front fill green
    leds.setPixelColor(i-1, leds.Color(150,0,0)); // back fill red
    leds.show(); //The status last written to the LEDs is displayed
    delay(300);	//Animation waiting time


  }
  leds.show(); //Display when exiting the loop (should not be necessary)


  //Serial.print("Show current day of the month: ");
  //Serial.println(day()); //day in the month
  Serial.println("Waiting...53.50 seconds");
  delay(3500); //Duration in which all LEDs of the respective day are on
  Serial.println("Waiting...50 seconds");
  delay(10000); //Duration in which all LEDs of the respective day are on
  Serial.println("Waiting...40 seconds");
  delay(10000); //Duration in which all LEDs of the respective day are on
  Serial.println("Waiting...30 seconds");
  delay(10000); //Duration in which all LEDs of the respective day are on
  Serial.println("Waiting...20 seconds");
  delay(10000); //Duration in which all LEDs of the respective day are on
  Serial.println("Waiting...10 seconds");
  delay(10000); //Duration in which all LEDs of the respective day are on
  Serial.println("Wiping LEDs clean");


for(int i=0;i<=(day()-1);i++){	//Switch off LEDs pixel by pixel until the respective day is reached
    //Serial.print("loop turn off: ");
    //Serial.println(i); //Output the number of the current LED
    leds.setPixelColor(i, leds.Color(0,0,0)); // Black
    leds.show(); //The status last written to the LEDs is displayed
    delay(75);	//(300); //Animation waiting time


  }
  leds.show(); //Display when exiting the loop (should not be necessary)
  

  Serial.println("Waiting...1 second");
  delay(1000); //Duration in which all LEDs are off

}
