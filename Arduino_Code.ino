int tempPin= A0;
double temp;
double temp_in_C;
   
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "WiFi-AE41-5G";        // your network SSID (name)
char pass[] = "abc";    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.mqttdashboard.com";
int        port     = 1883;
const char topic[]  = "temperature_data";

//set interval for sending messages (milliseconds)
const long interval = 8000;
unsigned long previousMillis = 0;

int count = 0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }
   
  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}

void loop() {
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  //mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    temp = analogRead(tempPin);    //reading value from A0
    temp_in_C = (double)temp / 1024; //getting percentage of input reading
    temp_in_C = temp_in_C*5;     //multiplication by 5 for getting voltage
    temp_in_C = temp_in_C -0.5;  //subtracted the offset
    temp_in_C = temp_in_C*100;   //converting to degrees
  
     //printing temperature value(Celcius degree)
    Serial.print("Temp: ");
    Serial.print(temp_in_C); 
    Serial.println(" Celsius");

    Serial.print("Sending message to topic: "); 
    Serial.println(topic);
     Serial.print("Temperature data sent: ");
    Serial.println(temp_in_C);
    
    mqttClient.beginMessage(topic);
    mqttClient.print(temp_in_C);
    mqttClient.endMessage();
    
  }
}



   
