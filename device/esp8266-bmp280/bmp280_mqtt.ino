#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <ESP8266WiFi.h>  // Enables the ESP8266 to connect to the local network (via WiFi)
#include <PubSubClient.h> // Connect and publish to the MQTT broker

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

// WiFi
const char* ssid = "TP-Link_F11C";
const char* wifi_password = "";

// MQTT
const char* mqtt_server = "192.168.0.161";
const char* mqtt_topic = "room2"; 
const char* clientID = "esp8266"; 
const int mqtt_port = 11883;

// BMP
const int bmp_address = 0x76;

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
PubSubClient client(wifiClient); 

Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

// Custom function to connet to the MQTT broker via WiFi
void connect_MQTT(){
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  client.setServer(mqtt_server, mqtt_port);

  // Debugging - Output the IP Address of the ESP8266
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to MQTT Broker
  // client.connect returns a boolean value to let us know if the connection was successful.
  // If the connection is failing, make sure you are using the correct MQTT Username and Password (Setup Earlier in the Instructable)
  if (client.connect(clientID)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("Start."));

  if (!bmp.begin(bmp_address)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  connect_MQTT();
}

void loop() {
  float t = bmp.readTemperature();
  float p = bmp.readPressure();
  
  Serial.print(F("Temperature = "));
  Serial.print(t);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(p);
  Serial.println(" Pa");

  Serial.println();

  String message = "temperature,pressure\n" + String(t) + "," + String(p);

  // PUBLISH to the MQTT Broker (topic = Temperature, defined at the beginning)
  if (client.publish(mqtt_topic, message.c_str())) {
    Serial.println("Message sent!");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
    Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(mqtt_topic, message.c_str());
  }

  delay(1000 * 60);      
}
