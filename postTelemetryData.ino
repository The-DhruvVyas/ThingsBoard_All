/*
This is a example to post telemetry data on thingboard iot plateform 
work on http post request 
conpatiable with arduino ide
support board are :
esp32,arduino328p,adafruit feather , raspberry pi pico 2040
*/

#include <SPI.h>
#include <Ethernet.h>
#include <Arduino.h>
#include <ArduinoJson.h>



// Server and endpoint configuration
char server[] = "YOUR SERVER";  // ThingsBoard server eg. thingsboard.cloud
const int port = 80;          // HTTP port80. Use 443 for HTTPS (needs additional library for secure connection)

// ThingsBoard device token and device ID
String deviceID = "YOUR DEVICE ID;


//----------- ethernet setting------------------------------------------------
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Static IP configuration (if DHCP fails or in not use )
IPAddress ipeth(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);

//Create an IPAddress object
//IPAddress ip;
// Initialize the Ethernet client library
EthernetClient client;

void setup() {
  // Open serial communications and wait for the port to open:
  Serial.begin(9600);
  delay(1000);
  Serial.println("------------Start New ---------------- ");
  delay(100);
  Ethernet.init(17);
  ethernetBegin();
  delay(1000);

}

void loop() {
  postdata();
  delay(3000);//adjust delay as per your need 
}

//--------------------------postdata-----------(on thingsboard server and port )
void postdata() {
  // Construct the JSON string with the current humidity value
  String json_data = "{\"humidity\":";
  json_data += 60;
  json_data += ",\"temperature\":";
  json_data += 22;
  json_data += "}";

  // Attempt to connect to the server:
  if (client.connect(server, port)) {
    Serial.println("Connected");

    // Make an HTTP POST request:
    client.println("POST /api/v1/" + deviceId + "/telemetry HTTP/1.1");  // Replace /api/endpoint with your API endpoint

    client.println("Host: thingsboard.cloud");  // Replace with your server's address
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(json_data.length());
    client.println();
    client.println(json_data);

    // Wait for a response from the server
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
    }
  } else {
    // If you didn't get a connection to the server:
    Serial.println("Connection failed");
  }

  // Ensure the client is properly closed after each POST request
  client.stop();
}
//----------------------------------------ethernetBegin----------------------------------------------------------------------------------
void ethernetBegin(){
  // Start the Ethernet connection: static ip
  Serial.println("Initializing Ethernet...");
  Ethernet.begin(mac, ipeth, myDns);
  delay(1000);
  Serial.println("Connecting...");
   
  // Start the Ethernet connection: DHCP
  // if (Ethernet.begin(mac) == 0) {
  //   Serial.println("Failed to configure Ethernet using DHCP");
  //   // Try to configure using IP address instead of DHCP:
  //   Ethernet.begin(mac, ip);
  // }
  // // Give the Ethernet shield a second to initialize:
  // delay(1000);
  // Serial.println("Connecting...");
 // Print the IP address of connection 
  Serial.print("Ethernet IP address: ");
  Serial.println(Ethernet.localIP());
}

