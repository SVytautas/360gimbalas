#include "main.h"
#include "uart2_dbg.h"
#include "btns_control.h"
#include "stepper_controls.h"


uint32_t shoot_time_ms2 = 1000;
uint32_t wait_time_ms2 = 1000;
uint32_t step_vertical2 = 5;
//uint32_t step_horizontal2 = 30;
uint32_t step_horizontal2[5] = {4,10,12,10,4};

bool STEPPER_CONTROLS_horizontal_in_pos();
bool STEPPER_CONTROLS_vertical_in_pos();

void STEPPER_CONTROLS_init(void)
{

	//disablinam variklius




}

volatile int32_t cycle_to_exsecute = 0;
volatile int32_t cycle_state = 0;


volatile uint32_t home_horizontal=0; //nustatom daugiau negu pilnas apsisukimas *2

volatile uint32_t horizontal_to_step = 0;
volatile uint32_t horizontal_now_step = 0;

volatile uint32_t horizontal_angle_steps = 0;
//float horizontal_steps_angle = 0;
volatile float horizontal_steps_angle[5] = {0,0,0,0,0};

volatile uint32_t home_vertical=0; //nustatom daugiau negu pilnas apsisukimas *2

volatile uint32_t vertical_to_step = 0;
//117 vertikalus tai 72 36 0 36 72
//72-117 = 45laipsniai 1 laipsnis = 21,6666 982
#define angle_to_steps 21.66667f
#define start_angle 55
#define start_steps angle_to_steps*start_angle

volatile uint32_t vertical_to_step2[6] = {
0, //0
start_steps, //72
start_steps+780, //36
start_steps+780*2, //0
start_steps+780*3, //36
start_steps+780*4}; //72
volatile uint32_t vertical_now_step = 0;

volatile uint32_t vertical_angle_steps = 0;
volatile float vertical_steps_angle = 0;


void STEPPER_CONTROLS_set_start(uint32_t shoot_time_ms_,uint32_t wait_time_ms_, uint32_t step_vertical_, uint32_t step_horizontal_)
{
		cycle_to_exsecute = 1;
		
		shoot_time_ms2 = shoot_time_ms_;
		wait_time_ms2 = wait_time_ms_;
		step_vertical2 = step_vertical_;
		
	//step_horizontal2 = step_horizontal_;
	
		horizontal_to_step = 0;
	  horizontal_now_step = 0;
		horizontal_angle_steps = 0;
	
		//horizontal_steps_angle = (float)STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL/(float)step_horizontal2;
	  horizontal_steps_angle[0] = (float)STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL/(float)4;
	  horizontal_steps_angle[1] = (float)STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL/(float)10;
	  horizontal_steps_angle[2] = (float)STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL/(float)12;
	  horizontal_steps_angle[3] = (float)STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL/(float)10;
	  horizontal_steps_angle[4] = (float)STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL/(float)4;
	
		home_horizontal = 0;
		cycle_state = 0;
	
		
	  //nuo centro atidedame po kazkeik
		home_vertical = 0;
		vertical_steps_angle = (float)(STEPPER_CONTROLS_STEPS_VERTICAL_FULL/2)/(float)(((step_vertical2-1)/2)+1);
	
		vertical_to_step = 0;
		vertical_now_step = 0;
		vertical_angle_steps = 0;
}



void STEPPER_CONTROLS_handler(void)
{
//while 
			if (cycle_to_exsecute>0)
			{
					switch (cycle_state)
					{
						case 0:
							//hominam asis
							home_horizontal = STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL * 2 + STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL/3;
							//hominam vertikalia asi
							home_vertical = STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL * 2 + STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL/3 ; //tiek pat kiek ir horiszontiala pilna apsisukima ir dar biskiai jei kas.
							//PRINTF("a");
							cycle_state++;
						break;
						
						case 1:
						//laukiam kol home abi asys
							if (home_horizontal==0 && home_vertical==0)
							{
								cycle_state++;
							}
						break;
							
						case 2:
						//nustatom pakelti vertikalia kiek reikia	
						if (vertical_angle_steps == step_vertical2)
						{
						  cycle_to_exsecute = 0;
							//diablinam variklius
							//STEPPER_CONTROLS_ST2_DISABLE();
							//STEPPER_CONTROLS_ST1_DISABLE();
							
							home_horizontal = STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL * 2 + STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL/3;
							//hominam vertikalia asi
							home_vertical = STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL * 2 + STEPPER_CONTROLS_STEPS_HORIZONTAL_FULL/3 ; //tiek pat kiek ir horiszontiala pilna apsisukima ir dar biskiai jei kas.
							PRINTF("end");
							break;
							
						}
						
						vertical_angle_steps++;
						//vertical_to_step = (uint32_t)((float)(vertical_angle_steps) * vertical_steps_angle)*2;
						vertical_to_step = vertical_to_step2[vertical_angle_steps]*2;
						cycle_state++;
						break;
						
						case 3:
							//laukiam kol pakels vertikalia.
						if (STEPPER_CONTROLS_vertical_in_pos())
						{
							cycle_state++;
							horizontal_angle_steps = 0;
							horizontal_now_step = 0;
							horizontal_to_step = 0;
						  PRINTF("V pos %u\r\n",vertical_to_step/2);
							//PRINTF("V pos %u\r\n",vertical_to_step[vertical_angle_steps]);
							
							//plaukiame nustatita laika
							//HAL_Delay(5000); //5s privertinai
							HAL_Delay(wait_time_ms2);
						
						}
						
						
						break;
						
						
						
						case 4:
						//fotkiname viena nuotrauka
						//shotter realise 
						HAL_GPIO_WritePin(Cam_Realise_GPIO_Port, Cam_Realise_Pin, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(Cam_Focus_GPIO_Port, Cam_Focus_Pin, GPIO_PIN_RESET);
						HAL_Delay(50);
						HAL_GPIO_WritePin(Cam_Realise_GPIO_Port, Cam_Realise_Pin, GPIO_PIN_SET);
						HAL_GPIO_WritePin(Cam_Focus_GPIO_Port, Cam_Focus_Pin, GPIO_PIN_SET);
						HAL_Delay(300);
						HAL_GPIO_WritePin(Cam_Realise_GPIO_Port, Cam_Realise_Pin, GPIO_PIN_RESET);
						HAL_GPIO_WritePin(Cam_Focus_GPIO_Port, Cam_Focus_Pin, GPIO_PIN_RESET);
						//
						PRINTF("H shoot pos %u\r\n",horizontal_to_step/2);
						//laukiame fotkinimo laika
						HAL_Delay(shoot_time_ms2);
						cycle_state++; 
						break;
						
						
						case 5: //pasukame horizontalu per kazkiek zingsniu
						  horizontal_angle_steps++;
							horizontal_to_step = (uint32_t)((float)horizontal_angle_steps * horizontal_steps_angle[vertical_angle_steps-1])*2;
							if (horizontal_angle_steps == step_horizontal2[vertical_angle_steps-1])
							{
									//end viena linija
								//cycle_to_exsecute = 0;
								//pereinam i pirma 
								cycle_state = 0;
								break;
							}								
						
							
							cycle_state++;
						break;
							
							
						case 6:
							//laukiame kol horizontalus atsistos i savo pozicija
							if (STEPPER_CONTROLS_horizontal_in_pos())
							{
								
								HAL_Delay(wait_time_ms2); //laukimo laikas ir i shoot;
								cycle_state = 4;
							}
						
						break;
					
					
						default:
							break;
					
					}
			
			
			}

}



bool STEPPER_CONTROLS_horizontal_in_pos()
{
	if (horizontal_to_step != horizontal_now_step)
	{
		return false;
	}
	
	return true;
}

bool STEPPER_CONTROLS_vertical_in_pos()
{
	if (vertical_to_step != vertical_now_step)
	{
		return false;
	}
	
	return true;
}
 
uint32_t speed_cntH=0;
#define deseleration_H_max 4*5  //4ms
#define deseleration_H_min 8  //2*5  //2ms //7per mazai 8ok gal
#define deseleration_H_speed 0.0625f //labai gerai
#define deseleartion_achievement_steps (deseleration_H_max-deseleration_H_min)/deseleration_H_speed
float deselerationH = deseleration_H_max;
void deselerationH_exe(void);
void deselerationH_exe(void)
{
	//simple accelaration decelartion
	//horizontal
	
	
	if (home_horizontal>0)
	{
		if (deselerationH>deseleration_H_min)
			{
				deselerationH-=deseleration_H_speed;
			}
	
	}
	else{
			if ((horizontal_to_step-horizontal_now_step) >deseleartion_achievement_steps)
			{
					if (deselerationH>deseleration_H_min)
					{
						deselerationH-=deseleration_H_speed;
					}
			}
			else if ((horizontal_to_step-horizontal_now_step) < deseleartion_achievement_steps)
			{
					if (deselerationH<deseleration_H_max)
					{
						deselerationH+=deseleration_H_speed;
					}
			
			}
	
	
	}

}

uint32_t speed_cntV=0;
#define deseleration_V_max 5*5  //4ms
#define deseleration_V_min 2*5  //2*5  //paliekam leta nekazka ir reikia pajudeti
#define deseleration_V_speed 0.0625f //
#define deseleartion_Vachievement_steps (deseleration_V_max-deseleration_V_min)/deseleration_V_speed
float deselerationV = deseleration_V_max;
void deselerationV_exe(void);
void deselerationV_exe(void)
{
	//simple accelaration decelartion
	//horizontal
	
	if (home_vertical>0)
	{
			if (deselerationV>deseleration_V_min)
			{
				deselerationV-=deseleration_V_speed;
			}
	}
	else{
	
		if ((vertical_to_step-vertical_now_step) >deseleartion_Vachievement_steps)
	{
			if (deselerationV>deseleration_V_min)
			{
				deselerationV-=deseleration_V_speed ;
			}
	}
	else if ((vertical_to_step-vertical_now_step) < deseleartion_Vachievement_steps)
	{
			if (deselerationV<deseleration_V_max)
			{
				deselerationV+=deseleration_V_speed ;
			}
	
	}
	
	}

}


void STEPPER_CONTROLS_handler_1kHz(void)
{
	
}


void STEPPER_CONTROLS_handler_5kHz(void)
{
	speed_cntH++;
	if (speed_cntH>=deselerationH)
	{
		speed_cntH=0;
		//PRINTF("a");
	//steper home position 
		if (home_horizontal>0)
		{
				home_horizontal--;
				deselerationH_exe();
				STEPPER_CONTROLS_ST1_DIR_CCW();
				STEPPER_CONTROLS_ST1_ENABLE();
				if (STEPPER_CONTROLS_END_HORIZONTAL())
				//if (home_horizontal==0)
				{
					PRINTF("hor home\r\n");
					home_horizontal=0;
					horizontal_now_step = 0;
					horizontal_to_step = 0;
					deselerationH = deseleration_H_max;
					STEPPER_CONTROLS_ST1_DISABLE();
				}
			
			
			STEPPER_CONTROLS_ST1_STEP_TOGGLE();
		}
		else
		{
			//vygdom zingius jei reikia
			if (horizontal_to_step != horizontal_now_step)
			{
				deselerationH_exe();
				horizontal_now_step++;
				STEPPER_CONTROLS_ST1_DIR_CW();
				STEPPER_CONTROLS_ST1_ENABLE();
				STEPPER_CONTROLS_ST1_STEP_TOGGLE();
			}
			else{
				
			STEPPER_CONTROLS_ST1_DISABLE(); //horizontaliam visada disabliname variklius laikyti nieko nereikia.
			}
		
		}
		
		
	}
	
	//*
	speed_cntV++;
	if (speed_cntV>=deselerationV)
	{
		speed_cntV=0;
		if (home_vertical>0)
		{
			home_vertical--;
			deselerationV_exe();
			STEPPER_CONTROLS_ST2_DIR_CCW();
			STEPPER_CONTROLS_ST2_ENABLE();
			if (STEPPER_CONTROLS_END_VERTICAL())
			{
				PRINTF("vertical home\r\n");
				home_vertical=0;
				deselerationV = deseleration_V_max;
				vertical_now_step = 0;
				vertical_to_step = 0;
				STEPPER_CONTROLS_ST2_DISABLE();
			}
			STEPPER_CONTROLS_ST2_STEP_TOGGLE();
		}
		else
		{
			//vygdom zingius jei reikia
			if (vertical_to_step != vertical_now_step)
			{
				deselerationV_exe();
				vertical_now_step++;
				STEPPER_CONTROLS_ST2_DIR_CW();
				STEPPER_CONTROLS_ST2_ENABLE();
				STEPPER_CONTROLS_ST2_STEP_TOGGLE();
			}
			else{
				
			STEPPER_CONTROLS_ST2_DISABLE(); //nedisablinam vertikaliam nes neislaikys fotiko
			}
		
		}
		
	}
	//*/
	
	
	
	

}

bool toggle_=true;

void pack_to_bag(void)
{
	if (toggle_)
	{
		home_vertical = 7800*2;
	}
	else
  {
		vertical_to_step = (start_steps+780*4)*2;
	}
	
toggle_ = !toggle_;	
		
}


void measure_0(void)
{
	if (toggle_)
	{
		home_vertical = 7800*2;
	}
	else
  {
		vertical_to_step = (start_steps+780*2)*2;
	}
	toggle_ = !toggle_;	
	
		
		
}






