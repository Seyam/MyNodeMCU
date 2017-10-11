/*                  (____/ 
 * Use NodeMCU to drive DHT11 and send temperature/humidity value to MQTT server
 * Tutorial URL http://osoyoo.com/2016/11/24/use-nodemcu-to-send-temperaturehumidity-data-to-mqtt-iot-broker/
 * CopyRight John Yu
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <dht.h>
dht DHT;

const int ledPin1 = D1; // ONBOARD LED WORKS OPPOSITE
const int ledPin2 = D2; // Pin
const int ledPin3 = D3; // 


// Define NodeMCU D3 pin to as temperature data pin of  DHT11
#define DHT11_PIN D3

// Update these with values suitable for your network.
const char* ssid = "DataSoft_WiFi";
const char* password = "support123";
//const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* mqtt_server = "182.48.84.180";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin1,OUTPUT);
  pinMode(ledPin2,OUTPUT);
  pinMode(ledPin3,OUTPUT);
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
//  int chk = DHT.read11(DHT11_PIN);
//  Serial.print(" Starting Humidity: " );
//  Serial.print(DHT.humidity, 1);
//  Serial.println('%');
//  Serial.print(" Starting Temparature ");
//  Serial.print(DHT.temperature, 1);
//  Serial.println('C');
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  
  long now = millis();
  // read DHT11 sensor every 2 seconds
  if (now - lastMsg > 5000) {
     lastMsg = now;
     //int chk = DHT.read11(DHT11_PIN);
     String msg="real time temperature: ";
     msg = msg+ DHT.temperature;
     msg = msg+" C ;real time Humidity: " ;
     msg = msg+DHT.humidity ;
     msg = msg+"%";
//     msg=msg+"1";  doesn't work
//      msg = "1";
     char message[58];
     msg.toCharArray(message,58);
     Serial.println(message);
     //publish sensor data to MQTT broker
    //client.publish("Seyam/data", message);
    
  }
}





void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Command is : ");
  Serial.print(topic);
  Serial.print(": ");
  int p =(char)payload[0]-'0';
  Serial.println(p);

  
  // if MQTT comes a 0 message, show humidity
  if(p==0) 
  {
    digitalWrite(ledPin1, LOW); // 
    Serial.println("ledPin0=OFF");
  } 
  // if MQTT comes a 1 message, show temperature
  if(p==1)
  {
   digitalWrite(ledPin1, HIGH);
   Serial.println("ledPin0=ON");
    
  }
    if(p==2)
  {
   digitalWrite(ledPin2, LOW);
   Serial.println("ledPin1=OFF");
    
  }

    if(p==3)
  {
   digitalWrite(ledPin2, HIGH);
   Serial.println("ledPin1=ON");
    
  }
      if(p==4)
  {
   digitalWrite(ledPin3, LOW);
   Serial.println("ledPin1=ON");
    
  }
      if(p==5)
  {
   digitalWrite(ledPin3, HIGH);
   Serial.println("ledPin1=ON");
  }
  Serial.println();
} //end callback







void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("ds/smartlife/app");
      Serial.println("Subscribed to the topic:    Seyam/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end reconnect()

