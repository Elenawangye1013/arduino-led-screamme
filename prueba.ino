#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#define LED_PIN     D3
#define NUM_LEDS         300
#define CHIPSET          WS2812B
#define COLOR_ORDER      GRB
CRGBArray<NUM_LEDS> leds;

#define BRIGHTNESS  128

int mic = A0; //analog pin

int minVol = 350;
int maxVol = 800; //0 to 1023 - high value = lower volume
int addLedsConstant = 3 ; //how fast are LEDS added to bar
int removeLedsTimeConstant = 2; //how fast are LEDS removed from bar
int middleColorLED = 100;
int numMensaje= 1;

int numLedsToLight;

long lastRefreshTime;

// tWITTER DEFINE 
String API = "GGWF1E3J9D6EQJPN";
// enter your ssid and password
const char* ssid = "Elena";
const char* password = "11111111";
String tweet = "fssxssxffssssddssrwef";
WiFiClient client;

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness(BRIGHTNESS);
  WiFi.begin(ssid, password);
}

void loop() {
  
 
  //Serial.println(analogRead(mic));

  int ledsToLight = map(analogRead(mic), minVol, maxVol, 0, NUM_LEDS);
  Serial.println(ledsToLight);

  
  if (ledsToLight > numLedsToLight) {
    numLedsToLight += addLedsConstant;
  } else if (ledsToLight < numLedsToLight) {
    if(millis() - lastRefreshTime >= removeLedsTimeConstant) {
      lastRefreshTime += removeLedsTimeConstant;
      numLedsToLight -= 1;
    }
  }
      
  if (numLedsToLight < 1) {
    numLedsToLight = 0;
  }
  if (numLedsToLight > NUM_LEDS) {
    numLedsToLight = NUM_LEDS;
  }
  

  for(int led = 0; led < numLedsToLight; led++) {
    if (led < middleColorLED) {
      leds[led] = CRGB(map(led, 0, middleColorLED - 1, 0, 255) ,255,0);
    } else {
      leds[led] = CRGB(255, map(led, middleColorLED, NUM_LEDS, 255, 0),0);
    }
  }
  for(int led = NUM_LEDS; led >= numLedsToLight; led--) {
    leds[led] = CRGB(0, 0, 0);
  }
  
  FastLED.show();
  if(numLedsToLight>=300){
    post();
    delay(5000);
    }
  }

void post(){
   
 if(client.connect("184.106.153.149", 80)){
  client.print("GET /apps/thingtweet/1/statuses/update?key=" + API + "&status=" + tweet + " HTTP/1.1\r\n");
  client.print("Host: api.thingspeak.com\r\n");
  client.print("Accept: */*\r\n");
  client.print("User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n");
  client.print("\r\n");
  
  tweet = "Enhorabuena%20lo%20conseguiste%20eres%20el%20numero:%20"+ String(numMensaje);
  numLedsToLight=0;
  numMensaje ++;
  }
  loop();
 }
