#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <PubSubClient.h>

const char* ssid = "networkID";
const char* password = "****";
const char* mqtt_server = "192.168.0.1";
const char* mqtt_username = "user";
const char* mqtt_password = "****";
const int mqtt_port = 1883;
const char* state = "";

#define SENSORNAME "strip"

///////////////////
//OTA CREDENTIALS//
///////////////////
#define sensorname "orb1"
#define OTApassword "orb1"
int OTAport = 8266;

//insert neopixel setup here
const int LED_PIN = D7;
const int LED_COUNT = 16;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// NETWORK: Static IP details...
IPAddress ip(192, 168, 0, 103);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiClient espClient;
PubSubClient client(espClient);

void orb100(){}
void orb50(){}
void orb25(){}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
  client.loop();
}

void blackout(){}


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

    if (state =="colorWipe") {
    
    client.loop();
    colorWipe(127,127);
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
    if ((char)payload[0] == '4') {
   state =  "colorWipe";

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
      client.publish("outTopic", "hello world - orb1");
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



void setup(){

  Serial.begin(9600);
  setup_wifi();

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  colorWipe(strip.Color(225, 160, 75), 50); // warm white
  
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
  if (state == "orb100"){
    orb100();
    }

     if (state == "orb50"){
    orb50();
    }

     if (state == "orb25"){
    orb25();
    }

       if (state == "colorWipe"){
    colorWipe(127,127);
    }
  else if (state == "blackout"){
    blackout();
    }
}


