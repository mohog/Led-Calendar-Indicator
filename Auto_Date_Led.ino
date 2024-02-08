//


#include <ESP8266WiFi.h>
#include <time.h>
#include <Wire.h>       //I2C
#include <Adafruit_NeoPixel.h> //LED driver

#define LED_INPUT_DATA D1	//Data pin for leds
#define BRIGHTNESS 10 // brightness
#define PIXELS 31		//Number of LEDs
Adafruit_NeoPixel leds = Adafruit_NeoPixel(PIXELS, LED_INPUT_DATA, NEO_GRB + NEO_KHZ800); // led driver definitions

const char* ssid = "**********"; // network name
const char* password = "**********"; // network password
const char* NTP_SERVER = "time.apple.com"; // "192.168.4.1";
const char* TZ_INFO    =  "CST6CDT5,M3.2.0,M11.1.0"; //"CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";  // enter your time zone (https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv)  (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)
tm timeinfo;
time_t now;
long unsigned lastNTPtime;
unsigned long lastEntryTime;
#define numday 31;


void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("\n\nNTP Sync Calendar\n");
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  Wire.begin();   // I2C  initialize
  pinMode(LED_INPUT_DATA, OUTPUT); //defining the data pin of the NeoPixel
  leds.begin();						//initialize leds
  leds.setBrightness(BRIGHTNESS); // Set the LEDs to the brightness value defined above

  WiFi.begin(ssid, password);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    if (++counter > 100) ESP.restart();
    Serial.print ( "." );
  }
  Serial.println("\n\nWiFi connected\n\n");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP() );

  configTime(0, 0, NTP_SERVER);
  // See https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv for Timezone codes for your region
  setenv("TZ", TZ_INFO, 1);
  Serial.println("\nWaiting for Internet time");

  if (getNTPtime(10)) {  // wait up to 10 sec to sync
  } else {
    Serial.println("Time not set");
    ESP.restart();
  }
  showTime(timeinfo);
  lastNTPtime = time(&now);
  lastEntryTime = millis();
}

void loop() {
  getNTPtime(10);
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
   
  Serial.print("day of the month: ");
  Serial.println(numday());

  for(int i=0;i<=(numday()-1);i++){	//Switch on LEDs pixel by pixel until the respective day is reached
    Serial.print("loop count up: ");
    Serial.println(i); //Output the number of the current LED
    leds.setPixelColor(i, leds.Color(150,0,0)); // pure red.
    leds.show(); //The status last written to the LEDs is displayed
    delay(300);	//Animation waiting time


  }
  leds.show(); //Display when exiting the loop (should not be necessary)


  Serial.print("Show current day of the month: ");
  //Serial.println(day()); //day in the month
  Serial.println("Wait...");
  delay(10000); //Duration in which all LEDs of the respective day are on


  for(int l=(numday()-1); l>=0 ;l--){	//day-1, because the NeoPixels start at 0; LEDs go out one after the other, starting from the daily maximum to 0
    Serial.print("count down loop: ");
    Serial.println(l); //day in the month

    leds.setPixelColor(l, leds.Color(0,0,0)); //Black level
    leds.show(); //The status last written to the LEDs is displayed
    delay(300);	//Duration of animation
  }
  leds.show();	//Display when exiting the loop (should not be necessary)

  Serial.println("Wait...");
  delay(1000); //Duration in which all LEDs are off


  

}

bool getNTPtime(int sec) {

  {
    uint32_t start = millis();
    do {
      time(&now);
      localtime_r(&now, &timeinfo);
      //Serial.print(".");
      delay(10);
    } while (((millis() - start) <= (1000 * sec)) && (timeinfo.tm_year < (2016 - 1900)));
    if (timeinfo.tm_year <= (2016 - 1900)) return false;  // the NTP call was not successful
    //Serial.print("now ");  Serial.println(now);
    char time_output[30];
    strftime(time_output, 30, "%a  %m-%d-%y %T", localtime(&now));
    //Serial.println(time_output);
    //Serial.println();
  }
  return true;
}



/*
void showTime(tm localTime) {
  Serial.print(localTime.tm_mday);
  Serial.print('/');
  Serial.print(localTime.tm_mon + 1);
  Serial.print('/');
  Serial.print(localTime.tm_year - 100);
  Serial.print('-');
  Serial.print(localTime.tm_hour);
  Serial.print(':');
  Serial.print(localTime.tm_min);
  Serial.print(':');
  Serial.print(localTime.tm_sec);
  Serial.print(" Day of Week ");
  if (localTime.tm_wday == 0)   Serial.println(7);
  else Serial.println(localTime.tm_wday);
}
*/

 // Shorter way of displaying the time
  void showTime(tm localTime) {
  Serial.printf(
    "%04d-%02d-%02d %02d:%02d:%02d, day %d, %s time\n",
    localTime.tm_year + 1900,
    localTime.tm_mon + 1,
    localTime.tm_mday,
    localTime.tm_hour,
    localTime.tm_min,
    localTime.tm_sec,
    (localTime.tm_wday > 0 ? localTime.tm_wday : 7 ),
    (localTime.tm_isdst == 1 ? "summer" : "standard")
  );
  }
