#include <Arduino.h>
#include <WiFi.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"

// Uncomment the following line to enable serial debug output
//#define ENABLE_DEBUG

#ifdef ENABLE_DEBUG
       #define DEBUG_ESP_PORT Serial
       #define NODEBUG_WEBSOCKETS
       #define NDEBUG
#endif 

#define WIFI_SSID         "your wifi name "      //Enter WiFi Name
#define WIFI_PASS         "wifi password"      //Enter WiFi Password
#define APP_KEY           "xxxxxxxxxxxxxxxxxxxxxxxxx"                  //Enter APP-KEY 
#define APP_SECRET        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"     //Enter APP-SECRET

//Enter the device IDs here
#define device_ID_1   "xxxxxxxxxxxxxxxxxxxxxxxx"  //SWITCH 1 ID
#define device_ID_2   "xxxxxxxxxxxxxxxxxxxxxxxx"  //SWITCH 2 ID
#define device_ID_3   "xxxxxxxxxxxxxxxxxxxxxxxx"  //SWITCH 3 ID

// Define the GPIO connected with Relays
#define LockRelayPin   <Pin-1>   // Relay for Lock
#define BulbRelayPin1  <Pin-2>    // Relay for Bulb 1
#define BulbRelayPin2  <Pin-3>    // Relay for Bulb 2

#define wifiLed   2    // LED to indicate WiFi connection

#define BAUD_RATE   9600

typedef struct {      // Struct for the std::map below
  int relayPIN;
} deviceConfig_t;

// Mapping device IDs to relay pins
std::map<String, deviceConfig_t> devices = {
    {lockDevice_ID, { LockRelayPin }},
    {bulbDevice_ID_1, { BulbRelayPin1 }},
    {bulbDevice_ID_2, { BulbRelayPin2 }},
};

bool onPowerState(String deviceId, bool &state) {
  Serial.printf("%s: %s\r\n", deviceId.c_str(), state ? "on" : "off");
  int relayPIN = devices[deviceId].relayPIN; // Get the relay pin for corresponding device
  digitalWrite(relayPIN, !state);            // Set the new relay state
  return true;
}

void setupRelays() { 
  for (auto &device : devices) {            // For each device (relay)
    int relayPIN = device.second.relayPIN;  // Get the relay pin
    pinMode(relayPIN, OUTPUT);              // Set relay pin to OUTPUT
    digitalWrite(relayPIN, HIGH);           // Initialize relay to OFF state
  }
}

void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  digitalWrite(wifiLed, HIGH);
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

void setupSinricPro() {
  for (auto &device : devices) {
    const char *deviceId = device.first.c_str();
    SinricProSwitch &mySwitch = SinricPro[deviceId];
    mySwitch.onPowerState(onPowerState);
  }

  SinricPro.begin(APP_KEY, APP_SECRET);
  SinricPro.restoreDeviceStates(true);
}

void setup() {
  Serial.begin(BAUD_RATE);

  pinMode(wifiLed, OUTPUT);
  digitalWrite(wifiLed, LOW);

  setupRelays();
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}

























// // ESP32 Google Assistant + Alexa + Manual HomeAutomation with Sinric Pro


// #include <Arduino.h>                          //ArduinoJson Library: https://github.com/bblanchon/ArduinoJson
// #include <WiFi.h>
// #include "SinricPro.h"                        //SinricPro Library: https://sinricpro.github.io/esp8266-esp32-sdk/
// #include "SinricProSwitch.h"
// #include <map>

// // Uncomment the following line to enable serial debug output
// //#define ENABLE_DEBUG

// #ifdef ENABLE_DEBUG
//        #define DEBUG_ESP_PORT Serial
//        #define NODEBUG_WEBSOCKETS             //arduinoWebSockets Library: https://github.com/Links2004/arduinoWebSockets
//        #define NDEBUG
// #endif 

// #define WIFI_SSID         "your wifi name "      //Enter WiFi Name
// #define WIFI_PASS         "wifi password"      //Enter WiFi Password
// #define APP_KEY           "xxxxxxxxxxxxxxxxxxxxxxxxx"                  //Enter APP-KEY 
// #define APP_SECRET        "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"     //Enter APP-SECRET

// //Enter the device IDs here
// #define device_ID_1   "xxxxxxxxxxxxxxxxxxxxxxxx"  //SWITCH 1 ID
// #define device_ID_2   "xxxxxxxxxxxxxxxxxxxxxxxx"  //SWITCH 2 ID
// #define device_ID_3   "xxxxxxxxxxxxxxxxxxxxxxxx"  //SWITCH 3 ID


// // define the GPIO connected with Relays and switches
// #define RelayPin1 15  //D15
// #define RelayPin2 2  //D2
// #define RelayPin3 4  //D4

// #define SwitchPin1 23  //D23
// #define SwitchPin2 22  //D22
// #define SwitchPin3 21  //D21


// #define wifiLed   2   //D2

// //uncomment the following line if you use Push Buttons to toggle Relays
// //#define TACTILE_BUTTON 1

// #define BAUD_RATE   9600

// #define DEBOUNCE_TIME 250

// typedef struct {      // struct for the std::map below
//   int relayPIN;
//   int flipSwitchPIN;
// } deviceConfig_t;


// std::map<String, deviceConfig_t> devices = {
//     //{deviceId, {relayPIN,  flipSwitchPIN}}
//     {device_ID_1, {  RelayPin1, SwitchPin1 }},
//     {device_ID_2, {  RelayPin2, SwitchPin2 }},
//     {device_ID_3, {  RelayPin3, SwitchPin3 }},
         
// };

// typedef struct {      // struct for the std::map below
//   String deviceId;
//   bool lastFlipSwitchState;
//   unsigned long lastFlipSwitchChange;
// } flipSwitchConfig_t;

// std::map<int, flipSwitchConfig_t> flipSwitches;    // this map is used to map flipSwitch PINs to deviceId and handling debounce and last flipSwitch state checks
//                                                   // it will be setup in "setupFlipSwitches" function, using informations from devices map

// void setupRelays() { 
//   for (auto &device : devices) {           // for each device (relay, flipSwitch combination)
//     int relayPIN = device.second.relayPIN; // get the relay pin
//     pinMode(relayPIN, OUTPUT);             // set relay pin to OUTPUT
//     digitalWrite(relayPIN, HIGH);
//   }
// }

// void setupFlipSwitches() {
//   for (auto &device : devices)  {                     // for each device (relay / flipSwitch combination)
//     flipSwitchConfig_t flipSwitchConfig;              // create a new flipSwitch configuration

//     flipSwitchConfig.deviceId = device.first;         // set the deviceId
//     flipSwitchConfig.lastFlipSwitchChange = 0;        // set debounce time
//     flipSwitchConfig.lastFlipSwitchState = false;     // set lastFlipSwitchState to false (LOW)--

//     int flipSwitchPIN = device.second.flipSwitchPIN;  // get the flipSwitchPIN

//     flipSwitches[flipSwitchPIN] = flipSwitchConfig;   // save the flipSwitch config to flipSwitches map
//     pinMode(flipSwitchPIN, INPUT_PULLUP);                   // set the flipSwitch pin to INPUT
//   }
// }

// bool onPowerState(String deviceId, bool &state)
// {
//   Serial.printf("%s: %s\r\n", deviceId.c_str(), state ? "on" : "off");
//   int relayPIN = devices[deviceId].relayPIN; // get the relay pin for corresponding device
//   digitalWrite(relayPIN, !state);             // set the new relay state
//   return true;
// }

// void handleFlipSwitches() {
//   unsigned long actualMillis = millis();                                          // get actual millis
//   for (auto &flipSwitch : flipSwitches) {                                         // for each flipSwitch in flipSwitches map
//     unsigned long lastFlipSwitchChange = flipSwitch.second.lastFlipSwitchChange;  // get the timestamp when flipSwitch was pressed last time (used to debounce / limit events)

//     if (actualMillis - lastFlipSwitchChange > DEBOUNCE_TIME) {                    // if time is > debounce time...

//       int flipSwitchPIN = flipSwitch.first;                                       // get the flipSwitch pin from configuration
//       bool lastFlipSwitchState = flipSwitch.second.lastFlipSwitchState;           // get the lastFlipSwitchState
//       bool flipSwitchState = digitalRead(flipSwitchPIN);                          // read the current flipSwitch state
//       if (flipSwitchState != lastFlipSwitchState) {                               // if the flipSwitchState has changed...
// #ifdef TACTILE_BUTTON
//         if (flipSwitchState) {                                                    // if the tactile button is pressed 
// #endif      
//           flipSwitch.second.lastFlipSwitchChange = actualMillis;                  // update lastFlipSwitchChange time
//           String deviceId = flipSwitch.second.deviceId;                           // get the deviceId from config
//           int relayPIN = devices[deviceId].relayPIN;                              // get the relayPIN from config
//           bool newRelayState = !digitalRead(relayPIN);                            // set the new relay State
//           digitalWrite(relayPIN, newRelayState);                                  // set the trelay to the new state

//           SinricProSwitch &mySwitch = SinricPro[deviceId];                        // get Switch device from SinricPro
//           mySwitch.sendPowerStateEvent(!newRelayState);                            // send the event
// #ifdef TACTILE_BUTTON
//         }
// #endif      
//         flipSwitch.second.lastFlipSwitchState = flipSwitchState;                  // update lastFlipSwitchState
//       }
//     }
//   }
// }

// void setupWiFi()
// {
//   Serial.printf("\r\n[Wifi]: Connecting");
//   WiFi.begin(WIFI_SSID, WIFI_PASS);

//   while (WiFi.status() != WL_CONNECTED)
//   {
//     Serial.printf(".");
//     delay(250);
//   }
//   digitalWrite(wifiLed, HIGH);
//   Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
// }

// void setupSinricPro()
// {
//   for (auto &device : devices)
//   {
//     const char *deviceId = device.first.c_str();
//     SinricProSwitch &mySwitch = SinricPro[deviceId];
//     mySwitch.onPowerState(onPowerState);
//   }

//   SinricPro.begin(APP_KEY, APP_SECRET);
//   SinricPro.restoreDeviceStates(true);
// }

// void setup()
// {
//   Serial.begin(BAUD_RATE);

//   pinMode(wifiLed, OUTPUT);
//   digitalWrite(wifiLed, LOW);

//   setupRelays();
//   setupFlipSwitches();
//   setupWiFi();
//   setupSinricPro();
// }

// void loop()
// {
//   SinricPro.handle();
//   handleFlipSwitches();
// }
