#include "heatingControl.h"
#include "oledControl.h"

static int selected_temp = DEFAULT_TEMP;
static int setting_temp = 0;
static uint8_t relay_state = 0;

// 선택온도를 설정온도로 지정한다.
void setSettingTemp() {
	setting_temp = selected_temp;
}

// 선택온도를 반환한다.
int getSelectedTemp() {
	return selected_temp;
}

// 선택온도를 올린다. (99 초과 시, 0 으로 넘어간다.)
void selectedTempUp() {
	selected_temp++;
	if (selected_temp > 99) {
		selected_temp = 0;
	}
	tempScreen(selected_temp);
}

// 선택온도를 내린다. (0 미만 시, 99 로 넘어간다.)
void selectedTempDown() {
	selected_temp--;
	if (selected_temp < 0) {
		selected_temp = 99;
	}
	tempScreen(selected_temp);
}

/*
 * 현재온도 정보를 받아와 설정온도와 비교하여 Relay 를 제어한다.
 * 설정온도보다 현재온도가 더 낮다면, Relay ON 한다.
 * 설정온도-RANGE 보다 현재온도가 더 높다면 Relay OFF 한다.
 * 난방실 온도가 올라가는 시간이 더 빠르기 때문에 RANGE 라는
 * 전역변수를 활용해 기존 설정온도보다 낮게 하여 제어한다.
 */
void relayControl(float current_temp) {
	if ((int) current_temp < setting_temp) {
		relayOnOff(ON_t);
	} else if ((int) current_temp >= (setting_temp - SELECTED_TEMP_RANGE)) {
		relayOnOff(OFF_t);
	}
}

/*
 * 열거형 변수값에 따라 Relay 를 제어하고 Relay 상태를 변수에 저장한다.
 * Relay 가 ON 이라면, workScreen 함수에서 ON 을 표시하고,
 * Relay 가 OFF 이라면, workScreen 함수에서 OFF 을 표시한다.
 */
void relayOnOff(ON_OFF_t on_off) {
	HAL_GPIO_WritePin(PB5_RELAY_CTRL_GPIO_Port, PB5_RELAY_CTRL_Pin, on_off);
	relay_state = on_off;

	if (on_off == ON_t) {
		workScreen(on_off);
	} else if (on_off == OFF_t) {
		workScreen(on_off);
	}
}

// relayOnOff 함수에서 상태를 저장한 변수를 반환한다.
ON_OFF_t getRelayState() {
	return relay_state;
}
