/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

#include "FastLED.h"

// Update these with values suitable for your network.

const char* ssid = "TONnointernet";
const char* password = "theatreofnote";
const char* mqtt_server = "192.168.2.101";
const char* mqtt_username = "rthea";
const char* mqtt_password = "fresnels";
const int mqtt_port = 1883;
const char* state = "";
const char* oldState = "";
//#define SENSORNAME "strip"


///////////////
//OTA CREDENTIALS//
///////////////////
 #define SENSORNAME "plate1" //change this to whatever you want to call your device
#define OTApassword "plate1" //the password you will need to enter to upload remotely via the ArduinoIDE
int OTAport = 8266;


#define NUM_LEDS_PER_STRIP 16
CRGB bottomLeds[NUM_LEDS_PER_STRIP];
CRGB tempLED;

// NETWORK: Static IP details...
IPAddress ip(192, 168, 2, 105);
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);


//globals for fireplace
byte hue = 0;
byte counter=0;   

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

void fadeall() { for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) { bottomLeds[i].nscale8(500); } }

/*
void orb100(){
        hue++;
  delay(random(20,40));
  int i = random8(16); 
    bottomLeds[i] = CHSV(random(230,245), 255, random(30,160));
     FastLED.show();

    delay(random(50,75));

     for(int x=0; x<NUM_LEDS_PER_STRIP; x++){
    bottomLeds[x].fadeToBlackBy(.5);
     }
}
*/

void orb100(){
        hue++;
  delay(random(20,40));
  for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) { 
    bottomLeds[i] = CHSV(random(230,245), 255, random(30,160));
     FastLED.show();

    delay(random(50,75));

     for(int x=0; x<NUM_LEDS_PER_STRIP; x++){
    bottomLeds[x].fadeToBlackBy(.5);
     }
}
}

void orb50() { 
  static uint8_t hue = 0;
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    // Set the i'th led to red 
    bottomLeds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(50);
  }
  client.loop();

  // Now go in the other direction.  
  for(int i = (NUM_LEDS_PER_STRIP)-1; i >= 0; i--) {
    // Set the i'th led to red 
    bottomLeds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(50);
  }
}

void orb25(){

  static uint8_t hue = 0;
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    // Set the i'th led to red 
    bottomLeds[i] = CHSV(20,random (50,100), random (150,200));
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(50);
  }
}

void blackout() {
  for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    // set our current dot to red, green, and blue
    bottomLeds[i] = CRGB::Black;
    FastLED.show();
   
  }
  //checkState();
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    // Static IP Setup Info Here...
  WiFi.config(ip, gateway, subnet);
  
  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void checkState(){
  if (state =="orb100") {
    
    client.loop();
    orb100();
  }

  if (state =="orb50") {
    
    client.loop();
    orb50();
  }

  if (state =="orb25") {
    
    client.loop();
    orb25();
  }
  
  if (state == "blackout"){
    client.loop();
    blackout();
  }
  

}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
   state =  "orb100";

  }
  if ((char)payload[0] == '2') {
   state =  "orb50";

  } 
  if ((char)payload[0] == '3') {
   state =  "orb25";

  } 
  if ((char)payload[0] == '0') {
    state = "blackout";
}
  }

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if (client.connect(SENSORNAME, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world - plate1");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
FastLED.addLeds<NEOPIXEL, D7>(bottomLeds, NUM_LEDS_PER_STRIP);
  
  Serial.begin(9600);
  setup_wifi();

    //OTA SETUP
  ArduinoOTA.setPort(OTAport);
  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(SENSORNAME);

  // No authentication by default
  ArduinoOTA.setPassword((const char *)OTApassword);

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
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

    }


void loop() {

  ArduinoOTA.handle();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  /*
  if (state == "orb100"){
    orb100();
    }

     if (state == "orb50"){
    orb50();
    }
*/
     if (state == "orb25"){
    orb25();
    }
  else if (state == "blackout"){
    blackout();
    }
}
