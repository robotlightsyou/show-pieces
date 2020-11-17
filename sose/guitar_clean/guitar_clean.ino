/*This is an attempt to merge Bitluni's Wifi web controlled lights 
with Bruh Automations OTA updates. 
*/

#include <ArduinoOTA.h>
#include <FastLED.h>  //might need to switch <> with " ".
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

#include <Adafruit_NeoPixel.h>


#include "PinStates.h"
#include "LedStates.h"
#include "Fader.h"
#include "RainbowFunction.h"
#include "christmas.h"
#include "hanukkah.h"
#include "SimpleRGBFunction.h"
#include "WaveFunction.h"
#include "RF.h"
#include "wtest.h"
#include "wave2.h"
#include "wave4.h"
#include "wave3.h"

////////////////////
//Wifi credentials//
////////////////////
const char* ssid = "network";
const char* password = "********";


ESP8266WebServer server(80);

///////////////////
//OTA Credentials//
///////////////////
#define SENSORNAME "guitar"  //Name your device here
#define OTApassword "guitar" //password to verify uploads via arduino IDE
int OTAport = 8266;  //I have no idea if this needs to be 8266 or is related to the chip



////////////////////////////////////////
//FastLED Setup, Commented out because// 
//Bitluni uses Adafruit Neopixel      //
////////////////////////////////////////
//#define NUM_LEDS    12
//#define DATA_PIN    D3
//#define CHIPSET     WS2812B
//#define COLOR_ORDER GRB 

const int LED_PIN = D3;
const int LED_COUNT = 16;

const int RF_OSC = 200;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

LedStates currentLedStates(strip);
LedStates targetLedStates(strip);
Fader<LedStates> ledFader(currentLedStates, targetLedStates);
PinStates currentPinStates;
PinStates targetPinStates;
Fader<PinStates> pinFader(currentPinStates, targetPinStates);

void handleRoot() {
  String message = "<html><head></head><body style='font-family: sans-serif; font-size: 12px'>Following functions are available:<br><br>";
  message += "<a href='/rainbow?fade=0'>/rainbow</a> a rainbow animation on LEDs<br>";
  
  message += "<a href='/hanukkah?fade=0'>/hanukkah</a> a step between white and blue<br>";
  message += "<a href='/christmas?fade=0'>/christmas</a> step between red and green<br>";  
  message += "<a href='/wave?r=255&g=32&b=10&fade=0'>/wave</a> red wave><br>";
  message += "<a href='/wave2?r=200&g=0&b=210&fade=0'>/wave2</a> pink wave><br>";
  message += "<a href='/wave4?r=240&g=75&b=20&fade=0'>/wave4</a> candle-ish <br>";
  message += "<a href='/wave3?r=0&g=32&b=255&fade=0'>/wave3</a> blue wave><br>";
  message += "<a href='/setleds?r=32&g=64&b=32&fade=0'>/setleds</a> sets LEDs to the color from arguments: r=<0..255> g=<0..255> b=<0..255><br>";
  message += "<a href='/wtest?fade=0'>/wtest</a> cycle green -> red -> green <br>";
  message += "<a href='/ledsoff?fade=0'>/ledsoff</a> turns off LEDs<br>";
  message += "Set LEDs takes arguments r=0-255, g=0-255, b=0-255 to make new colors.<br><br>";
  message += "You can adjust wave colors via RGB arguments as well.<br><br>";
  message += "Fade is in milliseconds.<br><br>";


  server.send(200, "text/html", message);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

int getArgValue(String name)
{
  for (uint8_t i = 0; i < server.args(); i++)
    if(server.argName(i) == name)
      return server.arg(i).toInt();
  return -1;
}

bool checkFadeAndSetLedFunction(LedFunction *f)
{
  int fade = getArgValue("fade");
  if(fade > -1)
  {
    targetLedStates.setFunction(f);
    ledFader.start(fade);
  }
  else
    currentLedStates.setFunction(f);  
}

void handleRf()
{
  const int pinNumbers[] = {D0, D1, D2, D3, D4, D5, D6, D7, D8};
  int pin = getArgValue("D");
  int t = getArgValue("t");
  if(t == -1) t = RF_OSC;
  int id = getArgValue("id");
  int ch = getArgValue("channel");
  int on = getArgValue("on");
  String out = "rf D";
  out += pin;
  out += " ";
  out += t;
  out += " ";
  out += id;
  out += " ";
  out += ch;
  out += " ";
  out += on;
  pinMode(pinNumbers[pin], OUTPUT);
  for(int i = 0; i < 5; i++)
    rfWriteCode(pinNumbers[pin], t, id, (1 << (ch + 1)) | (on > 0? 1: 0));
  server.send(200, "text/plain", out);  
}

// NETWORK: Static IP details...
IPAddress ip(192, 168, 0, 112);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.setAutoConnect(false);
      // Static IP Setup Info Here...
  WiFi.config(ip, gateway, subnet);
  //FastLED.addLEDS<Chipset, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS):
  //Start FastLED commented out because Neopixel is in use
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  colorWipe(strip.Color(255, 0, 0), 30); // red
  
  setup_wifi();
  /////////////
  //OTA Setup//
  /////////////
  ArduinoOTA.setPort(OTAport);
  //Hostname default to esp8266 - [chipIP]
  ArduinoOTA.setHostname(SENSORNAME);

  //No authentication by default
  ArduinoOTA.setPassword((const char*)OTApassword);

  ArduinoOTA.onStart([]() {
    Serial.println("Starting");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error = OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

//  Serial.println("Ready");
//  Serial.print("IP Adress: ");
//  Serial.println(WiFi.localIP());

}


////////////////////
//Start Setup Wifi//
////////////////////
void setup_wifi() {

  delay(10);
  //We start by connecting to a WiFi router
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }



  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  colorWipe(strip.Color(0, 255, 0), 30); // green

  //find it as http://lights.local
  /*if (MDNS.begin("lights")) 
  {
    Serial.println("MDNS responder started");
  }*/
  
  server.on("/", handleRoot);

  server.on("/rainbow", [](){
    server.send(200, "text/plain", "rainbow");
    checkFadeAndSetLedFunction(new RainbowFunction());
  });

  server.on("/christmas", [](){
    server.send(200, "text/plain", "christmas");
    checkFadeAndSetLedFunction(new christmas());
  });

  server.on("/hanukkah", [](){
    server.send(200, "text/plain", "hanukkah");
    checkFadeAndSetLedFunction(new hanukkah());
  });

  server.on("/wave", [](){
    server.send(200, "text/plain", "wave");
    WaveFunction *f = new WaveFunction();
    f->init(server);
    checkFadeAndSetLedFunction(f);
  });

  server.on("/wave4", [](){
    server.send(200, "text/plain", "wave4");
    wave4 *f = new wave4();
    f->init(server);
    checkFadeAndSetLedFunction(f);
  });

  server.on("/wave2", [](){
    server.send(200, "text/plain", "wave2");
    wave2 *f = new wave2();
    f->init(server);
    checkFadeAndSetLedFunction(f);
  });

  server.on("/wave3", [](){
    server.send(200, "text/plain", "wave3");
    wave3 *f = new wave3();
    f->init(server);
    checkFadeAndSetLedFunction(f);
  });

  server.on("/wtest", [](){
    server.send(200, "text/plain", "wtest");
    wtest *f = new wtest();
    f->init(server);
    checkFadeAndSetLedFunction(f);
  });

  server.on("/setleds", [](){
    server.send(200, "text/plain", "setleds, re-enter http with new RGB values");
    SimpleRGBFunction *f = new SimpleRGBFunction();
    f->init(server);
    checkFadeAndSetLedFunction(f);
  });

  server.on("/ledsoff", [](){
    server.send(200, "text/plain", "ledsoff");
    checkFadeAndSetLedFunction(new SimpleRGBFunction());
  });
  
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  

}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  
  ArduinoOTA.handle();
  server.handleClient();
  //MDNS.update();
  
  currentLedStates.render();
  if(ledFader.active)
    targetLedStates.render();
  if(!ledFader.fade())
    currentLedStates.commit();
  pinFader.fade();  


}
