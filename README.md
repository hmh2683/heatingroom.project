# Heating room temperature control system
* Period : 2022.03-2022.08 (6 months)
* Personnel : 1 person
* Development environment : STM32CubeIDE 1.9.0
* Development language : C
* Development goals  
  * The current temperature is displayed on the FND module by calling the temperature information of the sensor from the STM32.
  * Select the set temperature through 3 buttons and OLED module, and display status information of relay through OLED module.
  * Control relay by comparing set temperature with current temperature and maintain and manage set temperature.

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
* TIM2 -> TEMPSENSOR
  * Prescaler : 71
  * Counter Period : 65535
* TIM3 -> FND
  * Prescaler : 71
  * Counter Period : 99

### Connectivity
* SPI2 -> FND
  * Data Size : 8bit
  * First Bit : MSB
  * Prescaler : 16
  * Clock Polarity : High
  * Clock Phase : 1 Edge
* I2C2 -> OLED
  * Speed Mode : Fast Mode
  * Clock Speed : 400000 Hz
* USART1 -> FTDI
  * Baud Rate : 115200 Bits/s
  * Word Length : 8 Bit
* ONEWIRE -> SENSOR
### Clock Configuration
<a href="#"><img src="https://github.com/hmh2683/heatingroom.project/blob/main/images/clock.png" width="1000px" height="400px"></a> 

<br/> <br/>

## Code review
### Main
* The if statement is executed according to the button variable that is interrupted.
* LED is controlled according to the state of the start switch.
* Start temperature conversion and get temperature value.
* If the switch is on, the relay is controlled according to the temperature value. If the switch is off, the relay is turned off regardless of the temperature value.
```C
while (1) {
	checkButton();
	checkStartSw();

	if (!isConverted()) 
		startConvert();
	
	checkConvert();
	if (!isConverted()) {
		temperature = getTemp();
		if (getStartSw() == ON_t) {
			relayControl(temperature);
		} else {
			if (getRelayState() == ON_t) 
				relayOnOff(OFF_t);
		}
	}
}
```

### Interrupt
* 인터럽트가 걸렸을 때, 버튼의 상태 값을 변환시킨다.
* HAL_GetTick() 함수를 사용 
```C
void EXTI0_IRQHandler(void) {
  HAL_GPIO_EXTI_IRQHandler(PB0_TEMP_UP_BUTTON_Pin);

	if (HAL_GetTick() - before_time > CLICK_TIME) {
		button_up = 1;
	}
	before_time = HAL_GetTick();
}
```
* 
* Check the initialization status and busy status, and if true, get the current temperature value and display it on the FND.
```C
void TIM3_IRQHandler(void) {
  HAL_TIM_IRQHandler(&htim3);
  
	if (isTempSensorInit() && !isBusy()) {
		digitTemp((int)(getCurrentTemp() * 10));
	}
}
```

### Communication 
#### 1. SPI  
* Create the SPI function directly in software.
* Controls the clock by 1 bit from MSB.
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
* Use the SPI function provided by STM32.
```C
static SPI_HandleTypeDef *fhspi;

void send(uint8_t X) {
 	HAL_SPI_Transmit(fhspi, &X, 1, 100);
}
```
* Lower RCLK to LOW and raise it to HIGH again to send 16-bit information.
```C
void sendPort(uint8_t X, uint8_t port) {
	send(X);
	send(port);
	HAL_GPIO_WritePin(PB14_FND_RCLK_GPIO_Port, PB14_FND_RCLK_Pin, LOW);
	HAL_GPIO_WritePin(PB14_FND_RCLK_GPIO_Port, PB14_FND_RCLK_Pin, HIGH);
}
```

#### 2. I2C 
*
```C

```
*
```C

```

#### 3. UART
* UART handler and transfer function provided by STM32 are used.
* It is used to implement the printf function.
```C
extern UART_HandleTypeDef *huart1;

int _write(int file, char *p, int len) {
	HAL_UART_Transmit(&huart1, (uint8_t*) p, len, 10);
	return len;
}
```
#### 4. ONEWIRE
* If read bit value is low, initialization is successful.
```C
inline uint8_t OneWire_Reset(OneWire_t *OneWireStruct) {
	uint8_t i;

	ONEWIRE_LOW(OneWireStruct);
	ONEWIRE_OUTPUT(OneWireStruct);
	ONEWIRE_DELAY(480);
	ONEWIRE_DELAY(20);
	ONEWIRE_INPUT(OneWireStruct);
	ONEWIRE_DELAY(70);
	i = HAL_GPIO_ReadPin(OneWireStruct->GPIOx, OneWireStruct->GPIO_Pin);
	ONEWIRE_DELAY(410);
	
	return i;
}
```
* Send command information to slave device.
```C
inline void OneWire_WriteBit(OneWire_t *OneWireStruct, uint8_t bit) {
	if (bit) {
		ONEWIRE_LOW(OneWireStruct);
		ONEWIRE_OUTPUT(OneWireStruct);
		ONEWIRE_DELAY(10);
		ONEWIRE_INPUT(OneWireStruct);	
		ONEWIRE_DELAY(55);
		ONEWIRE_INPUT(OneWireStruct);
	} else {
		ONEWIRE_LOW(OneWireStruct);
		ONEWIRE_OUTPUT(OneWireStruct);
		ONEWIRE_DELAY(65);
		ONEWIRE_INPUT(OneWireStruct);
		ONEWIRE_DELAY(5);
		ONEWIRE_INPUT(OneWireStruct);
	}
}
```
* If read bit is low, then device is not finished yet with calculation temperature.
```C
inline uint8_t OneWire_ReadBit(OneWire_t *OneWireStruct) {
	uint8_t bit = 0;

	ONEWIRE_LOW(OneWireStruct);
	ONEWIRE_OUTPUT(OneWireStruct);
	ONEWIRE_DELAY(2);
	ONEWIRE_INPUT(OneWireStruct);
	ONEWIRE_DELAY(10);	
	if (HAL_GPIO_ReadPin(OneWireStruct->GPIOx, OneWireStruct->GPIO_Pin)) {
		bit = 1;
	}
	ONEWIRE_DELAY(50);

	return bit;
}
```
* Select ROM number.
```C
void OneWire_SelectWithPointer(OneWire_t *OneWireStruct, uint8_t *ROM) {
	uint8_t i;
	
	OneWire_WriteByte(OneWireStruct, ONEWIRE_CMD_MATCHROM);
	for (i = 0; i < 8; i++) {
		OneWire_WriteByte(OneWireStruct, *(ROM + i));
	}
}
```
<br/> <br/>

## Result
[![Heating room temperature control system](http://img.youtube.com/vi/CozpDULafgo/0.jpg)](https://youtu.be/CozpDULafgo?t=0s) 
