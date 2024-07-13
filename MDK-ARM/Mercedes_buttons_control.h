#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "main.h"


#define SHORT_PRESS_TIME_MIN 15
#define SHORT_PRESS_TIME_MAX 1200
#define LONG_PRESS_TIME_EVENT 1300
#define LONG_LONG_PRESS_TIME_EVENT 5000
#define VERY_LONG_PRESS_TIME_EVENT 25000


 /*!< Interrupt Number Definition */
typedef enum
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
	EVENT_NONE,
  EVENT_SHORT_BUT,
	EVENT_LONG_BUT,
	EVENT_LONG_LONG_EVENT_BUT,
	EVENT_VERY_LONG_EVENT_BUT,
} event_type;

 /*!< Interrupt Number Definition */
typedef enum
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
	FNC_NONE,
	
  FNC_BT_SKIP_FORWARD,
	FNC_BT_SKIP_BACKWARD,
	FNC_BT_SET_PLAY_PAUSE,
	
	FNC_BT_POWER_OFF_ON,
	FNC_RESTART_BT,
	
	FNC_ANSWER_CALL, //answer,end call, held accept
	FNC_REJECT_CALL,
	FNC_RECALL_CALL,
	
	FNC_VOLUME_UP,
	FNC_VOLUME_DOWN,
	
	FNC_TOGGLE_SOURCE,
	
} function_type;

typedef enum
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
	BTN_NONE = 0,
	
  BTN_1_NEXT_DISP = 1<<0,
	BTN_2_PREW_DISP = 1<<1,
	BTN_3 = 1<<2,
	BTN_4 = 1<<3,
	BTN_5_PLIUS_KEY = 1<<4,
	BTN_6_MINUS_KEY = 1<<5,
	BTN_7_PHONE_SEND = 1<<6,
	BTN_8_PHONE_END = 1<<7,
	
	/*
	MSG NAME: BUTTON_4_2 - phone End, OFFSET 8, LENGTH 1
						MSG NAME: BUTTON_4_1 - phone Send, OFFSET 9, LENGTH 1
						MSG NAME: BUTTON_3_2 - "-" key, OFFSET 10, LENGTH 1
						MSG NAME: BUTTON_3_1 - "+" Key, OFFSET 11, LENGTH 1
						MSG NAME: BUTTON_2_2 - reserve, OFFSET 12, LENGTH 1
						MSG NAME: BUTTON_2_1 - reserve, OFFSET 13, LENGTH 1
						MSG NAME: BUTTON_1_2 - Previous display, OFFSET 14, LENGTH 1
						MSG NAME: BUTTON_1_1 - Next display, OFFSET 15, LENGTH 1
						*/
	
} BTN_type;


typedef struct
{
  bool   active_state;
	bool   event_hit;
  uint32_t button_bits_nr; //1st
  uint32_t button_press_count; //kiek yra laiko paspaustas
  uint32_t button_press_count2;
  uint32_t button_event_type; //koks eventas generuojamas, short,long, verylong, double/press
  uint32_t button_funcion_to_exe;
  
} buttons_links_t;

void Mercedes_buttons_control_init();
void Mercedes_buttons_control_handler();
void Mercedes_buttons_control_systick();
void Mercedes_buttons_control(uint32_t event_to_exe);
void Mercedes_buttons_get_CAN_buttons_data(uint8_t *data);


