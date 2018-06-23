#include <PxMatrix.h>

#ifdef ESP32

#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 2
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#endif

#ifdef ESP8266

#include <Ticker.h>
Ticker display_ticker;
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
#define P_E 0
#define P_OE 2

#endif

// Pins for LED MATRIX
PxMATRIX display(64,32,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);

#ifdef ESP8266
// ISR for display refresh
void display_updater()
{
  //display.displayTestPattern(70);
  display.display(70);
}
#endif

#ifdef ESP32
void IRAM_ATTR display_updater(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  //isplay.display(70);
  display.displayTestPattern(70);
  portEXIT_CRITICAL_ISR(&timerMux);
}
#endif

//=== SEGMENTS ===
#include "Digit.h"
Digit digit0(&display, 0, 28+9,8, display.color565(0, 0,255));  
Digit digit1(&display, 0, 28,8, display.color565(0, 0,255));  
int changeSpeed = 500;
     
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  display.begin(16);
  
  #ifdef ESP8266
    display_ticker.attach(0.002, display_updater);
  #endif

  #ifdef ESP32
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 2000, true);
    timerAlarmEnable(timer);
  #endif

  display.fillScreen(display.color565(0, 0,0));
  digit0.Draw(0); 
  digit1.Draw(0); 
  delay(changeSpeed);
}


void loop() {
  for (byte d=1; d<=10; d++) {
    if (d==10) {
      digit0.Morph(0);
      int nextTens = digit1.Value() + 1;
      digit1.Morph(nextTens==10 ? 0 : nextTens);
    }
    else
      digit0.Morph(d);
    delay(changeSpeed);
  }
}
