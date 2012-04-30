#define __AVR_ATmega328P__
#define __cplusplus
#define __builtin_va_list int
#define __attribute__(x)
#define __inline__
#define __asm__(x)
#define ARDUINO 100
extern "C" void __cxa_pure_virtual() {}
void keepAliveToggle();
void setup();
void loop();
void triggerToggle();
void onSecondTick();
inline void changeState(CycleStates newState);
inline void startCycle();
inline void fireMarker();
inline void onTimerTick();
inline void onADCReadComplete();

#include "C:\Program Files (x86)\arduino\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\Program Files (x86)\arduino\hardware\arduino\cores\arduino\Arduino.h"
#include "C:\Programming\YAAB\YAAB.ino" 
