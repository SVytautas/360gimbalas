#include "main.h"
#include "uart2_dbg.h"
#include "btns_control.h"
#include "stepper_controls.h"

gpio_btn_t gpio_btn={
  .btn_count = {0,0,0},

};


uint32_t adc_value_now = 0;
uint32_t adc_ptr_now = 0;

uint32_t shoot_time_ms_select[5]={
		10000,
		5000,
		2200, //default
	  //2200, //default
		1000,
		500,
};

uint32_t wait_time_ms_seclect[5]={
		10000,
		5000,
		4000, //default
		1000,
		100
};

uint32_t step_vertical_ms_select[5]={
		1,
		3,
		5, //default
		7,
		9
};

uint32_t step_horizontal_ms_select[5]={
		15,
		20,
		12, //default
		35,
		36
};


uint32_t shoot_time_ms = 1000;
uint32_t wait_time_ms = 1000;
uint32_t step_vertical = 5;
uint32_t step_horizontal = 30;

uint32_t set_step = 0;
uint32_t set_step_size = 5; //5 start


//ADC
ADC_HandleTypeDef hadc1;
static void MX_ADC1_Init(void);
void BTNS_CONTROL_event_set_param(uint8_t set_step_);

void BTNS_CONTROL_init(void)
{
		MX_ADC1_Init();
}

static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
	
}



void BTNS_CONTROL_handler_100Hz(void)
{

	//pirmas mygtukas BTN1
	if (!BTNS_CONTROL_BTN1())
	{
		gpio_btn.btn_count[BTNS_CONTROL_BTN1_PTR]++;
			
		if (gpio_btn.btn_count[BTNS_CONTROL_BTN1_PTR]==GPIO_BTN_LONG)
		{
		   //PRINTF("btn stm long");
			 BTNS_CONTROL_event_handler(BTNS_CONTROL_BTN1_LONG);
		}
		
		//if 
	   
		 
	}
	else
  {
     if (gpio_btn.btn_count[BTNS_CONTROL_BTN1_PTR]> GPIO_BTN_DEBOUNCE && 
			 gpio_btn.btn_count[BTNS_CONTROL_BTN1_PTR]< GPIO_BTN_LONG
		 )
		{
			//PRINTF("btn stm short");
			BTNS_CONTROL_event_handler(BTNS_CONTROL_BTN1_SHORT);
			gpio_btn.btn_count[BTNS_CONTROL_BTN1_PTR]=0;
		
		}
    else if (gpio_btn.btn_count[BTNS_CONTROL_BTN1_PTR]>=GPIO_BTN_LONG)
		{
		  gpio_btn.btn_count[BTNS_CONTROL_BTN1_PTR]=0;
		}
		else
		{
			if (gpio_btn.btn_count[BTNS_CONTROL_BTN1_PTR]>0)
			{
			  gpio_btn.btn_count[BTNS_CONTROL_BTN1_PTR]--;
			}
		  
		}			
	}
	
	
	//pirmas mygtukas BTN2
	if (!BTNS_CONTROL_BTN2())
	{
		gpio_btn.btn_count[BTNS_CONTROL_BTN2_PTR]++;
		
		if (gpio_btn.btn_count[BTNS_CONTROL_BTN2_PTR]==GPIO_BTN_LONG)
		{
		   //PRINTF("btn stm long");
			 BTNS_CONTROL_event_handler(BTNS_CONTROL_BTN2_LONG);
		}
		
	}
	else
  {
     if (gpio_btn.btn_count[BTNS_CONTROL_BTN2_PTR]> GPIO_BTN_DEBOUNCE && 
			 gpio_btn.btn_count[BTNS_CONTROL_BTN2_PTR]< GPIO_BTN_LONG
		 )
		{
			//PRINTF("btn stm short");
			BTNS_CONTROL_event_handler(BTNS_CONTROL_BTN2_SHORT);
			gpio_btn.btn_count[BTNS_CONTROL_BTN2_PTR]=0;
		
		}
    else if (gpio_btn.btn_count[BTNS_CONTROL_BTN2_PTR]>=GPIO_BTN_LONG)
		{
		  gpio_btn.btn_count[BTNS_CONTROL_BTN2_PTR]=0;
		}
		else
		{
			if (gpio_btn.btn_count[BTNS_CONTROL_BTN2_PTR]>0)
			{
			  gpio_btn.btn_count[BTNS_CONTROL_BTN2_PTR]--;
			}
		  
		}			
	}
	
	
	  HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,1);
		//PRINTF("adc %u\r\n",HAL_ADC_GetValue(&hadc1));
	  adc_value_now = (adc_value_now + HAL_ADC_GetValue(&hadc1))/2;
		HAL_ADC_Stop(&hadc1);
	
	  if (adc_value_now<=100)
		{
			adc_ptr_now=4;
		}
		else if (adc_value_now>100 && adc_value_now<=1200)
		{
			adc_ptr_now=3;
		}
		else if (adc_value_now>1200 && adc_value_now<=2600)
		{
			adc_ptr_now=2;
		}
		else if (adc_value_now>2600 && adc_value_now<=3900)
		{
			adc_ptr_now=1;
		}
		else 
		{
			adc_ptr_now=0;
		}
		
		//PRINTF("adc %u\r\n",adc_ptr_now);
		BTNS_CONTROL_event_set_param(set_step);
}




void BTNS_CONTROL_event_handler(uint8_t btn_event)
{
		switch (btn_event)
		{
			case BTNS_CONTROL_BTN1_SHORT:
			PRINTF("btn1 short\r\n");	
			set_step++;
			if (set_step>=set_step_size)
			{
				set_step=0;
			}
			break;
			
			case BTNS_CONTROL_BTN2_SHORT:
			PRINTF("btn2 short\r\n");
			if (set_step == 4) //jeigu ant ready pradedam 
			{
				set_step++;
				//PRINTF("start sh:%u wai:%u ver:%u hor:%u\r\n",shoot_time_ms,wait_time_ms,step_vertical,step_horizontal );
				BTNS_CONTROL_LED1_OFF();
				BTNS_CONTROL_LED2_OFF();
				BTNS_CONTROL_LED3_OFF();
				BTNS_CONTROL_LED4_OFF();
				BTNS_CONTROL_LED5_OFF();
				STEPPER_CONTROLS_set_start(shoot_time_ms,wait_time_ms,step_vertical,step_horizontal);
				
			}
			
			break;
			
			case BTNS_CONTROL_BTN2_LONG:
				pack_to_bag();
			break;
			
			case BTNS_CONTROL_BTN1_LONG:
				measure_0();
			break;
		
		  default:
				break;
		
		}


}

uint16_t toggle_efect = 0;
void BTNS_CONTROL_event_set_param(uint8_t set_step_)
{
		switch (set_step_)
		{
			case 0: //shoot time ms
			shoot_time_ms = shoot_time_ms_select[adc_ptr_now];
			BTNS_CONTROL_LED1_OFF();
			BTNS_CONTROL_LED2_OFF();
			BTNS_CONTROL_LED3_OFF();
			BTNS_CONTROL_LED4_OFF();
			BTNS_CONTROL_LED5_OFF();
			
			BTNS_CONTROL_LED1_ON();
			break;
			
			case 1: //wait time  ms
			wait_time_ms = wait_time_ms_seclect[adc_ptr_now];
			BTNS_CONTROL_LED1_OFF();
			BTNS_CONTROL_LED2_OFF();
			BTNS_CONTROL_LED3_OFF();
			BTNS_CONTROL_LED4_OFF();
			BTNS_CONTROL_LED5_OFF();
			
			BTNS_CONTROL_LED2_ON();
			break;
			
			case 2: //vertical steps
			step_vertical = step_vertical_ms_select[adc_ptr_now];
			BTNS_CONTROL_LED1_OFF();
			BTNS_CONTROL_LED2_OFF();
			BTNS_CONTROL_LED3_OFF();
			BTNS_CONTROL_LED4_OFF();
			BTNS_CONTROL_LED5_OFF();
			
			BTNS_CONTROL_LED3_ON();
			break;
			
			case 3: //horizontal steps
			step_horizontal	= step_horizontal_ms_select[adc_ptr_now];
			BTNS_CONTROL_LED1_OFF();
			BTNS_CONTROL_LED2_OFF();
			BTNS_CONTROL_LED3_OFF();
			BTNS_CONTROL_LED4_OFF();
			BTNS_CONTROL_LED5_OFF();
			
			BTNS_CONTROL_LED4_ON();
			break;
			
			case 4: //ready to excecute
				BTNS_CONTROL_LED1_OFF();
			BTNS_CONTROL_LED2_OFF();
			BTNS_CONTROL_LED3_OFF();
			BTNS_CONTROL_LED4_OFF();
			BTNS_CONTROL_LED5_OFF();
			
			BTNS_CONTROL_LED5_ON();
			break;
			
			case 5: //exsecuting
			toggle_efect++;
			if (toggle_efect>=50)
			{
				toggle_efect=0;
				BTNS_CONTROL_LED3_TOGGLE();	
			}
					
			
			break;
			
		
		  default:
				break;
		
		}


}





