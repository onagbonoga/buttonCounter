/*
  Button Count Arduino Code
  

  connections
  A0 --> Red LED
  A1 --> Green LED
  A2 --> Blue LED
  D5 --> button
*/
#include <SPI.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h" 
// constants won't change. They're used here to set pin numbers:
const int buttonPin = 5;    // the number of the pushbutton pin
int red = A0;
int green = A1;
int blue = A2;

// Variables will change:
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int ledState = 0;
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int delayTime = 600;

///////Wifi Set up
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

// Initialize the Wifi client library
WiFiClient client;

// server address ***********************************ENTER CORRECT SERVER ADDRESS*********************
//char server[] = "example.com";
IPAddress server(000,000,1,4);

unsigned long lastConnectionTime = 0;            // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 1L * 1000L; // delay between updates, in milliseconds
void setup() {
  pinMode(buttonPin, INPUT);

  // set initial LED state
  //set up analog pin modes
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
  analogWrite(red,0);
  analogWrite(green,0);
  analogWrite(blue,0);

    //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  // you're connected now, so print out the status and turn on LED's
  printWifiStatus();
  Serial.println("Push button to send requests");
}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  if (reading == HIGH){
    switch_action();
    // make http request 
    httpRequest();
  }
  
}

void switch_action(){
  delay(delayTime);

  ledState = !ledState;
  if (ledState){
    analogWrite(red,200);
    analogWrite(green,200);
    analogWrite(blue,200);
  }
  else {
    analogWrite(red,0);
    analogWrite(green,0);
    analogWrite(blue,0);
  }
  delay(delayTime);
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // close any connection before send a new request.
  // This will free the socket on the Nina module
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, 5000)) {//80 or 443 idk
    Serial.println("connecting...");
    // send the HTTP request:
    String json = "{\"id\":\"2\"}";

    // Build the request
    String request = "POST /buttonCount HTTP/1.1\r\n";
    request += "Content-Type: application/json\r\n";
    request += "Content-Length: " + String(json.length()) + "\r\n";
    request += "\r\n";
    request += json;
    Serial.println(request);
    client.println(request);
    client.println();

    // Wait for the server response
    while (!client.available()) {
      // Wait until data is available
      delay(5);
    }
    
    String responseString; // should only contain RGB values
    boolean responseStarted = false;
    while (client.available()) {
      char c = client.read();
      if (responseStarted)
        responseString += c;
      if (c == '*')
        responseStarted = true;
    }

    //Serial.println("Response: ");
    Serial.println(responseString);
    //Serial.println("******Response end");

    // note the time that the connection was made:
    lastConnectionTime = millis();
    
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
