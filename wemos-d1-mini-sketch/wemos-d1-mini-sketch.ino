/*
 * WebSocketClientSocketIO.ino
 *
 *  Created on: 06.06.2016
 *  
 *  Using code sample from:
 *  https://github.com/Links2004/arduinoWebSockets
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ArduinoJson.h>

#include <WebSocketsClient.h>
#include <SocketIOclient.h>

#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;
SocketIOclient socketIO;

// #define USE_SERIAL Serial1
#define USE_SERIAL Serial

const char *ssid = "SSID";
const char *pass = "PASS";

const byte interruptPin = D3;
const byte ledPin = D4;

volatile byte updateEvent = LOW;

ICACHE_RAM_ATTR void doorDetect() {
  Serial.println("Interrupt called!");
  updateEvent = HIGH;
}

void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case sIOtype_DISCONNECT:
            USE_SERIAL.printf("[IOc] Disconnected!\n");
            break;
        case sIOtype_CONNECT:
            USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);
            updateEventCallback();
            break;
        case sIOtype_EVENT:
            USE_SERIAL.printf("[IOc] get event: %s\n", payload);
            // set updateEvent flag to HIGH if event is "poll"
            if (!strncmp((char *)payload, "[\"poll\"]", length))
              updateEvent = HIGH;
            break;
        case sIOtype_ACK:
            USE_SERIAL.printf("[IOc] get ack: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_ERROR:
            USE_SERIAL.printf("[IOc] get error: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_BINARY_EVENT:
            USE_SERIAL.printf("[IOc] get binary: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_BINARY_ACK:
            USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
            hexdump(payload, length);
            break;
    }
}

void updateEventCallback() {
    updateEvent = LOW;
    bool sensorVal = digitalRead(interruptPin);
    digitalWrite(ledPin, sensorVal);
    String doorStatus = sensorVal ? "doorOpened" : "doorClosed";
    
    // creat JSON message for Socket.IO (event)
    DynamicJsonDocument doc(1024);
    JsonArray array = doc.to<JsonArray>();
    
    // add event name
    // Hint: socket.on('event_name', ....
    array.add(doorStatus);
  
    // JSON to String (serializion)
    String output;
    serializeJson(doc, output);
  
    // Send event        
    socketIO.sendEVENT(output);
  
    // Print JSON for debugging
    USE_SERIAL.print("[socket.io] Sent: ");
    USE_SERIAL.println(output);
}

void setup() {
    // USE_SERIAL.begin(921600);
    USE_SERIAL.begin(115200);

    //configure interrupt pin as an input and enable the internal pull-up resistor
    pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), doorDetect, CHANGE);
    
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);

    //Serial.setDebugOutput(true);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }

    // disable AP
    if(WiFi.getMode() & WIFI_AP) {
        WiFi.softAPdisconnect(true);
    }

    WiFiMulti.addAP(ssid, pass);

    //WiFi.disconnect();
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    String ip = WiFi.localIP().toString();
    USE_SERIAL.printf("[SETUP] WiFi Connected %s\n", ip.c_str());

    // server address, port and URL
    socketIO.begin("raspberrypi4.local", 80);

    // event handler
    socketIO.onEvent(socketIOEvent);
}

unsigned long messageTimestamp = 0;

void loop() {
    socketIO.loop();

    if (updateEvent)
      updateEventCallback();
}
