#include "buttonControl.h"
#include "oledControl.h"
#include "heatingControl.h"

extern UART_HandleTypeDef *huart1;

int _write(int file, char *p, int len) {
	HAL_UART_Transmit(&huart1, (uint8_t*) p, len, 10);
	return len;
}

/*
 * interrupt 를 통해 받아온 버튼 상태 값에 따라 OLED의 선택온도를 올리거나 낮춘다. 또한,
 * 선택온도를 지정하면 OLED 화면이 2회 점멸하고 설정온도를 지정한다. 설정온도를 바꾸기 위해서는
 * 설정했던 버튼을 한 번더 클릭하여 선택온도를 지정할 수 있다.
 */
void checkButton() {
	static char button_set_flag = 0;

	if (button_up == 1 && button_set_flag == 0) {
		button_up = 0;
		selectedTempUp();
		buttonLed();
		printf("Heating room temperature : %d.0\r\n", getSelectedTemp());
	}
	if (button_down == 1 && button_set_flag == 0) {
		button_down = 0;
		selectedTempDown();
		buttonLed();
		printf("Heating room temperature : %d.0\r\n", getSelectedTemp());
	}
	if (button_set == 1 && button_set_flag == 0) {
		button_set = 0;
		button_set_flag = 1;
		setSettingTemp();
		settingScreen();
		buttonSetLed();
		printf("Confirmation of heating room temperature\r\n");
	}
	if (button_set == 1 && button_set_flag == 1) {
		button_set = 0;
		button_set_flag = 0;
		buttonSetLed();
		printf("Cancel heating room temperature\r\n");
	}
}

// START SW 상태 값에 따라 LED가 켜지거나 꺼진다.
void checkStartSw() {
	static int start_sw_time = 0;

	if (start_sw_time > START_SW_CHECK_TIME) {
		if (getStartSw() == ON_t) {
			startSwLed(ON_t);
		} else {
			startSwLed(OFF_t);
		}
		start_sw_time = 0;
	}
	start_sw_time++;

}

// START SW 상태가 0 이라면 ON_t, 1 이라면 OFF_t를 반환한다.
ON_OFF_t getStartSw() {
	if (!HAL_GPIO_ReadPin(PB12_START_SW_GPIO_Port, PB12_START_SW_Pin)) {
		return ON_t;
	} else {
		return OFF_t;
	}
}
