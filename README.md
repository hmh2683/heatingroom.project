# Heating room temperature control system
* Period : 2022.03-2022.08 (6 months)
* Personnel : 1 person
* Development environment : STM32CubeIDE 1.9.0
* Development language : C
* Development goals  
  * The current temperature is displayed on the FND module by calling the temperature information of the sensor from the STM32
  * Select the set temperature through 3 buttons and OLED module, and display status information of relay through OLED module
  * Control relay by comparing set temperature with current temperature and maintain and manage set temperature
<br/> <br/>

## Microcontroller
<a href="#"><img src="https://github.com/hmh2683/heatingroom.project/blob/main/images/stm32f103c8t6.png" width="350px" height="350px"></a> 
* Part : STM32F103C8T6
* Manufacturer : ST-Microelectronics
* Core : Arm Cortex-M3
* Clock Speed(MAX) : 72MHz
* Package : LQFP 48 pin
<br/> <br/>
## Pinout & Configuration
<a href="#"><img src="https://github.com/hmh2683/heatingroom.project/blob/main/images/pin.png" width="400px" height="400px"></a>

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

### Clock Configuration
<a href="#"><img src="https://github.com/hmh2683/heatingroom.project/blob/main/images/clock.png" width="1000px" height="400px"></a> 
<br/> <br/>
## Code
### Communication 
#### 1. SPI  
* Create the SPI function directly in software   
* Controls the clock by 1 bit from MSB
```C
void send(uint8_t X) {
	for (int i = 8; i >= 1; i--) {
		if (X & 0x80) {
			HAL_GPIO_WritePin(FND_DIO_GPIO_Port, FND_DIO_Pin, HIGH); 
		} else {
			HAL_GPIO_WritePin(FND_DIO_GPIO_Port, FND_DIO_Pin, LOW);
		}
		X <<= 1;
		HAL_GPIO_WritePin(FND_SCLK_GPIO_Port, FND_SCLK_Pin, LOW);
		HAL_GPIO_WritePin(FND_SCLK_GPIO_Port, FND_SCLK_Pin, HIGH);
	}
}
```
* Use the SPI function provided by STM32
```C
void send(uint8_t X) {
 HAL_SPI_Transmit(m_hspi, &X, 1, 100);
}
```
* Lower RCLK to LOW and raise it to HIGH again to send 16-bit information
```C
void sendPort(uint8_t X, uint8_t port) {
	send(X);
	send(port);
	HAL_GPIO_WritePin(PB14_FND_RCLK_GPIO_Port, PB14_FND_RCLK_Pin, LOW);
	HAL_GPIO_WritePin(PB14_FND_RCLK_GPIO_Port, PB14_FND_RCLK_Pin, HIGH);
}
```

#### 2. I2C 
<br/> <br/>
#### 4. ONEWIRE
#### 3. UART
* UART handler and transfer function provided by STM32 are used
* It is used to implement the printf function
```C
extern UART_HandleTypeDef *huart1;

int _write(int file, char *p, int len) {
	HAL_UART_Transmit(&huart1, (uint8_t*) p, len, 10);
	return len;
}
```
#### 4. ONEWIRE

### Main
<br/> <br/>
## Result
[![Heating room temperature control system](http://img.youtube.com/vi/CozpDULafgo/0.jpg)](https://youtu.be/CozpDULafgo?t=0s) 
