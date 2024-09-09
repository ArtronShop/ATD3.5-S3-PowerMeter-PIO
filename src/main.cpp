#include <Arduino.h>
#include <lvgl.h>
#include <ATD3.5-S3.h>
#include "gui/ui.h"
#include <PZEM004Tv30.h>

#define PZEM_RX_PIN 2
#define PZEM_TX_PIN 1
#define PZEM_SERIAL Serial2

PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6kvBNagwF"
#define BLYNK_TEMPLATE_NAME "Power Meter"
#define BLYNK_AUTH_TOKEN "65OaKKb58hJBrOpd8KKXKR2HR13ueFBi"


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "CYD1";
char pass[] = "12345678";

void read_power_meter_timer_cb(lv_timer_t * timer) {
  // Read the data from the sensor
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();

  // Check if the data is valid
  if(isnan(voltage)){
    Serial.println("Error reading voltage");
  } else if (isnan(current)) {
    Serial.println("Error reading current");
  } else if (isnan(power)) {
    Serial.println("Error reading power");
  } else if (isnan(energy)) {
    Serial.println("Error reading energy");
  } else if (isnan(frequency)) {
    Serial.println("Error reading frequency");
  } else if (isnan(pf)) {
    Serial.println("Error reading power factor");
  } else {
    // Print the values to the Serial console
    Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
    Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
    Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
    Serial.print("Energy: ");       Serial.print(energy,3);     Serial.println("kWh");
    Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
    Serial.print("PF: ");           Serial.println(pf);
  }
  Serial.println();

  lv_label_set_text_fmt(ui_voltage_value_label, "%.0f", voltage);
  lv_arc_set_value(ui_voltage_value_arc, voltage);
  lv_label_set_text_fmt(ui_current_value_label, "%.01f", current);
  lv_arc_set_value(ui_current_value_arc, current);
  lv_label_set_text_fmt(ui_power_value_label, "%.0f", power);
  lv_arc_set_value(ui_power_value_arc, power);
  lv_label_set_text_fmt(ui_frequency_value_label, "%.0f", frequency);
  lv_arc_set_value(ui_frequency_value_arc, frequency);
  lv_label_set_text_fmt(ui_pf_value_label, "%.02f", pf);
  lv_arc_set_value(ui_pf_value_arc, pf * 100);
  lv_label_set_text_fmt(ui_energy_value_label, "%.03f", energy);

  Blynk.virtualWrite(0, voltage);
  Blynk.virtualWrite(1, current);
  Blynk.virtualWrite(2, power);
  Blynk.virtualWrite(3, energy);
  Blynk.virtualWrite(4, frequency);
  Blynk.virtualWrite(5, pf);
}

void setup() {
  Serial.begin(115200);
  
  // Setup peripherals
  Display.begin(0); // rotation number 0
  Touch.begin();
  Sound.begin();
  // Card.begin(); // uncomment if you want to Read/Write/Play/Load file in MicroSD Card

  // Map peripheral to LVGL
  Display.useLVGL(); // Map display to LVGL
  Touch.useLVGL(); // Map touch screen to LVGL
  Sound.useLVGL(); // Map speaker to LVGL
  // Card.useLVGL(); // Map MicroSD Card to LVGL File System

  Display.enableAutoSleep(120); // Auto off display if not touch in 2 min
  
  // Add load your UI function
  ui_init();

  // Add event handle
  lv_timer_create(read_power_meter_timer_cb, 1000, NULL);

  // Add Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Display.loop(); // Keep GUI work
  Blynk.run(); // Keep Blynk work
}
