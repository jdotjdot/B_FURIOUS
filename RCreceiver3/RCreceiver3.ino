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
#include <Arduino.h>

// *** WIFI SETTINGS ***
char ssid[] = NETWORK; // your network SSID (name)
char pass[] = PASSWORD; // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0; // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(23);
String clientMsg = "";
boolean alreadyConnected = false; // whether or not the client was connected previously

// *** MOTOR SETTINGS ***
int controlPin1 = 2; // on-off pin, need to send its output to #5
int controlPin2 = 3; // direction pin, need to sendits output to #4
int enablePin = 9;
// create some variables to hold values from your inputs
int onOffSwitchState = 0;  // current state of the On/Off switch
int previousOnOffSwitchState = 0; // previous position of the on/off switch
int directionSwitchState = 0;  // current state of the direction switch
int previousDirectionSwitchState = 0;  // previous state of the direction switch

int motorEnabled = 0; // Turns the motor on/off
int motorSpeed = 0; // speed of the motor
int motorDirection = 1; // current direction of the motor


void setup() {

  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);

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
// when the client sends the first byte, log to serial:
  if (client) {
    if (!alreadyConnected) {
      // clean out the input buffer:
      client.flush();
      Serial.println("Client connected");
      alreadyConnected = true;
    } 

    if (client.available() > 0) {
      // read the bytes incoming from the client, add to message:
      char thisChar = client.read();
      clientMsg += thisChar;
      // when message is complete, pass it to motor control functions
      if (thisChar == '\n') {
        Serial.println("Message from client: " + clientMsg);
        parseOnOffInput(enablePin, clientMsg);
        parseDirectionInput(controlPin1, controlPin2, clientMsg);
        // reset message
        clientMsg = "";
      }
    }
  }
}


void parseOnOffInput(int enablePin, String input) {
//  if (input.equals("\n")) {
  if (5 == 5) {
    Serial.println("ON!");
    digitalWrite(enablePin, HIGH);
  } else if (input.equals("\n")) {
    Serial.println("OFF!");
    digitalWrite(enablePin, LOW);
  } else {
    Serial.println("Neither ON nor OFF");
    Serial.print(input);
    Serial.print("Is there a line above this?");
  }
}

void parseDirectionInput(int controlPin1, int controlPin2, String input) {
//  if (input == "FORWARD") {
    if (5 == 5) {
    Serial.println("FORWARD!");
    digitalWrite(controlPin1, HIGH);
    digitalWrite(controlPin2, LOW);
  } else if (input == "BACKWARD") {
    Serial.println("BACKAWARD!");
    digitalWrite(controlPin1, LOW);
    digitalWrite(controlPin2, HIGH);
  } else {
    Serial.println("Neither FORWARD nor BACKWARD");
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
