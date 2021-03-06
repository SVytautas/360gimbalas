#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"


#define SIZE_STEPS_ARR_MAX 64

typedef struct
{
	uint32_t vertical_steps_size;
	float vertical_degs_pozs[SIZE_STEPS_ARR_MAX]; //vertikalios pozicijos 0 atkaitos -90;0;90 iki 64
	float horizontal_counts_pozs[SIZE_STEPS_ARR_MAX]; //horizontalios pozicijos counteriai kiek kiekviename vertikaliame
	
  uint32_t horizontal_steps_pozs_temp[SIZE_STEPS_ARR_MAX];
  uint32_t vertical_steps_pozs_temp[SIZE_STEPS_ARR_MAX];	
}vert_horz_t;



//#define STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL 1200   //100hz skaitimas
//#define STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL 1200*6   //100hz skaitimas
#define STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL 7800   //100hz skaitimas
#define DEG_STEPS (float)STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL/360.0f
//#define STEPPER_CONTROLS_STEPS_VERTICAL_FULL 430  //100hz skaitimas 2sekundes
//#define STEPPER_CONTROLS_STEPS_VERTICAL_MIDLE 288


//#define STEPPER_CONTROLS_END_HORIZONTAL() 	(HAL_GPIO_ReadPin(END_ST1_GPIO_Port, END_ST1_Pin))
#define STEPPER_CONTROLS_END_HORIZONTAL() 	(!HAL_GPIO_ReadPin(END_ST2_GPIO_Port, END_ST2_Pin))
#define STEPPER_CONTROLS_END_VERTICAL() 		(!HAL_GPIO_ReadPin(END_ST1_GPIO_Port, END_ST1_Pin))

//step kontroliavimas ST1 horizontalus
#define STEPPER_CONTROLS_ST1_ENABLE();			HAL_GPIO_WritePin(ST1_ENA_GPIO_Port, ST1_ENA_Pin, GPIO_PIN_RESET);
#define STEPPER_CONTROLS_ST1_DISABLE();			HAL_GPIO_WritePin(ST1_ENA_GPIO_Port, ST1_ENA_Pin, GPIO_PIN_SET);

//#define STEPPER_CONTROLS_ST1_DIR_CW();			HAL_GPIO_WritePin(ST1_DIR_GPIO_Port, ST1_DIR_Pin, GPIO_PIN_RESET);
//#define STEPPER_CONTROLS_ST1_DIR_CCW();			HAL_GPIO_WritePin(ST1_DIR_GPIO_Port, ST1_DIR_Pin, GPIO_PIN_SET);
#define STEPPER_CONTROLS_ST1_DIR_CW();			HAL_GPIO_WritePin(ST1_DIR_GPIO_Port, ST1_DIR_Pin, GPIO_PIN_SET);
#define STEPPER_CONTROLS_ST1_DIR_CCW();			HAL_GPIO_WritePin(ST1_DIR_GPIO_Port, ST1_DIR_Pin, GPIO_PIN_RESET);

#define STEPPER_CONTROLS_ST1_STEP_H();			HAL_GPIO_WritePin(ST1_STEP_GPIO_Port, ST1_STEP_Pin, GPIO_PIN_SET);
#define STEPPER_CONTROLS_ST1_STEP_L();			HAL_GPIO_WritePin(ST1_STEP_GPIO_Port, ST1_STEP_Pin, GPIO_PIN_RESET);
#define STEPPER_CONTROLS_ST1_STEP_TOGGLE();			HAL_GPIO_TogglePin(ST1_STEP_GPIO_Port, ST1_STEP_Pin);

//step kontroliavimas ST2
#define STEPPER_CONTROLS_ST2_ENABLE();			HAL_GPIO_WritePin(ST2_ENA_GPIO_Port, ST2_ENA_Pin, GPIO_PIN_RESET);
#define STEPPER_CONTROLS_ST2_DISABLE();			HAL_GPIO_WritePin(ST2_ENA_GPIO_Port, ST2_ENA_Pin, GPIO_PIN_SET);

#define STEPPER_CONTROLS_ST2_DIR_CW();			HAL_GPIO_WritePin(ST2_DIR_GPIO_Port, ST2_DIR_Pin, GPIO_PIN_RESET);
#define STEPPER_CONTROLS_ST2_DIR_CCW();			HAL_GPIO_WritePin(ST2_DIR_GPIO_Port, ST2_DIR_Pin, GPIO_PIN_SET);

#define STEPPER_CONTROLS_ST2_STEP_H();			HAL_GPIO_WritePin(ST2_STEP_GPIO_Port, ST2_STEP_Pin, GPIO_PIN_SET);
#define STEPPER_CONTROLS_ST2_STEP_L();			HAL_GPIO_WritePin(ST2_STEP_GPIO_Port, ST2_STEP_Pin, GPIO_PIN_RESET);
#define STEPPER_CONTROLS_ST2_STEP_TOGGLE();	HAL_GPIO_TogglePin(ST2_STEP_GPIO_Port, ST2_STEP_Pin);


void pack_to_bag(void);
void measure_0(void);
void STEPPER_CONTROLS_init(void);
void STEPPER_CONTROLS_handler_1kHz(void);
void STEPPER_CONTROLS_handler_5kHz(void);
void STEPPER_CONTROLS_handler(void);

void STEPPER_CONTROLS_set_start(uint32_t shoot_time_ms_,uint32_t wait_time_ms_, uint32_t step_vertical_, uint32_t step_horizontal_);