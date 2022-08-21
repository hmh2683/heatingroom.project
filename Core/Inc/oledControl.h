#ifndef INC_OLEDCONTROL_H_
#define INC_OLEDCONTROL_H_

#include <stdio.h>
#include "main.h"
#include "defines.h"
#include "controlType.h"

void Screen_Init(void);
void settingScreen(void);
void defaultScreen(void);
void tempScreen(int temp);
void workScreen(ON_OFF_t on_off);

void buttonLed();
void buttonSetLed();
void startSwLed(ON_OFF_t on_off);

#endif
