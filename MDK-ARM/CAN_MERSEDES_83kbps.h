#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"


typedef struct
{
  uint32_t StdId; //ID
	uint8_t data[8]; //data
  
} CAN_pack_get;


void CAN_mersas_init(void);



void CAN_mersas_receiveFIFO0_irq_handler(void);
void CAN_mersas_handler(void);
void CAN_mersedes_handler();

void systick1s();

void test_short_fnc_can();
void test_long_fnc_can();
void test_custom(uint8_t buttons, uint32_t time);

