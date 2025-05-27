#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <FastLED.h>

#define UUU 111
#define UUR 112
#define UUD 113
#define UUL 114
#define URU 121
#define URR 122
#define URD 123
#define URL 124
#define UDU 131
#define UDR 132
#define UDD 133
#define UDL 134
#define ULU 141
#define ULR 142
#define ULD 143
#define ULL 144
#define RUU 211
#define RUR 212
#define RUD 213
#define RUL 214
#define RRU 221
#define RRR 222
#define RRD 223
#define RRL 224
#define RDU 231
#define RDR 232
#define RDD 233
#define RDL 234
#define RLU 241
#define RLR 242
#define RLD 243
#define RLL 244
#define DUU 311
#define DUR 312
#define DUD 313
#define DUL 314
#define DRU 321
#define DRR 322
#define DRD 323
#define DRL 324
#define DDU 331
#define DDR 332
#define DDD 333
#define DDL 334
#define DLU 341
#define DLR 342
#define DLD 343
#define DLL 344
#define LUU 411
#define LUR 412
#define LUD 413
#define LUL 414
#define LRU 421
#define LRR 422
#define LRD 423
#define LRL 424
#define LDU 431
#define LDR 432
#define LDD 433
#define LDL 434
#define LLU 441
#define LLR 442
#define LLD 443
#define LLL 444

#define cmdTreeSize 21

#define LED_IN 4 // D2

#define NUM_LEDS 8

#define ID_LED_1 0
#define ID_LED_2 1
#define ID_LED_3 2

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int move;
} struct_message;

const uint16_t emiPin = 5; // D1
const uint16_t rcvPin = 0; // D3

IRsend irSend(emiPin);
IRrecv irRecv(rcvPin);

decode_results results;

// const uint16_t kCaptureBufferSize = 1024;
// const uint8_t kTimeout = 50;
// const uint16_t kFrequency = 38000;
// IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, false);

// Create a struct_message called myData
struct_message myData;

CRGB leds[NUM_LEDS];


//Default Command tree PROJETOR
unsigned short int commandTree[cmdTreeSize][3] = {{UUU, 0x0, 0x14}, // ON/OFF
                                                  {UUR, 0x0, 0x13}, // MENU
                                                  {UUD, 0x0, 0x7}, // OK
                                                  {UUL, 0x0, 0x5C}, // BACK
                                                  {URU, 0x0, 0x82}, // MOUSE
                                                  {URR, 0x0, 0xB}, // PLUS SOUND
                                                  {URD, 0x0, 0x1}, // MUTE
                                                  {URL, 0x0, 0x58}, // MINUS SOUND
                                                  {UDU, 0x0, 0x3}, // ARROW UP
                                                  {UDR, 0x0, 0x1A}, // ARROW RIGHT
                                                  {UDD, 0x0, 0x2}, // ARROW DOWN
                                                  {UDL, 0x0, 0xE}, // ARROW LEFT
                                                  {ULU, 0x0, 0x48}, // HOME
                                                  // {ULR, 0x0, 0x34},
                                                  // {ULD, 0x0, 0x35},
                                                  // {ULL, 0x0, 0x36},
                                                  // {RUU, 0x0, 0x37},
                                                  // {RUR, 0x0, 0x38},
                                                  // {RUD, 0x0, 0x39},
                                                  // {RUL, 0x0, 0x40},
                                                  // {DUU, 0x0, 0x41}
                                                  };

void wait(unsigned long milliseconds)
{
  unsigned long currentTime = millis();
  unsigned long previousTime = millis();

  while (currentTime - previousTime <= milliseconds)
  {
    currentTime = millis();
  }
}

void ledControl(uint8 singleMove, uint8 idLed){
  switch (singleMove){
    case 1:
      leds[idLed] = CRGB(255, 0, 0);
      FastLED.show();
      break;
    case 2:
      leds[idLed] = CRGB(0, 255, 0);
      FastLED.show();
      break;
    case 3:
      leds[idLed] = CRGB(0, 0, 255);
      FastLED.show();
      break;
    case 4:
      leds[idLed] = CRGB(255, 255, 0);
      FastLED.show();
      break;
    default:
      FastLED.clear();
      break;
  }
}

void sendIRbyMoveSequence(unsigned short int movement){
  for (int row = 0; row < cmdTreeSize; row++) {
    if(movement == commandTree[row][0]){
      irSend.sendNEC(irSend.encodeNEC(commandTree[row][1], commandTree[row][2]));
      Serial.println(commandTree[row][2]);
      Serial.println("Comando enviado!");
      Serial.println();
      break;
    }
  }
}
// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  // digitalWrite(LED_IN, LOW);
  memcpy(&myData, incomingData, sizeof(myData));

  Serial.println(myData.move);

  ledControl(myData.move/100, ID_LED_1);
  ledControl((myData.move%100)/10, ID_LED_2);
  ledControl((myData.move%100)%10, ID_LED_3);

  if(myData.move <= 144){
    sendIRbyMoveSequence(myData.move);
  
  } else {
    Serial.println("Comando Não pertence a este!");
    Serial.println();
  }
  wait(100);
  FastLED.clear();
}

 
void setup() {
  irSend.begin();

  // pinMode(LED_IN, OUTPUT);
  FastLED.addLeds<NEOPIXEL, LED_IN>(leds, NUM_LEDS);

  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  irRecv.enableIRIn();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  // digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  if(irRecv.decode(&results)){
    Serial.println("Decoding IR Received");
    Serial.println(results.decode_type);
    Serial.println("Address: ");
    Serial.print("0x");
    Serial.println(results.address, HEX);
    Serial.println("Command: ");
    Serial.print("0x");
    Serial.println(results.command, HEX);
    Serial.println();
    irRecv.resume(); // Receive the next value
  };
}