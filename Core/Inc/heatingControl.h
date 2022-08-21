#ifndef INC_HEATERCONTROLLER_H_
#define INC_HEATERCONTROLLER_H_

#include "main.h"
#include "defines.h"
#include "controlType.h"


int getSelectedTemp(void);
void setSettingTemp(void);

void selectedTempUp(void);
void selectedTempDown(void);

void relayControl(float current_temp);
void relayOnOff(ON_OFF_t on_off);
ON_OFF_t getRelayState(void);

#endif
