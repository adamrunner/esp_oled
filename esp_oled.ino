#include "ESP8266WiFi.h"

// OLED Library
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WebServer.h>
#include <Fonts/FreeSans24pt7b.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 5
#define OLED_SDA 13 //MOSI

#define OLED_SCL 14 //CLK

#define OLED_DC 2 //DC

#define OLED_CS 12 // no need of connecting anything to this pin, just use some pin number

#define OLED_RESET 0 //RES

const char* ssid = "home-network";
const char* password = "tacocat1234";
const long pollTempInterval = 1000;
String displayedTempF = "n/a";
unsigned long previousMillis;

// this is the constructor to call the OLED display where we specify exact pin numbers.

Adafruit_SSD1306 display(OLED_SDA,OLED_SCL, OLED_DC, OLED_RESET, OLED_CS); 

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
ESP8266WebServer server(80);

void setupWiFi(){
  Serial.println("Connecting to Wifi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}

bool updateInterval(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= pollTempInterval) {
    previousMillis = currentMillis;
    return true;
  }else{
    return false;
  }
}

void handleRoot() {
  display.fillRect(122, 0, 5, 5, 1);
  display.display();
  server.send(200, "text/plain", "Current Temp: " + displayedTempF + (char)176 + "F");
}

void handleNumberOnly() {
  display.fillRect(122, 0, 5, 5, 1);
  display.display();
  server.send(200, "text/plain", displayedTempF);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setupDisplay(){
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(1);
  display.setFont(&FreeSans24pt7b);
  display.display();  
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  setupDisplay();
  sensors.begin();
  server.on("/", handleRoot);
  server.on("/temp", handleNumberOnly);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void displayIP(){
  display.setFont();
  display.setCursor(0,0);
  display.print(WiFi.localIP());
}

void displayTemp(String fahrenheit){
  display.clearDisplay();
  displayIP();
  display.setFont(&FreeSans24pt7b);
  display.setCursor(6,60);    
  display.print(fahrenheit);
  display.display();  
}


void loop() {
  server.handleClient();
  if(updateInterval()){
    sensors.requestTemperatures();
    displayedTempF = sensors.getTempFByIndex(0);
    displayTemp(displayedTempF);
  }  
}
