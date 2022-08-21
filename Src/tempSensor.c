#include "ds18b20.h"

Ds18b20Sensor_t	tempSensor;
extern OneWire_t OneWire;
extern uint8_t OneWireDevices;

static char init_state = 0;
static char convert_state = 0;

/*
 * OneWire 를 초기화시키고 GPIO 핀설정을 완료한 후, 사용하는 온도센서의 ROM 주소를 OneWire 주소 리스트에 초기화시키고
 * GetFullROM 함수를 사용해 설정한 주소리스트를 온도센서 주소에 대입한다. 온도 정밀도를 설정하기 위해 SetResolution 함수를
 * 통하여 온도를 12bit 만큼 정밀하게 표현하도록 한다. 알람기능도 지원하지만 사용하지 않기 때문에 비활성화한다.
 */
void TempSensor_Init(void){
	init_state = 0;
	OneWire_Init(&OneWire,_DS18B20_GPIO ,_DS18B20_PIN);
	OneWire.ROM_NO[0] = 0x28;
	OneWire.ROM_NO[1] = 0x78;
	OneWire.ROM_NO[2] = 0x89;
	OneWire.ROM_NO[3] = 0x95;
	OneWire.ROM_NO[4] = 0xf0;
	OneWire.ROM_NO[5] = 0x01;
	OneWire.ROM_NO[6] = 0x3c;
	OneWire.ROM_NO[7] = 0x2b;
	OneWire_GetFullROM(&OneWire, tempSensor.Address);

	Ds18b20Delay(50);
	DS18B20_SetResolution(&OneWire, tempSensor.Address, DS18B20_Resolution_12bits);
	Ds18b20Delay(50);
	DS18B20_DisableAlarmTemperature(&OneWire, tempSensor.Address);
	init_state = 1;
}

// StartAll 함수는 모든 slave 장치에게 동시에 명령어를 보낼 수 있도록 하고, 온도 변환 시작을 알린다. AllDone 함수에서는 진행은 0을 완료는 1을 반환한다.
void startConvert(){
	DS18B20_StartAll(&OneWire);
	convert_state = !DS18B20_AllDone(&OneWire);
	convert_state = 1;
}

// AllDone 함수에서 완료(1)가 되었을 때, 온도정보를 받아올 수 있다.
void checkConvert() {
	convert_state = !DS18B20_AllDone(&OneWire);
}

// 초기화 상태를 반환한다.
char isTempSensorInit(){
	return init_state;
}

// 온도변환 상태를 반환한다.
char isConverted(){
	return convert_state;
}

/*
 * Read 함수를 통해 OneWire 리셋하고 OneWire ROM 주소를 선택한 후, 온도정보가 담겨있는 9 byte의 scratchpad 를 읽는다. CRC 함수로 오류를 검출하고
 * 사람이 편히 볼 수 있는 형태의 온도정보로 변환한다. 온도정보가 타당한 지를 보여 줄 수 있도록 DataIsValid 에 false, true 중 하나를 반환한다.
 */
float getTemp(){
	Ds18b20Delay(100);
	tempSensor.DataIsValid = DS18B20_Read(&OneWire, tempSensor.Address, &tempSensor.Temperature);

	return tempSensor.Temperature;
}

// 온도정보를 반환한다.
float getCurrentTemp(){
	return tempSensor.Temperature;
}

