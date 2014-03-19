/*
Chat Server
A simple server that distributes any incoming messages to all
connected clients. To use telnet to your device's IP address and type.
You can see the client's input in the serial monitor as well.
This example is written for a network using WPA encryption. For
WEP or WPA, change the Wifi.begin() call accordingly.
Circuit:
WiFi shield attached
created 18 Dec 2009
by David A. Mellis
modified 31 May 2012
by Tom Igoe
*/
#include <SPI.h>
#include <WiFi.h>
#include <string.h>
char ssid[] = NETWORK; // your network SSID (name)
char pass[] = PASSWORD; // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0; // your network key Index number (needed only for WEP)
int on_off = 2; // on-off pin, need to send its output to #5
int direction = 3; // direction pin, need to send its output to #4
int status = WL_IDLE_STATUS;
WiFiServer server(23);
String clientMsg = "";
boolean alreadyConnected = false; // whether or not the client was connected previously
void setup() {

  pinMode(on_off, OUTPUT);
  pinMode(direction, OUTPUT);


//Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
; // wait for serial port to connect. Needed for Leonardo only
}
// check for the presence of the shield:
if (WiFi.status() == WL_NO_SHIELD) {
  Serial.println("WiFi shield not present");
// don't continue:
  while(true);
}
// attempt to connect to Wifi network:
while ( status != WL_CONNECTED) {
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
// Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  status = WiFi.begin(ssid, pass);
// wait 10 seconds for connection:
  delay(10000);
}
// start the server:
server.begin();
// you're connected now, so print out the status:
printWifiStatus();
}
void loop() {
// wait for a new client:
  WiFiClient client = server.available();
// when the client sends the first byte, say hello:
  if (client) {

/* if (!alreadyConnected) {
  // clead out the input buffer:
  client.flush();
  Serial.println("We have a new client");
  client.println("Hello, client!");
  alreadyConnected = true;
} */

  if (client.available() > 0) {
  // read the bytes incoming from the client:
    char thisChar = client.read();

  // echo the bytes back to the client:
  //server.write(thisChar);
  // echo the bytes to the server as well:
  //Serial.write(thisChar);

    clientMsg += thisChar;

    if (thisChar == '\n') {
      Serial.println("Message from client: " + clientMsg);
      parseLightInput(on_off, clientMsg);
      clientMsg = "";
    }
  }
}
}

void parseLightInput(int light, String input) {
  if (input == "ON") {
    digitalWrite(light, HIGH);
  } else if (input == "OFF") {
    digitalWrite(light, LOW);
  } else {
    Serial.println("Neither ON nor OFF");
    Serial.println(input);
  }
}

void printWifiStatus() {
// print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
// print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("Local IP Address: ");
  Serial.println(ip);
// print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
