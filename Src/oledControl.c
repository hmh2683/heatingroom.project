#include "oledControl.h"
#include "heatingControl.h"

#include "ssd1306.h"
#include "fonts.h"
#include "bitmap.h"

/*
 * OLED 화면에 LOGO를 2초간 보여주고 1초마다 반전한다. 그 후, 난방실 준비 및 시작을 알리고
 * 기본 스크린, 온도 스크린, 상태 스크린을 합친 화면이 출력된다.
 */
void Screen_Init() {
	SSD1306_DrawBitmap(0, 0, logo, 128, 64, 1);
	SSD1306_UpdateScreen();
	HAL_Delay(2000);

	SSD1306_InvertDisplay(1);
	HAL_Delay(1000);
	SSD1306_InvertDisplay(0);
	HAL_Delay(1000);
	SSD1306_InvertDisplay(1);
	HAL_Delay(1000);
	SSD1306_InvertDisplay(0);
	HAL_Delay(1000);

	SSD1306_Clear();

	SSD1306_GotoXY(0, 0);
	SSD1306_Puts("HEATING", &Font_11x18, 1);

	SSD1306_GotoXY(30, 20);
	SSD1306_Puts("ROOM", &Font_11x18, 1);

	SSD1306_GotoXY(45, 40);
	SSD1306_Puts("READY..", &Font_11x18, 1);
	SSD1306_UpdateScreen();

	SSD1306_ScrollRight(0x00, 0x0f);
	HAL_Delay(2500);
	SSD1306_ScrollLeft(0x00, 0x0f);
	HAL_Delay(2500);

	SSD1306_Stopscroll();
	SSD1306_Clear();

	SSD1306_GotoXY(0, 0);
	SSD1306_Puts("HEATING", &Font_11x18, 1);
	SSD1306_GotoXY(30, 20);
	SSD1306_Puts("ROOM", &Font_11x18, 1);
	SSD1306_GotoXY(45, 40);
	SSD1306_Puts("START!!", &Font_11x18, 1);
	SSD1306_UpdateScreen();
	HAL_Delay(2000);

	SSD1306_Clear();

	defaultScreen();
	tempScreen(getSelectedTemp());
	workScreen(getRelayState());
}

// 화면을 지우고 기본, 온도, 상태 스크린이 출력된다.
void settingScreen() {
	for (int i = 0; i < 4; i++) {
		if (i % 2 == 0) {
			SSD1306_Clear();
			SSD1306_UpdateScreen();
		} else {
			defaultScreen();
			tempScreen(getSelectedTemp());
			workScreen(getRelayState());
		}
		HAL_Delay(250);
	}
}

// 온도정보와 상태정보를 보여줄 수 있는 기본 스크린을 출력한다.
void defaultScreen() {
	SSD1306_GotoXY(0, 0);
	SSD1306_Puts(" TEMP WORK ", &Font_11x18, 1);
	SSD1306_GotoXY(0, 15);
	SSD1306_Puts("___________", &Font_11x18, 1);
}

// 정수형 온도변수를 받아와 문자열로 변환 후 OLED에 출력한다.
void tempScreen(int temp) {
	char str[50] = "";
	sprintf(str, "%2d.0", temp);

	SSD1306_GotoXY(10, 40);
	SSD1306_Puts(str, &Font_11x18, 1);
	SSD1306_UpdateScreen();
}

// 열거형 상태변수를 받아와 릴레이의 상태를 출력한다.
void workScreen(ON_OFF_t on_off) {
	SSD1306_GotoXY(80, 40);
	if (on_off == ON_t) {
		SSD1306_Puts("ON ", &Font_11x18, 1);
	} else if (on_off == OFF_t) {
		SSD1306_Puts("OFF", &Font_11x18, 1);
	}
	SSD1306_UpdateScreen();
}

// 선택온도를 낮추거나 올릴 수 있는 버튼의 LED가 20ms 동안 점멸한다.
void buttonLed() {
	HAL_GPIO_TogglePin(PB6_BUTTON_LED_GPIO_Port, PB6_BUTTON_LED_Pin);
	HAL_Delay(20);
	HAL_GPIO_TogglePin(PB6_BUTTON_LED_GPIO_Port, PB6_BUTTON_LED_Pin);
}

// 설정온도를 지정하면 LED가 켜지고, 설정온도를 바꾸게 되면 LED는 꺼진다.
void buttonSetLed() {
	HAL_GPIO_TogglePin(PB6_BUTTON_LED_GPIO_Port, PB6_BUTTON_LED_Pin);
}

// START SW 상태 값에 따라 LED가 켜지거나 꺼진다.
void startSwLed(ON_OFF_t on_off) {
	if (on_off == ON_t) {
		HAL_GPIO_WritePin(PB7_START_SW_LED_GPIO_Port, PB7_START_SW_LED_Pin,
				on_off);
	} else if (on_off == OFF_t) {
		HAL_GPIO_WritePin(PB7_START_SW_LED_GPIO_Port, PB7_START_SW_LED_Pin,
				on_off);
	}
}
