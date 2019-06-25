#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
 
//-------- Customise these values -----------
const char* ssid = "Redmi 5";
const char* password = "venkat@123";
#define ONE_WIRE_BUS D2
OneWire oneWire(ONE_WIRE_BUS);


#define ORG "rwcoys"
#define DEVICE_TYPE "NODEMCU5060"
#define DEVICE_ID "DELL"
#define TOKEN "9381575060"
//-------- Customise the above values --------
 
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/anil/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

float temp;
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);

int tankPump=3;
int reserviorPump=2;
int trigPin = D8;//white
int echoPin = D1;//brown
long duration;
int distance;


void setup(){
Serial.begin(9600);
pinMode(tankPump,OUTPUT);
pinMode(reserviorPump,OUTPUT);
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
Serial.println();
Serial.print("Connecting to ");
Serial.print(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
} 
Serial.println("");
 
Serial.print("WiFi connected, IP address: ");
Serial.println(WiFi.localIP());
}

void loop(){
ultrasound();
pump();
}

void ultrasound(){
  
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance:");
Serial.println(distance);
delay(1000);
// publish
PublishData(distance);
}


void pump(){
if (distance > 10){
digitalWrite(tankPump,HIGH);
digitalWrite(reserviorPump,LOW);
}
else{
digitalWrite(tankPump,LOW);
}
if(distance < 6){
digitalWrite(reserviorPump,HIGH);
digitalWrite(tankPump,LOW);
}
else{
digitalWrite(reserviorPump,LOW);
 }
 }void PublishData(float distance){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 } String payload = "{\"d\":{\"distance\":";
  payload += distance;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
