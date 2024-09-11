#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>

#define CHANNEL 1

void OnDataRecev(uint8_t, uint8_t, uint8_t);

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW initialization failed");
    while(true);
  }

  esp_now_set_recv_cb(OnDataRecv);

  uint8_t peerAddress[] = {0x24, 0x6F, 0x28, 0x7B, 0xA9, 0xB1}; // MAC Address do emissor
  esp_now_add_peer(peerAddress, ESP_NOW_ROLE_COMBO, CHANNEL, NULL, 0);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void OnDataRecv(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len) {
    Serial.print("Received value: ");
    Serial.println((int)incomingData);
}