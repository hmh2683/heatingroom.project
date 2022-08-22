## 난방실 온도제어 시스템
* 개발기간 : 2022.03-2022.08 (6개월)
* 개발인원 : 1명 (개인 프로젝트)
* 개발환경 : STM32CubeIDE 1.9.0
* 개발언어 : C
* 개발목표  
STM32 MCU에서 센서의 온도정보를 호출하여 현재온도를 FND 모듈에 표시한다.  
3개의 버튼과 OLED 모듈을 통해 설정온도를 선택하고, RELAY 의 상태정보를 OLED 모듈을 통해 표시한다.  
설정온도와 현재온도를 비교하여 RELAY 를 제어하고 설정온도를 유지하고 관리한다.  
 
## Microcontroller
<a href="#"><img src="https://github.com/hmh2683/heatingroom.project/blob/main/images/stm32f103c8t6.png" width="300px" height="300px"></a> 
* Part : STM32F103C8T6
* Manufacturer : ST-Microelectronics
* Core : Arm Cortex-M3
* Clock Speed(MAX) : 72MHz
* Package : LQFP 48 pin

## Pinout & Configuration
<a href="#"><img src="https://github.com/hmh2683/heatingroom.project/blob/main/images/pin.png" width="400px" height="400px"></a> 
<a href="#"><img src="https://github.com/hmh2683/heatingroom.project/blob/main/images/clock.png" width="400px" height="200px"></a> 
### System Core
* GPIO -> INPUT -> PB0_TEMP_UP, PB1_TEMP_SET, PB2_TEMP_DOWN, PB12_START_SW
* GPIO -> OUTPUT -> PA3_TEMP_DATA, PB5_RELAY, PB6_BUTTON_LED, PB7_START_LED, PB14_FND_RCLK
* SPI -> PB13_FND_SCLK, PB15_FND_DIO
* I2C -> PB10_I2C2_SCL, PB11_I2C2_SDA
* SYS -> PA13_JTMS-SWDIO, PA14_SYS_JTCK-SWCLK
* RCC -> PC14_RCC_OSC32_IN, PC15_RCC_OSC32_OUT, PD0_RCC_OSC_IN, PD1_RCC_OSC_OUT
### Timers
* TIM2 -> Temperature sensor
  * Prescaler : 71
  * Counter Period : 65535
* TIM3 -> FND
  * Prescaler : 71
  * Counter Period : 99
### Connectivity
* SPI2
  * Data Size : 8bit
  * First Bit : MSB
  * Prescaler : 16
  * Clock Polarity : High
  * Clock Phase : 1 Edge
* I2C2
  * Speed Mode : Fast Mode
  * Clock Speed : 400000 Hz
* USART1
  * Baud Rate : 115200 Bits/s
  * Word Length : 8 Bit

## Code
### Communication 
* SPI
* I2C 
* UART
* ONEWIRE
```javascript 
function test() { 
 console.log("hello world!"); 
} 
```

