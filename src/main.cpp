#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

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

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    uint8_t move;
} struct_message;

const uint16_t irPin = 4;

IRsend irsend(irPin);


// Create a struct_message called myData
struct_message myData;

//Default Command tree
unsigned short int commandTree[cmdTreeSize][3] = {{UUU, 0x0, 0x21},
                                                  {UUR, 0x0, 0x22},
                                                  {UUD, 0x0, 0x23},
                                                  {UUL, 0x0, 0x24},
                                                  {URU, 0x0, 0x25},
                                                  {URR, 0x0, 0x26},
                                                  {URD, 0x0, 0x27},
                                                  {URL, 0x0, 0x28},
                                                  {UDU, 0x0, 0x29},
                                                  {UDR, 0x0, 0x30},
                                                  {UDD, 0x0, 0x31},
                                                  {UDL, 0x0, 0x32},
                                                  {ULU, 0x0, 0x33},
                                                  {ULR, 0x0, 0x34},
                                                  {ULD, 0x0, 0x35},
                                                  {ULL, 0x0, 0x36},
                                                  {RUU, 0x0, 0x37},
                                                  {RUR, 0x0, 0x38},
                                                  {RUD, 0x0, 0x39},
                                                  {RUL, 0x0, 0x40},
                                                  {DUU, 0x0, 0x41}};

void sendIRbyMoveSequence(unsigned short int movement){
  for (int row = 0; row < cmdTreeSize; row++) {
    if(movement == commandTree[row][0]){
      irsend.sendNEC(irsend.encodeNEC(commandTree[row][1], commandTree[row][2]));
      Serial.println(commandTree[row][2]);
      Serial.println("Comando enviado!");
      Serial.println();
      break;
    }
  }
}
// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  if(myData.move <= 144){
    Serial.println(myData.move);
    sendIRbyMoveSequence(myData.move);
  } else {
    Serial.println(myData.move);
    Serial.println("Comando Não pertence a este!");
    Serial.println();
  }
}

 
void setup() {
  irsend.begin();

  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
}