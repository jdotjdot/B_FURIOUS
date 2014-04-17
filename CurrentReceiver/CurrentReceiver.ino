#include <SPI.h>
#include <WiFi.h>
#include <string.h>
#include <utility/server_drv.h>
#include <MemoryFree.h>

char ssid[] = "NETWORK"; // your network SSID (name)
char pass[] = "PASSWORD"; // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0; // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
WiFiServer server(80);
String clientMsg = "";
boolean alreadyConnected = false; // whether or not the client was connected previously

const int controlPin1 = 2;
const int controlPin2 = 3;
const int enablePin = 9;

int previousOnOffSwitchState = 0;
int previousDirectionSwitchState = 0;

int motorEnabled = 0;
int motorSpeed = 0;
int motorDirection = 1;

struct clientInput {
 boolean onOffInput;
 boolean directionInput;
 int motorSpeed;
};

void wifiSetup() {
    //Initialize serial and wait for port to open:
    Serial.begin(9600);
    Serial.println("********************************");
    Serial.println("\nInitial socket status (before connection):");
    Serial.print("Free memory: ");
    Serial.println(freeMemory());
    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present");
    // don't continue:
      while(true);
    }
    // attempt to connect to Wifi network:
    while ( status != WL_CONNECTED) {
      Serial.print("\nAttempting to connect to SSID: ");
      Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
      status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
      delay(10000);
    }

    // you're connected now, so print out the status:
    Serial.println(" ");  
    printWifiStatus();

    // start the server:
    server.begin();
    Serial.println("\nSocket status after server start:");
    Serial.print("First available socket: ");
    Serial.println(WiFi.getSocket());


}

void motorSetup() {
    pinMode(controlPin1, OUTPUT);
    pinMode(controlPin2, OUTPUT);
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, LOW);
}



struct clientInput parseClientInput(String inputMessage) {
    // inputMessage expected to be five chars:
    // First is for on/off: 1 or 0
    // Second for direction: 1 or 0
    // Last three for speed: 0 (stopped) to 255 (fast)

    struct clientInput holder;

    holder.onOffInput = inputMessage.substring(6,7).toInt();
    holder.directionInput = inputMessage.substring(7,8).toInt();
    holder.motorSpeed = inputMessage.substring(8,13).toInt();

    Serial.println(String("Motorspeed: ") + holder.motorSpeed);

    return holder;

}

void motorControl(boolean onOffInput, boolean directionInput, int motorSpeed) { // need to add arguments

    motorEnabled = onOffInput;
    motorDirection = directionInput;

    if(motorDirection == 1) {
        digitalWrite(controlPin1, HIGH);
        digitalWrite(controlPin2, LOW);
    } else {
        digitalWrite(controlPin1, LOW);
        digitalWrite(controlPin2, HIGH);
    }

    if(motorEnabled == 1) {
        Serial.println("motor enabled");
        analogWrite(enablePin, motorSpeed);
    } else {
        Serial.println("motor disabled");
        analogWrite(enablePin, 0);
    }

    previousDirectionSwitchState = directionInput;
    previousOnOffSwitchState = onOffInput;
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

void setup() {
    wifiSetup();
    motorSetup();
}

void loop() {
  
   status = WiFi.status();
   if (status == WL_CONNECTION_LOST | status == WL_CONNECT_FAILED | status == WL_DISCONNECTED) {
     Serial.print("WiFi Status: ");
     Serial.println(status);
     server.disconnect();
     WiFi.disconnect();
     wifiSetup();
   }

    // Wait for a new client
    WiFiClient client = server.available();

    //when client sends first byte
    if (client) {
        Serial.println("\n~~~~~New Client~~~~~");
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        while (client.connected()) {

          if (client.available() > 0) {
              // read bytes incoming from client
              char thisChar = client.read();

              clientMsg += thisChar;

              if (thisChar == '\n' && currentLineIsBlank) {
                  Serial.println("Message from client: " + clientMsg);

              struct clientInput parsedInput = parseClientInput(clientMsg);
              motorControl(parsedInput.onOffInput, parsedInput.directionInput,
                           parsedInput.motorSpeed);

                clientMsg = "";
                client.println("Received!");
		client.stop();
                break;
             }

             if (thisChar == '\n') {
                currentLineIsBlank = true;
              } else if (thisChar != '\r') {
                currentLineIsBlank = false;
             }
          }
          server.write("\0");
       }
    }
}
