/*
 * main.cpp
 *
 * Created on Sun Jan 10 2021
 *
 * Copyright (c) 2021 occamzchainsaw
 */

#include <Arduino.h>                                                //  Arduino Framework
#include <U8g2lib.h>                                                //  For drawing/writing on the OLED
#define FASTLED_INTERNAL
#include <FastLED.h>                                                //  Great lib for the LEDs
#include <Wifi.h>                                                   //  Handling the WiFi connection
#include <ESPAsyncWebServer.h>                                      //  Asynchronous WebServer <3
#include "ArduinoJson.h"

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

//  WiFi credentials
const char* ssid =      "FBI Surveillance Van #9973";
const char* password =  "e4e5Nf3Nc6Bb5";

//  Webserver talking on port 80
AsyncWebServer server(80);

//  stuff for the RGB LEDs
#define PIN_LED   5                                                   //  Data pin number
#define NUM_LEDS  144                                                 //  Number of LEDs on the whole strip
#define MAX_POWER 17500                                               //  Power limit for FastLED

CRGB g_LEDs[NUM_LEDS] = {0};                                          //  Frame buffer for FastLED                                         
uint8_t g_brightness = 16;                                            //  Brightness level for FastLED

uint effect_selector = 0;

//  My own includes, such as effects, website, and helper functions.
//  These need to be down here, cause I'll use some extern variables
//  in them, from this main.cpp file
#include <website.h>
#include <helpers.h>
#include <light_control.h>

byte g_red1, g_green1, g_blue1, g_red2, g_green2, g_blue2;
byte rainbow_speed, rainbow_init_hue, rainbow_delta;
int breathe_cycle, rainbow_cycle;
bool g_breathing, rainbow_run, rainbow_down;

void setup() {
  //Serial.begin(9600);

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
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(g_brightness);                                //  Set the brightness (only initial)
  set_max_power_in_milliwatts(MAX_POWER);                             //  We wouldn't wanna blow anything up, would we now?

  //  Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  //  This bit should handle all the requests made to the webserver
  //  When root requested, send an HTML 200 code, and the actual HTML to display the web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/html", index_html);
  });

  server.onRequestBody([](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    if (request->url() == "/updateParameters") {
      DynamicJsonDocument jsonDoc(2048);
      auto error = deserializeJson(jsonDoc, (const char*)data);
      if (error)
      {
          Serial.println(error.c_str());
      }
      else
      {
          std::string effect_type = jsonDoc["type"];
          if (effect_type == "solid_color")
          {
            effect_selector = 1;
            g_red1 = parseColor(jsonDoc["red"]);
            g_green1 = parseColor(jsonDoc["green"]);
            g_blue1 = parseColor(jsonDoc["blue"]);
            g_brightness = parseBrightness(jsonDoc["brightness"]);
            g_breathing = parseBool(jsonDoc["breathingOn"]);
          }
          else if(effect_type == "rainbow")
          {
            effect_selector = 2;
            rainbow_init_hue = parseColor(jsonDoc["init_hue"]);
            rainbow_delta = parseDelta(jsonDoc["delta_hue"]);
            rainbow_run = parseBool(jsonDoc["runningOn"]);
            g_brightness = parseBrightness(jsonDoc["brightness"]);
          }
      }
      request->send(200, "text/plain", "end");
    }
  });

  //  Start the WebServer - since it's asynchronous, we don't need to do anything else in the main loop
  server.begin();
}

void loop() {
  for(;;)                                                                               //  A very tight loop - makes things faster
  {
    //  Handle the LEDs
    // EVERY_N_MILLISECONDS(20)
    // {
    //   uint8_t hue = beatsin8(20, 0, 255);
    //   fill_rainbow(g_LEDs, NUM_LEDS, hue, 1);
    // }
    
    //g_brightness = 20;
    
    EVERY_N_MILLISECONDS(20)
    {
      switch (effect_selector) 
      {
        case 1:                                                         //  solid color fill
          solid_color(g_red1, g_green1, g_blue1);
          brightness_control(g_brightness, g_breathing, 2);
          break;

        case 2:                                                         //  rainbow controls
          if (rainbow_run)
            running_rainbow(3, rainbow_delta, false);                   //  this will be updated
          else
            solid_rainbow(rainbow_init_hue, rainbow_delta);
          brightness_control(g_brightness, false, 2);
          break;

        default:
          running_rainbow(3, 1, false);
          brightness_control(32, false, 2);
      }
      //solid_gradient(232, 153, 14, 15, 43, 195);
      //solid_white(Tungsten100W);
      //solid_rainbow(0, 2);
      // if (!flag)
      // {
      //   running_rainbow(3, 1, false);
      //   brightness_control(32, false, 2);
      // }
      // else
      // {
      //   solid_color(g_red1, g_green1, g_blue1);
      //   brightness_control(g_brightness, g_breathing, 2);
      // }      
    }

    //  Handle the OLED Display
    g_OLED.home();
    g_OLED.clearBuffer();

    EVERY_N_MILLISECONDS(1000)                                                          //  only update the OLED display every second, no need to go faster
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

    //FastLED.delay(10);                                                                  //  show the leds, and introduce a little delay to the loop
    FastLED.show();
  }
}