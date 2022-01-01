#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

#define UART_MAX_GAUTA 5
#define UART_MAX_PARAM 10
#define UART_MAX_STRING 64

typedef struct
{
	uint8_t string[UART_MAX_STRING];
	uint16_t cmd;
	uint16_t skaic[UART_MAX_PARAM];
	uint8_t param_cnt;
}cmd_t;

typedef struct
{
	uint8_t gauta_wp; //gauto string wp
	uint8_t gauta_rp;
	uint8_t cmd_wp;   //gautos cmd wp
	uint8_t cmd_rp;
	uint8_t cnt;
  cmd_t   gauta[UART_MAX_GAUTA];
}uart_t;





//void vprint(const char *fmt, va_list argp);
void PRINTF(const char *fmt, ...);
void uart2_dbg_init(void);
void uart2_dbg_irq_handler(void);

void uart2_dbg_put_char(uint8_t charas);
void uart2_dbg_handler(void);
void uart2_dbg_cmd_handler(void);
