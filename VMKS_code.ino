#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "SerialMP3Player.h"

#define SCREEN_WIDTH 128  
#define SCREEN_HEIGHT 64  

#define BUTTON_PAUSE 8      // Pin connected to "Pause/Resume" button
#define BUTTON_VOL_DOWN 5   // Pin connected to "Volume Down" button
#define BUTTON_VOL_UP 6     // Pin connected to "Volume Up" button
#define BUTTON_PREV_SONG 7  // Pin connected to "Previous Song" button
#define BUTTON_NEXT_SONG 4  // Pin connected to "Next Song" button

#define STATUS_STOPPED 0
#define STATUS_PLAYING 1
#define STATUS_PAUSED  2

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

SerialMP3Player mp3(0, 1);

static int pauseToggle = 0;

void setup() {
  Serial.begin(9600);
  mp3.begin(9600);        
  delay(500);            
  mp3.sendCommand(CMD_SEL_DEV, 0, 2);
  delay(500);            

  pinMode(BUTTON_PAUSE, INPUT_PULLUP);
  pinMode(BUTTON_VOL_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_VOL_UP, INPUT_PULLUP);
  pinMode(BUTTON_PREV_SONG, INPUT_PULLUP);
  pinMode(BUTTON_NEXT_SONG, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Display allocation failed"));
    for (;;);
  }

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
}

//String byte2hex(byte b)
//{
//  String shex;
//
//  //Serial.print("0x");
//  shex="0X";
//  //if (b < 16) Serial.print("0");
//
//  if (b < 16) shex+="0";
//  //Serial.print(b, HEX);
//  shex+=String(b,HEX);
//  //Serial.print(" ");
//  shex+=" ";
//  return shex;
//}

int count = 0;

void loop() {
//  count++;
//  if (mp3.available()) {
//    byte c;
//    do{
//      c = mp3.read();
//      Serial.println("Count:" + String(count) + " " + byte2hex(c));
//    }while(c!=0xEF);

////    if (mp3Data == 0x3D) {
////      mp3.playNext();
////    }
//  }
  if (mp3.available()){
    char c = mp3.read();
    if (c == 0x3D) {
        mp3.playNext();
      }
  }
  
  // Read button states
  bool pauseButtonState = digitalRead(BUTTON_PAUSE);
  bool volDownButtonState = digitalRead(BUTTON_VOL_DOWN);
  bool volUpButtonState = digitalRead(BUTTON_VOL_UP);
  bool prevSongButtonState = digitalRead(BUTTON_PREV_SONG);
  bool nextSongButtonState = digitalRead(BUTTON_NEXT_SONG);

  displayMessage("");

  // Handle button actions
  if (!pauseButtonState) {
    if (pauseToggle % 2 == 0) {
      mp3.play();
      displayMessage("Play");
      pauseToggle++;
    } else {
      mp3.pause();
      displayMessage("Pause");
      pauseToggle--;
    }
  }
  else if (!volDownButtonState) {
    mp3.volDown();
    mp3.volDown();
    mp3.volDown();
    mp3.volDown();
    displayMessage("Volume Down");
  }
  else if (!volUpButtonState) {
    mp3.volUp();
    mp3.volUp();
    mp3.volUp();
    mp3.volUp();
    displayMessage("Volume Up");
  }
  else if (!prevSongButtonState  ) {
    mp3.playPrevious();
    displayMessage("Previous Song");
  }
  else if (!nextSongButtonState) {
    mp3.playNext();
    displayMessage("Next Song");
  }


//  if (mp3.available()){
//    Serial.println(mp3.decodeMP3Answer()); // print decoded answers from mp3
//  }


  delay(500); // Debounce delay
}

void displayMessage(const char *message) {
  display.clearDisplay();
  display.setCursor(10, 10);
  display.println(F("MBTPlayer"));
  display.setCursor(10, 30);
  display.println(message);
  display.display();
}
