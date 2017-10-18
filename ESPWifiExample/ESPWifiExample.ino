/*
Released under Creative Commons Attribution 4.0
by bitluni 2016
https://creativecommons.org/licenses/by/4.0/
Attribution means you can use it however you like as long you
mention that it's base on my stuff.
I'll be pleased if you'd do it by sharing http://youtube.com/bitlunislab
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <Adafruit_NeoPixel.h>

#include "PinStates.h"
#include "LedStates.h"
#include "Fader.h"
#include "RainbowFunction.h"
#include "SimpleRGBFunction.h"
#include "WaveFunction.h"
#include "RF.h"
#include "wtest.h"
#include "wave2.h"
#include "wave4.h"
#include "wave3.h"

const char* ssid = "WiFi-Network";
const char* password = "********";

ESP8266WebServer server(80);

const int LED_PIN = D3;
const int LED_COUNT = 27;

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
  message += "<a href='/rainbow?fade=3000'>/rainbow</a> a rainbow animation on LEDs<br>";
  message += "<a href='/wave?r=255&g=32&b=10&fade=5000'>/wave</a> red wave><br>";
  message += "<a href='/wave2?r=0&g=255&b=10&fade=5000'>/wave2</a> green wave><br>";
  message += "<a href='/wave4?r=0&g=255&b=10&fade=5000'>/wave4</a> green blinky thing<br>";
  message += "<a href='/wave3?r=0&g=32&b=255&fade=5000'>/wave3</a> blue wave><br>";
  message += "<a href='/setleds?r=32&g=64&b=32&fade=1000'>/setleds</a> sets LEDs to the color from arguments: r=<0..255> g=<0..255> b=<0..255><br>";
  message += "<a href='/wtest?fade=5000'>/wtest</a> cycle green -> red -> green <br>";
  message += "<a href='/ledsoff?fade=500'>/ledsoff</a> turns off LEDs<br>";
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
IPAddress ip(192, 168, 0, 102);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

void setup(void){
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

    // Static IP Setup Info Here...
WiFi.config(ip, gateway, subnet);

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

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
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop(void)
{
  server.handleClient();
  //MDNS.update();
  currentLedStates.render();
  if(ledFader.active)
    targetLedStates.render();
  if(!ledFader.fade())
    currentLedStates.commit();
  pinFader.fade();
}
