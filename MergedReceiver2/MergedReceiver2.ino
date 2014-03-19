
#include <SPI.h>
#include <WiFi.h>
#include <string.h>

char ssid[] = NETWORK; // your network SSID (name)
char pass[] = PASSWORD; // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0; // your network key Index number (needed only for WEP)
int on_off = 2; // on-off pin, need to send its output to #5
int direction = 3; // direction pin, need to send its output to #4
int status = WL_IDLE_STATUS;
//WiFiServer server(23);
WiFiServer server(80);
String clientMsg = "";
boolean alreadyConnected = false; // whether or not the client was connected previously

const int controlPin1 = 2;
const int controlPin2 = 3;
const int enablePin = 9;
const int potPin = A0;

int onOffSwitchState = 0;
int previousOnOffSwitchState = 0;
int directionSwitchState = 0;
int previousDirectionSwitchState = 0;

int motorEnabled = 0;
int motorSpeed = 0;
int motorDirection = 1;

/* typedef struct clientInput_t {
  boolean onOffInput;
  boolean directionInput;
  int motorSpeed;
} clientInput; */

struct clientInput {
 boolean onOffInput;
 boolean directionInput;
 int motorSpeed;
};

void wifiSetup() {
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
      delay(5000);
    }
    // start the server:
    server.begin();
    // you're connected now, so print out the status:
    printWifiStatus();


}

void motorSetup() {
    pinMode(controlPin1, OUTPUT);
    pinMode(controlPin2, OUTPUT);
    pinMode(enablePin, OUTPUT);

    digitalWrite(enablePin, LOW);
}



struct clientInput parseClientInput(String inputMessage) {
    // inputMessage expected to be two chars:
    // First is for on/off: 1 or 0
    // Second for direction: 1 or 0

    //clientInput* holder = malloc(sizeof(clientInput));

    struct clientInput holder;

    holder.onOffInput = inputMessage.substring(6,7).toInt();
    holder.directionInput = inputMessage.substring(7,8).toInt();
    holder.motorSpeed = inputMessage.substring(8,13).toInt();

    Serial.println(String("Motorspeed: ") + holder.motorSpeed);

    return holder;

}

/*
boolean (*parseClientInput(String inputMessage))[2] {

    // inputMessage expected to be two chars:
    // First is for on/off: 1 or 0
    // Second for direction: 1 or 0

    boolean (*returnArray)[2] = {atoi(inputMessage.substring(0,1)),
                                 atoi(inputMessage.substring(1,2))};

    return returnArray;

}

return_struct *struct_thing = f(7);
mybool = struct_thing->bool1;
free(struct_thing);

typedef struct return_struct_t {
  boolean bool1;
  boolean bool2;
  float f;
} return_struct;

return_struct *f (int n) {
  return_struct* thingy = malloc(sizeof(return_struct));
  thingy->bool1 = true;
  thingy->bool2 = false;
  // do stuff
  return thingy
}
 */

void motorControl(boolean onOffInput, boolean directionInput, int motorSpeed) {  // need to add arguments

/*    onOffSwitchState = digitalRead(onOffSwitchStatePin);
    delay(1);
    directionSwitchState = digitalRead(directionSwitchPin);
    motorSpeed = analogRead(potPin)/4; */

    onOffSwitchState = onOffInput;
    directionSwitchState = directionInput;
    //motorSpeed = analogRead(potPin)/4;
/*
    if(onOffSwitchState != previousOnOffSwitchState) {
        if(onOffSwitchState == HIGH) {
            motorEnabled = !motorEnabled;
        }
    }

    if(directionSwitchState != previousDirectionSwitchState) {
        if(directionSwitchState == HIGH) {
            motorDirection = !motorDirection;
        }
    }
    */
    motorDirection = directionSwitchState;
    motorEnabled = onOffSwitchState;

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

    previousDirectionSwitchState = directionSwitchState;
    previousOnOffSwitchState = onOffSwitchState;
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
     Serial.println(status);
     //WiFiServer server(80);
     WiFi.disconnect();
     wifiSetup();
   }

    // Wait for a new client
    WiFiClient client = server.available();

    //Serial.println(WiFiDrv.getConnectionStatus());
    //Serial.println(WiFi.status());

    //when client sends first byte

    if (client) {
        Serial.println("New Client");
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
        while (client.connected()) {

          if (client.available() > 0) {
              // read bytes incoming from client
              char thisChar = client.read();

              clientMsg += thisChar;

              if (thisChar == '\n' && currentLineIsBlank) {
                  Serial.println("Message from client: " + clientMsg);
                  //msgArray = parseClientInput(clientMsg);
                  //motorControl(msgArray[0], msgArray[1]);

                //if (clientMsg.startsWith("0") | clientMsg.startsWith("1")) {
                    struct clientInput parsedInput = parseClientInput(clientMsg);
                    motorControl(parsedInput.onOffInput, parsedInput.directionInput,
                                 parsedInput.motorSpeed);
                //}

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

