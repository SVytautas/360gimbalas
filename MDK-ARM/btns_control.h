#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

#define GPIO_BTN_DEBOUNCE 5  //100hz skaitimas
#define GPIO_BTN_LONG 120  //100hz skaitimas 2sekundes

#define GPIO_BTN_STAICIUS 3

//#define GPIO_BTN_STM_READ() 				(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8))
#define BTNS_CONTROL_BTN1() 				(HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin))
#define BTNS_CONTROL_BTN2() 				(HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin))


//LEDAI------------------------------
#define BTNS_CONTROL_LED1_ON();				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
#define BTNS_CONTROL_LED1_OFF();			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
#define BTNS_CONTROL_LED1_TOGGLE();		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

#define BTNS_CONTROL_LED2_ON();				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
#define BTNS_CONTROL_LED2_OFF();			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
#define BTNS_CONTROL_LED2_TOGGLE();		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

#define BTNS_CONTROL_LED3_ON();				HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
#define BTNS_CONTROL_LED3_OFF();			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
#define BTNS_CONTROL_LED3_TOGGLE();		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);

#define BTNS_CONTROL_LED4_ON();				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
#define BTNS_CONTROL_LED4_OFF();			HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
#define BTNS_CONTROL_LED4_TOGGLE();		HAL_GPIO_TogglePin(LED4_GPIO_Port, LED4_Pin);

#define BTNS_CONTROL_LED5_ON();				HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_RESET);
#define BTNS_CONTROL_LED5_OFF();			HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_SET);
#define BTNS_CONTROL_LED5_TOGGLE();		HAL_GPIO_TogglePin(LED5_GPIO_Port, LED5_Pin);


typedef struct
{
  uint32_t btn_count[GPIO_BTN_STAICIUS];
	uint32_t btn_count_long[GPIO_BTN_STAICIUS];
  bool 		 btn_status_[GPIO_BTN_STAICIUS];
  bool     btn_event[GPIO_BTN_STAICIUS];	
}gpio_btn_t;


enum {
BTNS_CONTROL_BTN1_PTR=0,  //counterio pionteris
BTNS_CONTROL_BTN2_PTR,
BTNS_CONTROL_BTN3_PTR,

};


enum {
BTNS_CONTROL_BTN1_SHORT=0,  //counterio pionteris
BTNS_CONTROL_BTN2_SHORT,
BTNS_CONTROL_BTN1_LONG,
BTNS_CONTROL_BTN2_LONG,

};




void BTNS_CONTROL_init(void);
void BTNS_CONTROL_handler_100Hz(void);
void BTNS_CONTROL_event_handler(uint8_t btn_event);