#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "uart2_dbg.h"
#include "main.h"


#define UART_RX_SIZE 64
//static uint8_t uart_rx_buff[UART_RX_SIZE], uart_rx_buff_rp=0, uart_rx_buff_wp=0;
static uint8_t uart_rx_buff2[UART_RX_SIZE], uart_rx_buff_rp2=0, uart_rx_buff_wp2=0;

UART_HandleTypeDef huart2;

//komandu priemimo struck
uart_t uart_get=
{
   .gauta_wp = 0,
	 .gauta_rp = 0,
	 .cmd_wp   = 0,
	 .cmd_rp   = 0,
	 .cnt=0,

};

static uint8_t uart_tx_buff[256], uart_tx_buff_rp=0,uart_tx_buff_wp=0;


char string[32];
void vprint(const  char *fmt, va_list argp)
{
    memset(string,0,32);
    if(0 < vsprintf(string,fmt,argp)) // 
    {
        //HAL_UART_Transmit(&huart2, (uint8_t*)string, strlen(string), 0xffffff); // send message via UART
			  for (uint16_t i=0;i<strlen(string);i++)
				{
				   //xuart_tx_buff[xuart_tx_buff_wp++]=(uint8_t)string[i];
					uart_tx_buff[uart_tx_buff_wp++]= string[i];
					
				}
				__HAL_UART_ENABLE_IT(&huart2, UART_IT_TXE);
    }
}

void uart2_dbg_put_char(uint8_t charas)
{
   uart_tx_buff[uart_tx_buff_wp++]= charas;
   __HAL_UART_ENABLE_IT(&huart2, UART_IT_TXE);
}



void PRINTF(const char *fmt, ...) // 
{
    va_list argp;
    va_start(argp, fmt);
    vprint( fmt, argp);
    va_end(argp);
}





//MX_USART1_UART_Init();
void MX_USART2_UART_Init(void);
/* USART1 init function */
/* USART2 init function */
void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
	
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);

}

void uart2_dbg_irq_handler(void)
{
     /* USER CODE BEGIN USART1_IRQn 0 */
	//uartas_irq_handler();
    if((__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) != RESET) && 
       (__HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_RXNE) != RESET))
    {
        uart_rx_buff2[uart_rx_buff_wp2++] = (uint8_t)(huart2.Instance->DR & 0x00FF);
			  //PRINTF(".");
        if (uart_rx_buff_wp2>=UART_RX_SIZE) {uart_rx_buff_wp2=0;}
        /* Clear RXNE interrupt flag */
        __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_RXNE);
    }
		
		
    if((__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TXE) != RESET) &&
       (__HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_TXE) != RESET))
    {
        if (uart_tx_buff_rp==uart_tx_buff_wp) {
            __HAL_UART_DISABLE_IT(&huart2, UART_IT_TXE);
            __HAL_UART_ENABLE_IT(&huart2, UART_IT_TC);
        } else {
            huart2.Instance->DR = (uint8_t)(uart_tx_buff[uart_tx_buff_rp++] & (uint8_t)0xFF);
           // tx_o++;
        }
    }
    if((__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) != RESET) &&
       (__HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_TC) != RESET))
    {
      //  tx_busy = 0;
        __HAL_UART_DISABLE_IT(&huart2, UART_IT_TC);
    }


}


void uart2_dbg_handler(void)
{
    uint8_t chr;
    //uint8_t gi=0;
    if (uart_rx_buff_rp2!=uart_rx_buff_wp2)
		{
			
			 chr =uart_rx_buff2[uart_rx_buff_rp2++];
			 if (uart_rx_buff_rp2>=UART_RX_SIZE)
			 {
			   uart_rx_buff_rp2=0;
			 }
			 if (uart_get.cnt<UART_MAX_STRING)
			 {
			     uart_get.gauta[uart_get.gauta_wp].string[uart_get.cnt++]=chr;
			 }
			 
			 if (chr == 10) // \n
			 {
				  //PRINTF("%s",uart_get.gauta[uart_get.gauta_wp].string);
			    uart_get.gauta_wp++;
				  if (uart_get.gauta_wp>=UART_MAX_GAUTA)
					{
					   uart_get.gauta_wp=0;
					}
					
					memset(&uart_get.gauta[uart_get.gauta_wp].string,0,sizeof(uart_get.gauta[uart_get.gauta_wp].string));
					uart_get.gauta[uart_get.gauta_wp].param_cnt=0;
					uart_get.cnt=0;
			 }
			
		}
		
		
		//iskoduojam komanda
		if (uart_get.gauta_wp!=uart_get.gauta_rp)
		{
			 uint8_t i=0, sk=0;
			 char temp[10];
			 uart_get.gauta[uart_get.gauta_rp].cmd=0;
		   while((uart_get.gauta[uart_get.gauta_rp].string[i]!=13) && (i!=63))
			 {
				 
				 //pradzia sudedam komada
				 if (i<2)
				 {
					  uart_get.gauta[uart_get.gauta_rp].cmd = uart_get.gauta[uart_get.gauta_rp].cmd<<8;
				    uart_get.gauta[uart_get.gauta_rp].cmd = uart_get.gauta[uart_get.gauta_rp].cmd | uart_get.gauta[uart_get.gauta_rp].string[i];
					  
				 }
				 else
				 {
					 //parinam parametrus
				   if (uart_get.gauta[uart_get.gauta_rp].string[i]==32) //space
					 {
						  if (sk>0)
							{
								 if (uart_get.gauta[uart_get.gauta_rp].param_cnt<UART_MAX_PARAM)
								 {
								    uart_get.gauta[uart_get.gauta_rp].skaic[uart_get.gauta[uart_get.gauta_rp].param_cnt] = atoi(temp);
										uart_get.gauta[uart_get.gauta_rp].param_cnt++;
									  sk=0;
									  memset(&temp,0,sizeof(temp));
								 }
								 
								 
							}
					 
					 }
					 else
					 {
						 if (sk<9 && uart_get.gauta[uart_get.gauta_rp].string[i]!=32)
						 {
						    temp[sk]=uart_get.gauta[uart_get.gauta_rp].string[i];
							  sk++;
						 }
						 
					 
					 }
				 
				 }
				 i++;
			
			 
			 
			 }
			 
			 
			 //PRINTF("cmd: %s",uart_get.gauta[uart_get.gauta_rp].string);
			 //PRINTF("cmd: %u",uart_get.gauta[uart_get.gauta_rp].cmd);
			 //PRINTF("param: %u",uart_get.gauta[uart_get.gauta_rp].skaic[0]);
			 uart_get.gauta_rp++;
			 if (uart_get.gauta_rp>=UART_MAX_GAUTA)
			 {
			    uart_get.gauta_rp=0;
			 }
			 
			 //pazymim kad gavom komanda
			 uart_get.cmd_wp++;
			 if (uart_get.cmd_wp>=UART_MAX_GAUTA)
			 {
			    uart_get.cmd_wp=0;
			 }
			 
		   
		}



}



void uart2_dbg_init(void)
{
		MX_USART2_UART_Init();

}









// Comandu handleris
void uart2_dbg_cmd_handler(void)
{

    if (uart_get.cmd_wp!=uart_get.cmd_rp)
		{
		
		   switch (uart_get.gauta[uart_get.cmd_rp].cmd)
			 {
			 
			    //------------------------***CI***--------------------------------------------//
					case (('I'<<8)|'D'): // 
					  PRINTF("ID \r\n");
					break;
										
					default: //visos kitos
						PRINTF("no cmd %c%c\r\n",uart_get.gauta[uart_get.cmd_rp].cmd>>8,uart_get.gauta[uart_get.cmd_rp].cmd&0x00FF);
					break;
				}
						
						//PRINTF("GO %u\r\n",uart_get.gauta[uart_get.cmd_rp].skaic[0]);
					
				
		   uart_get.cmd_rp++;
		   if (uart_get.cmd_rp>=UART_MAX_GAUTA)
			 {
			    uart_get.cmd_rp=0;
			 }
		
		
		
		}


}


