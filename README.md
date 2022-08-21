## 난방실 온도제어 시스템
* 개발기간 : 2022.03-2022.08 (6개월)
* 개발인원 : 1명 (개인 프로젝트)
* 개발환경 : STM32CubeIDE 1.9.0
* 개발언어 : C
* 개발목표  
STM32 MCU에서 센서의 온도정보를 호출하여 현재온도를 FND 모듈에 표시한다.  
3개의 버튼과 OLED 모듈을 통해 설정온도를 선택하고, RELAY SW의 상태정보를 OLED 모듈을 통해 표시한다.  
설정온도와 현재온도를 비교하여 RELAY SW를 제어하고 설정온도를 유지하고 관리한다.  

## MCU 정보
### Microcontroller
* Part : STM32F103C8T6
* Manufacturer : ST-Microelectronics
* Core : Arm Cortex-M3
* Clock Speed(MAX) : 72MHz
* Package : LQFP 48 pin

### Internal memories
* FLASH : 64KB
* SRAM : 20KB

### Power
* sources
 * Any +3.3V pin (+3.3V)
 * Any +5V pin (+5V)
 * USB connector (+5V)


<a href="#"><img src="https://github.com/hmh2683/heatingroom.project/blob/main/images/KakaoTalk_20220820_213332997.jpg" width="400px" alt="sample image"></a> 


### 코드리뷰!

```javascript 
function test() { 
 console.log("hello world!"); 
} 
```

