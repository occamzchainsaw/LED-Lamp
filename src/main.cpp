/*
 * main.cpp
 *
 * Created on         Sun Jan 10 2021
 * Last modified on   Fri May 05 2023
 *
 * Copyright (c) 2021 occamzchainsaw
 */

#include <Arduino.h>                                                //  Arduino Framework
#include <U8g2lib.h>                                                //  For drawing/writing on the OLED
#define FASTLED_INTERNAL
#include <FastLED.h>                                                //  Great lib for the LEDs
#include <Wifi.h>                                                   //  Handling the WiFi connection
#include "ArduinoJson.h"
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
const char* ssid =      "Get Your Own Wi-Fi";
const char* password =  "e4e5Nf3Nc6Bb5";
IPAddress local_ip(192,168,0,194);
IPAddress gateway(192,168,0,1);
IPAddress mask(255,255,255,0);

//  Webserver talking on port 80
AsyncWebServer server(80);

//  stuff for the RGB LEDs
#define PIN_LED   5                                                   //  Data pin number

CRGB g_LEDs[NUM_LEDS] = {0};                                          //  Frame buffer for FastLED                                         
uint8_t g_brightness = 170;                                            //  Brightness level for FastLED

std::string g_effect = "";
std::string g_option1 = "";
std::string g_option2 = "";
std::map<std::string, int> effectMap = {
  {"solid",     1},
  {"palette",   2},
  {"rainbow",   3},
  {"fire",      4}
};

byte g_red1, g_green1, g_blue1, g_red2, g_green2, g_blue2;
byte rainbow_speed, rainbow_init_hue, rainbow_delta;
int breathe_cycle, rainbow_cycle;
bool g_breathing, rainbow_run, rainbow_down;

SolidColourEffect solidColour = SolidColourEffect();
ColourRotationEffect paletteEffect = ColourRotationEffect();
RainbowEffect rainbowEffect = RainbowEffect();

int wifiTries = 0;

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "application/json", "{\"message\":\"Not Found\"}");
}

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
  if (!WiFi.config(local_ip, gateway, mask))
  {
    Serial.println("STA failed to configure");
  }
  WiFi.begin(ssid, password);
  while ((WiFi.status() != WL_CONNECTED) && (wifiTries < 10))
  {
    Serial.printf("Attempt number %i to connect to wifi\n", wifiTries+1);
    delay(500);
    wifiTries++;
  }
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("Failed to connect to wifi");
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", "{\"message\":\"Hello there\"}");
  });

  // REQUEST EXAMPLE
  // http://192.168.88.196/get-effect?brightness=75&effect=fire&option1=redFire
  // http://192.168.0.104/get-effect?brightness=50&effect=rainbow&option1=running&option2=false
  // CURL:
  // curl -X "GET" "http:..."
  server.on("/get-effect", HTTP_ANY, [](AsyncWebServerRequest *request){
    StaticJsonDocument<500> doc;
    if(request->hasParam("brightness"))
    {
      doc["brightness"] = request->getParam("brightness")->value();
      g_brightness = scaleBrightness(doc["brightness"].as<int>());
    }
    else
    {
      doc["brightness"] = 0;
    }

    if(request->hasParam("effect"))
    {
      doc["effect"] = request->getParam("effect")->value();
      g_effect = doc["effect"].as<std::string>();
    }
    else
    {
      doc["effect"] = "No effect parameter";
    }

    if(request->hasParam("option1"))
    {
      doc["option1"] = request->getParam("option1")->value();
      g_option1 = doc["option1"].as<std::string>();
    }
    else
    {
      doc["option1"] = "No value";
    }

    if(request->hasParam("option2"))
    {
      doc["option2"] = request->getParam("option2")->value();
      g_option2 = doc["option2"].as<std::string>();
    }
    else
    {
      doc["option2"] = "No value";
    }

    switch (effectMap[g_effect])
    {
      case 1:
        solidColour.setupEffect(g_option1, g_option2);
        break;
      
      case 2:
        paletteEffect.setupEffect(g_option1, g_option2);
        break;
      
      case 3:
        rainbowEffect.setupEffect(g_option1, g_option2);
        break;
    }

    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });
  server.onNotFound(notFound);

  server.begin();
}

//  Main ESP32 loop
void loop() {
  for(;;)                                                                               //  A very tight loop - makes things faster
  {
    // EFFECT SELECTION AND OPTION HANDLING
    switch (effectMap[g_effect])
    {
      case 0:
        rainbowEffect.drawSelected();
        break;

      case 1:
        solidColour.draw(g_brightness);
        break;

      case 2:
        paletteEffect.draw();
        break;

      case 3:
        rainbowEffect.drawSelected();
        break;

      case 4:
        FireEffect(PaletteMap[g_option1]);
        break;
    }

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

    uint8_t scaledBri = calculate_max_brightness_for_power_mW(FastLED.leds(), FastLED.size(), g_brightness, MAX_POWER);
    FastLED.setBrightness(scaledBri);
    FastLED.show();
    FastLED.delay(fpsToMillis(50));
  }
}