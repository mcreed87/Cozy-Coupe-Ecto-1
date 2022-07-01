#include <SoftwareSerial.h>
#include "Adafruit_Soundboard.h"


// Choose any two pins that can be used with SoftwareSerial to RX & TX
#define SFX_TX 11
#define SFX_RX 10

// Connect to the RST pin on the Sound Board
#define SFX_RST 9

#define SwitchSound1 7
#define SwitchSound2 8

SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit Sound Board!");

  pinMode( SwitchSound1, INPUT );
  pinMode( SwitchSound2, INPUT );
  
  ss.begin(9600);

  if (!sfx.reset()) 
  {
    Serial.println("Not found");
    while (1);
  }
  Serial.println("SFX board found");
}


void loop() {
  flushInput();
  
  byte switchSound1 = digitalRead(SwitchSound1);
  byte switchSound2 = digitalRead(SwitchSound2);
  byte rest = digitalRead(SFX_RST);
      
  flushInput();
  
  char cmd = 'r';
  if (switchSound1 == 1)
  {
    if(!sfx.playTrack(2))
    {
      Serial.println("Sound 1 Failed");
    }
  }
  else if (switchSound2 == 1)
  {
    if(!sfx.playTrack(3))
    {
      Serial.println("Sound 2 Failed");
    }
  }
  else
  {
    if (!sfx.reset()) 
    {
      Serial.println("Reset failed");
    }
  }
}

/************************ MENU HELPERS ***************************/

void flushInput() {
  // Read all available serial input to flush pending data.
  uint16_t timeoutloop = 0;
  while (timeoutloop++ < 40) {
    while(ss.available()) {
      ss.read();
      timeoutloop = 0;  // If char was received reset the timer
    }
    delay(1);
  }
}

char readBlocking() {
  while (!Serial.available());
  return Serial.read();
}

uint16_t readnumber() {
  uint16_t x = 0;
  char c;
  while (! isdigit(c = readBlocking())) {
    //Serial.print(c);
  }
  Serial.print(c);
  x = c - '0';
  while (isdigit(c = readBlocking())) {
    Serial.print(c);
    x *= 10;
    x += c - '0';
  }
  return x;
}

uint8_t readline(char *buff, uint8_t maxbuff) {
  uint16_t buffidx = 0;
  
  while (true) {
    if (buffidx > maxbuff) {
      break;
    }

    if (Serial.available()) {
      char c =  Serial.read();
      //Serial.print(c, HEX); Serial.print("#"); Serial.println(c);

      if (c == '\r') continue;
      if (c == 0xA) {
        if (buffidx == 0) {  // the first 0x0A is ignored
          continue;
        }
        buff[buffidx] = 0;  // null term
        return buffidx;
      }
      buff[buffidx] = c;
      buffidx++;
    }
  }
  buff[buffidx] = 0;  // null term
  return buffidx;
}
/************************ MENU HELPERS ***************************/
