/**************************************************************************
 *                                                                       *
     NeoPixel / WS2812b Adventskalender
     created by Tim! / 54696d21

   rev 1 10.12.2017
 *                                                                      *
 **************************************************************************/


// benötigte Bibilotheken -- required libraries
#include <ESP8266WiFi.h>  //standard library
#include <TimeLib.h>    //TimeLib.h by Paul Stoffregen
#include <Wire.h>       //I2C
#include <NTPClient.h>  //NTP Zeit -- NTP Time
#include <WiFiUdp.h>    //Netzwerkprotokoll für NTP -- Network protocol for NTP
#include <Adafruit_NeoPixel.h> //LED-Treiber -- LED driver


#define LED_INPUT_DATA D1 //Datenpin leds -- Data pin LEDs
#define BRIGHTNESS 10 // Helligkeit -- brightness
#define PIXELS 24   //Anzahl der Leds -- Number of LEDs

WiFiUDP ntpUDP;               //Protokoll für NTP auf UDP festlegen -- Set protocol for NTP to UDP
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 20000); //festlegen der NTP-Einstellungen -- specify the NTP settings

const char *ssid     = "**********************";          //WLAN daten zum stellen der Uhr via NTP -- WLAN data to set the clock via NTP
const char *password = "**********************";

Adafruit_NeoPixel leds = Adafruit_NeoPixel(PIXELS, LED_INPUT_DATA, NEO_GRB + NEO_KHZ800); // led treiber definitionen -- led driver definitions



void setup () {
  WiFi.begin(ssid, password); //aufbauen der WLAN-Verbindung -- establishing the WLAN connection
  Serial.begin(115200); //Serielle Schnittstelle initialisieren zur Textausgabe -- Initialize serial interface for text output
  Wire.begin();   // I2C  initialisieren -- Initializing I2C


  pinMode(LED_INPUT_DATA, OUTPUT); //festlegen des Datenpins der NeoPixel -- defining the data pin of the NeoPixel
  leds.begin();           //leds initialisieren -- initialize leds
  leds.setBrightness(BRIGHTNESS); // leds auf oben definierten Hellgikeitswert setzen -- Set the LEDs to the brightness value defined above



}

void loop () {
  timeClient.update();  //Zeitverwaltung via NTP -- Time management via NTP
  setTime(timeClient.getEpochTime()); //NTP-Zeit übergeben, zur verwaltung in einem human-readable-format -- NTP time passed for management in a human-readable format
  Serial.print("Day of the month: "); //Serial.print("Tag im Monat: ");
  Serial.println(day());

  for(int i=0;i<=(day()-1);i++){  //leds pixelweise einschalten bis jeweiliger Tag erreicht ist -- Switch on LEDs pixel by pixel until the respective day is reached
    Serial.print("count up: "); //Serial.print("loop hochzählen: ");
    Serial.println(i); //ausgabe der nummer der aktuellen led -- Output the number of the current LED
    leds.setPixelColor(i, leds.Color(150,0,0)); // reines rot -- pure red
    leds.show(); //der zuletzt auf leds geschriebene zustand wird angezeigt -- The status last written to the LEDs is displayed
    delay(300); //Wartezeit der Animation -- Animation waiting time


  }
  leds.show(); //anzeigen bei austritt aus der schleife (sollte nicht notwendig sein) -- Display when exiting the loop (should not be necessary)


  Serial.print("Show current day of the month: "); //Serial.print("Zeige aktuellen Tag im Monat: ");
  Serial.println(day()); //ausgabe des tages im monat -- edition of the day in the month
  Serial.println("Wait..."); // Serial.println("Warte...");
  delay(5000); //zeitdauer in der alle leds des jeweiligen tages an sind -- Duration in which all LEDs of the respective day are on


  for(int l=(day()-1); l>=0 ;l--){  //day-1, because the NeoPixels start at; LEDs go out one after the other, starting from daily maximum up to 0 //for(int l=(day()-1); l>=0 ;l--){  //day-1, denn die NeoPixelbeginnen bei 0; leds gehen nacheinander aus, ausgehend von tagesmaxium bis zur 0
    Serial.print("count down loop: "); //Serial.print("loop runterzählen: ");
    Serial.println(l); //ausgabe des tages im monat -- edition of the day in the month

    leds.setPixelColor(l, leds.Color(0,0,0)); //Schwarzwert -- Black level
    leds.show(); //der zuletzt auf leds geschriebene zustand wird angezeigt -- The status last written to the LEDs is displayed
    delay(300); //Dauer der Animation -- Duration of animation
  }
  leds.show();  //anzeigen bei austritt aus der schleife (sollte nicht notwendig sein) -- Display when exiting the loop (should not be necessary)

  Serial.println("Wait..."); //Serial.println("Warte...");
  delay(1000); //zeitdauer in der alle leds aus sind -- Duration in which all LEDs are off

}
