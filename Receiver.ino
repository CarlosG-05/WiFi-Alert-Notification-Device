#include <esp_now.h>
#include <WiFi.h>
#include "pitches.h"

int melody1[] = {
  REST, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_B4,
  NOTE_G5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_E5, NOTE_F5, NOTE_G5, REST,
  NOTE_E5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5,
  NOTE_A4, NOTE_A4, REST, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_G4, REST, NOTE_G4,
  NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_C5, NOTE_B4,
  NOTE_G5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_E5, NOTE_F5, NOTE_G5,
  NOTE_E5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5,
  NOTE_A4, NOTE_A4, REST, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_G4, REST, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_A4, NOTE_A4, REST, NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_B4, NOTE_B4, NOTE_C5, REST,
  REST
};

int durations1[] = {
  4, 2, 2, 2, 2, 2, 2, 2, 4,
  4, 8, 8, 4, 8, 8, 2, 2,
  4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8,
  4, 8, 8, 4, 8, 8, 8, 8, 2, 8, 8,
  8, 8, 4, 4, 4, 1,
  4, 8, 8, 4, 8, 8, 1,
  4, 8, 8, 4, 8, 8, 4, 8, 8, 4, 8, 8,
  4, 8, 8, 4, 8, 8, 8, 8, 4, 4, 8, 8,
  8, 8, 8, 8, 4, 4, 8, 8, 4, 4, 8, 8,
  1
};

int melody2[] = {
  NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4,
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4,NOTE_F4,NOTE_F4,
  NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4,
  NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_G4,

  NOTE_DS5, NOTE_D5, NOTE_DS5, NOTE_C5, REST,
  NOTE_DS5, NOTE_D5, REST,
  NOTE_F5, NOTE_DS5, REST,

  NOTE_DS5, NOTE_D5, NOTE_DS5, NOTE_C5, REST,
  NOTE_DS5, NOTE_D5, REST,
  NOTE_F5, NOTE_DS5, REST,

  NOTE_DS5, NOTE_D5, NOTE_DS5, NOTE_C5, REST,
  NOTE_DS5, NOTE_D5, REST,
  NOTE_F5, NOTE_DS5, REST,

  NOTE_AS4, NOTE_C5, NOTE_AS5, NOTE_GS5, NOTE_G5, NOTE_G5,
};

int durations2[] = {
  4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4,

  4, 4, 4, 2, 4,
  4, 2, 4,
  4, 2, 2,

  4, 4, 4, 2, 4,
  4, 2, 4,
  4, 2, 2,

  4, 4, 4, 2, 4,
  4, 2, 4,
  4, 2, 2,

  4, 4, 4, 2, 2, 1,
};

int melody3[] = {
  NOTE_D4, NOTE_F4, NOTE_D5,

  NOTE_C5, NOTE_A4,

  NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,

  NOTE_D4,

  NOTE_F4, NOTE_A4, NOTE_F5,

  NOTE_E5, NOTE_C5, NOTE_A4,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4,

  NOTE_G4, NOTE_F4,

  NOTE_E4, NOTE_A4, NOTE_C5,

  NOTE_D5, NOTE_D6, NOTE_C6,

  NOTE_C6, NOTE_A4, NOTE_C5,

  NOTE_D5, NOTE_AS5, NOTE_A5,

  NOTE_A5, NOTE_A4, NOTE_A5,

  NOTE_D5, NOTE_F5, NOTE_D5, NOTE_C5,

  NOTE_A4, NOTE_C5, NOTE_G4, NOTE_F4,

  NOTE_D4, NOTE_F4, NOTE_D4, NOTE_C4
};

int durations3[] = {
  2, 4, 4,

  2, 2,

  6, 8, 4, 4,

  1,

  2, 4, 4,

  2, 4, 4,

  4, 4, 6, 8,////////////

  2, 2,

  2, 4, 4,

  2, 6, 8,

  2, 4, 4,

  2, 6, 8,

  2, 4, 4,

  4, 4, 4, 4,

  4, 4, 4, 4,

  4, 4, 4, 4
};

#define CHANNEL 1

uint8_t recivedData;

void setup() {
  Serial.begin(115200);

  pinMode(2, OUTPUT);
  pinMode(23, OUTPUT);

  WiFi.mode(WIFI_AP); //Sets Reciver AS Access Point
  WiFi.softAP("TARGET_1", "TARGET_1_Password", CHANNEL, 0);

  esp_now_init();
  esp_now_register_recv_cb(GetData);
}

void loop() {
  if (recivedData == 1) {
    int size = sizeof(durations1) / sizeof(int);

    for (int note = 0; note < size; note++) {
      //to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int duration = 1000 / durations1[note];
      tone(23, melody1[note], duration);

      //to distinguish the notes, set a minimum time between them.
      //the note's duration + 30% seems to work well:
      int pauseBetweenNotes = duration * 1.30;
      delay(pauseBetweenNotes);

      //stop the tone playing:
      noTone(23);
    }
    recivedData = 0;
  } 
  else if(recivedData == 2) {
    int size = sizeof(durations2) / sizeof(int);

    for (int note = 0; note < size; note++) {
      //to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int duration = 1000 / durations2[note];
      tone(23, melody2[note], duration);

      //to distinguish the notes, set a minimum time between them.
      //the note's duration + 30% seems to work well:
      int pauseBetweenNotes = duration * 1.30;
      delay(pauseBetweenNotes);

      //stop the tone playing:
      noTone(23);
    }

    recivedData = 0;
  } 
  else if(recivedData == 3) {
    int size = sizeof(durations3) / sizeof(int);

    for (int note = 0; note < size; note++) {
      //to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int duration = 1000 / durations3[note];
      tone(23, melody3[note], duration);

      //to distinguish the notes, set a minimum time between them.
      //the note's duration + 30% seems to work well:
      int pauseBetweenNotes = duration * 1.30;
      delay(pauseBetweenNotes);

      //stop the tone playing:
      noTone(23);
    }

    recivedData = 0;
  }
  else {
    noTone(23);
  }
}

void GetData(const uint8_t * mac_addr, const uint8_t *data, int data_len) {
  Serial.print("Data Has Been Recived: ");
  Serial.println(*data);
  memcpy(&recivedData, data, sizeof(recivedData));
}
