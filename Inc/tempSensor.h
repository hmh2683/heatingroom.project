#ifndef INC_TEMPSENSOR_H_
#define INC_TEMPSENSOR_H_

void TempSensor_Init(void);
void startConvert(void);
void checkConvert(void);

char isTempSensorInit(void);
char isConverted(void);

float getTemp(void);
float getCurrentTemp(void);

#endif /* INC_TEMPSENSOR_H_ */
