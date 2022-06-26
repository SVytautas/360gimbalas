#include "main.h"
#include "uart2_dbg.h"
#include "btns_control.h"
#include "stepper_controls.h"
#include "IR_control.h"


int _freq = 40;
/**
 * @brief  This function provides a delay (in microseconds)
 * @param  microseconds: delay in microseconds
 */
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t microseconds)
{
  uint32_t clk_cycle_start = DWT->CYCCNT;
 
  /* Go to number of cycles for system */
  microseconds *= (HAL_RCC_GetHCLKFreq() / 1000000);
 
  /* Delay till end */
  while ((DWT->CYCCNT - clk_cycle_start) < microseconds);
}


void high(unsigned int time, int freq){
  int pause = (1000/freq/2);
	
	uint32_t clk_cycle_start = DWT->CYCCNT;
	time *= (HAL_RCC_GetHCLKFreq() / 1000000);
	
  while(DWT->CYCCNT-clk_cycle_start<=time){
		
    //digitalWrite(pinLED,HIGH); //on
		HAL_GPIO_WritePin(Cam_Realise_GPIO_Port, Cam_Realise_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Cam_Focus_GPIO_Port, Cam_Focus_Pin, GPIO_PIN_SET);
		
    DWT_Delay_us(pause);
		
		 //digitalWrite(pinLED,LOW); //off
		HAL_GPIO_WritePin(Cam_Realise_GPIO_Port, Cam_Realise_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Cam_Focus_GPIO_Port, Cam_Focus_Pin, GPIO_PIN_RESET);
   
    DWT_Delay_us(pause);
		
  }
}

void SonyShotNow()
{
  bool _seq[] = {1,0,1,1,0,1,0,0,1,0,1,1,1,0,0,0,1,1,1,1  };
	//bool _seq[] = {1,1,1,1,0,0,0,1,1,1,0,1,0,0,1,0,1,1,0,1 }; //rev
  high(2400,_freq); //header 2400us
	DWT_Delay_us(600); //space no output	
  for (int i=0;i<sizeof(_seq);i++){
    if (_seq[i]==0){
      high(600,_freq);
      DWT_Delay_us(600);
    }
    else{
      high(1200,_freq);
      DWT_Delay_us(600);
    }
  };
}

void SonyShotDelayed()
{
  bool _seqDelayed[] = {
    1,1,1,0,1,1,0,0,1,0,1,1,1,0,0,0,1,1,1,1  };
  high(2320,_freq);
  for (int i=0;i<sizeof(_seqDelayed);i++){
    if (_seqDelayed[i]==0){
      high(575,_freq);
      DWT_Delay_us(650);
    }
    else{
      high(1175,_freq);
      DWT_Delay_us(650);
    }
  };
}





void IR_control_init()
{


}



