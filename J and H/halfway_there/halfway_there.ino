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

#include "FastLED.h"

// Update these with values suitable for your network.

const char* ssid = "DoNotConnect";
const char* password = "9498786485";
const char* mqtt_server = "192.168.0.19";
const char* mqtt_username = "rthea";
const char* mqtt_password = "fresnels";
const int mqtt_port = 1883;

#define SENSORNAME "strip"


#define NUM_LEDS_PER_STRIP 9
CRGB leftWallLeds[NUM_LEDS_PER_STRIP];
CRGB rightWallLeds[NUM_LEDS_PER_STRIP];
CRGB bottomLeds[NUM_LEDS_PER_STRIP];
CRGB tempLED;

//globals for fireplace
byte hue = 0;
byte counter=0;   

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;


void fireplace(){
 // while (true){
        hue++;
  delay(random(20,40));
  int i = random8(8); 
    leftWallLeds[i] = CHSV(random(1,15), 255, random(30,160));
    rightWallLeds[i] = CHSV(random(115,125), 255, random(30,160));
    bottomLeds[i] = CHSV(random(230,245), 255, random(30,160));
     FastLED.show();

    delay(random(50,75));

     for(int x=0; x<NUM_LEDS_PER_STRIP; x++){
    leftWallLeds[x].fadeToBlackBy(.5);
     }
     for(int x=0; x<NUM_LEDS_PER_STRIP; x++){
    rightWallLeds[x].fadeToBlackBy(.5);
     }
     for(int x=0; x<NUM_LEDS_PER_STRIP; x++){
    bottomLeds[x].fadeToBlackBy(.5);
     }
    
 // }
}


void blackout() {
  for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    // set our current dot to red, green, and blue
    leftWallLeds[i] = CRGB::Black;
    rightWallLeds[i] = CRGB::Black;
    bottomLeds[i] = CRGB::Black;
    FastLED.show();
   
  }
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

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
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
  while ((char)payload[0] == '1') {
 
    fireplace();
    
  } 
while ((char)payload[0] != '1') {
blackout();
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
      client.publish("outTopic", "hello world");
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
  FastLED.addLeds<NEOPIXEL, D3>(leftWallLeds, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, D5>(rightWallLeds, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, D7>(bottomLeds, NUM_LEDS_PER_STRIP);
  
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

    }


void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  
}
