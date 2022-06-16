/*
 * main.cpp
 *
 * Created on         Sun Jan 10 2021
 * Last modified on   Sun Aug 15 2021
 *
 * Copyright (c) 2021 occamzchainsaw
 */

#include <Arduino.h>                                                //  Arduino Framework
#include <U8g2lib.h>                                                //  For drawing/writing on the OLED
#define FASTLED_INTERNAL
#include <FastLED.h>                                                //  Great lib for the LEDs
#include <Wifi.h>                                                   //  Handling the WiFi connection
#include <SPIFFS.h>                                                 //  FileSystem
#include "ArduinoJson.h"
#include <ESPmDNS.h>
#include "ESPAsyncWebServer.h"
#include "AsyncJson.h"

//  My own includes, such as effects, website, and helper functions.
//  These need to be down here, cause I'll use some extern variables
//  in them, from this main.cpp file
#include <helpers.h>
#include <colour_lists.h>
#include <solid_colour.h>
#include <colour_rotation.h>
#include <lit_stuff.h>
#include <rainbows.h>
#include <pacifica.h>

//  stuff for the OLED display
#define OLED_CLOCK  15                                              //  OLED clock pin
#define OLED_DATA   4                                               //  OLED data pin
#define OLED_RESET  16                                              //  OLED reset pin

#define X_PADDING 3
#define Y_PADDING 3

U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R2, OLED_RESET, OLED_CLOCK, OLED_DATA);     //  OLED Display object, I2C handled in HW for speeeeeeeeeeed

int g_LineHeight = 0;
uint g_oled_width = 0;
uint g_oled_height = 0;

//  WiFi credentials      *** THIS NEEDS TO BE CHANGED TO THE DESIRED CREDENTIALS EVERY TIME  ***
const char* ssid =      "FBI Surveillance Van #9973";
const char* password =  "e4e5Nf3Nc6Bb5";

//  Webserver talking on port 80
AsyncWebServer server(80);

//  stuff for the RGB LEDs
#define PIN_LED   5                                                   //  Data pin number

CRGB g_LEDs[NUM_LEDS] = {0};                                          //  Frame buffer for FastLED                                         
uint8_t g_brightness = 50;                                            //  Brightness level for FastLED

uint8_t effect_selector = 0;
uint8_t option_selector = 0;

byte g_red1, g_green1, g_blue1, g_red2, g_green2, g_blue2;
byte rainbow_speed, rainbow_init_hue, rainbow_delta;
int breathe_cycle, rainbow_cycle;
bool g_breathing, rainbow_run, rainbow_down;

int wifiTries = 0;

//  The setup routine runs when the ESP32 starts.
//  It's used to configure some stuff for the OLED, the FastLED object, connect to the WiFi, and start the WebServer
void setup() {
  Serial.begin(9600);                                                 //  Used for serial monitor, if debugging is needed

  pinMode(PIN_LED, OUTPUT);                                           //  Set the LED data pin to output mode

  //  Some settings for the OLED
  g_OLED.begin();
  g_OLED.clear();
  g_OLED.setFont(u8g2_font_helvR10_tf);                               //  If I can use Helvetica, I'll use Helvetica.
  g_LineHeight = g_OLED.getFontAscent() - g_OLED.getFontDescent();    //  Descent is negative, so to add it, I subtract it.
  g_oled_width = g_OLED.getWidth();
  g_oled_height = g_OLED.getHeight();

  //  Some settings for FastLED
  FastLED.addLeds<WS2812B, PIN_LED, GRB>(g_LEDs, NUM_LEDS);           //  Define the type and color scheme for the LEDs
  FastLED.setCorrection(TypicalLEDStrip);                             //  Colour correction - basically less green, cause green LEDs are usually perceived the brightest
  FastLED.setBrightness(g_brightness);                                //  Set the brightness (only initial)
  set_max_power_in_milliwatts(MAX_POWER);                             //  We wouldn't wanna blow anything up, would we now?

  //  Try to connect to the WiFi 5 times. If not possible, just move on.
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while ((WiFi.status() != WL_CONNECTED) && (wifiTries < 5))
  {
    delay(500);
    wifiTries++;
  }
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println(WiFi.localIP());
  }

  if(!SPIFFS.begin(true))
  {
    Serial.println("Error while mounting SPIFFS...");
    return;
  }

  MDNS.begin("rgb-controller");

  //  DefaultHeaders for the browser
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, PUT");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

  //  Webserver stuff
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send(SPIFFS, "/index.html", String());
  // });

  // server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send(SPIFFS, "/index.css", "text/css");
  // });

  // server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send(SPIFFS, "/index.js", "text/script");
  // });

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  //  Handler for the JSON
  server.addHandler(new AsyncCallbackJsonWebHandler("/led", [](AsyncWebServerRequest *request, JsonVariant &json) {
    const JsonObject &jsonObj = json.as<JsonObject>();
    if (jsonObj["effect"] == "solid")
    {
      Serial.println("Solid Colour Effect selected");
      effect_selector = 1;
      std::string sCol = jsonObj["option1"];
      CRGB col = parseHexCode(sCol);

      Serial.printf("Input: %s \n", sCol);
      Serial.printf("Output: %d, %d, %d", col.r, col.g, col.b);
    }
    else if (jsonObj["effect"] == "fire")
    {
      Serial.println("BLAZE IT.");
    }
    request->send(200, "SOLID");
  }));

  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS)
    {
      request->send(200);                   // send OK if options
    }
    else
    {
      Serial.println("Not found!");
      request->send(404, "Not found");      // otherwise, send NOT FOUND
    }
  });

  server.begin();
}

//  Main ESP32 loop
void loop() {
  FastLED.setBrightness(150);
  
  for(;;)                                                                               //  A very tight loop - makes things faster
  {
    //  TESTING SECTION

      //FireEffect(RedFire);
      //ColorRotationEffect(Rainbow, 1000);
      //OscillatingRainbow();
      //pacifica_loop();
      RunningRainbow(false);

    //  Handle the OLED Display
    g_OLED.home();
    g_OLED.clearBuffer();

    EVERY_N_MILLISECONDS(5000)                                                          //  only update the OLED display every 5 seconds, no need to go faster
    {
      calculate_actual_power();
      g_OLED.setCursor(X_PADDING, Y_PADDING + g_LineHeight);
      g_OLED.printf("fps: %d @ B: %d", FastLED.getFPS(), actual_brightness);            //  display how fast FastLED can draw
      g_OLED.setCursor(X_PADDING, Y_PADDING + g_LineHeight * 2);
      g_OLED.printf("P: %.3f W", uscaled_power);                                        //  display the requested power (prior to FastLED limiting it)
      g_OLED.setCursor(X_PADDING, Y_PADDING + g_LineHeight * 3);
      g_OLED.printf("p: %.3f W", actual_power);                                         //  display the limited power
      // If connected to wifi, print the local IP on the OLED
      if (WiFi.status() == WL_CONNECTED)
      {
        g_OLED.setCursor(X_PADDING, Y_PADDING + g_LineHeight * 4);
        g_OLED.print(WiFi.localIP());
      }
      g_OLED.sendBuffer();
    }

    uint8_t scaledBri = calculate_max_brightness_for_power_mW(FastLED.leds(), FastLED.size(), 180, MAX_POWER);
    FastLED.setBrightness(scaledBri);
    FastLED.show();
    FastLED.delay(fpsToMillis(50));
  }
}