#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "main.h"

enum task_unique_id {
		BT_CONTROL_ID_ZERO,
		BT_CONTROL_ID_NONE,
    BT_CONTROL_ID_POWER_ON_OFF,
		BT_CONTROL_ID_POWER_ON_OFF2,
		BT_CONTROL_ID_SKIP_FORWARD,
		BT_CONTROL_ID_SKIP_BACKWARD,
		BT_CONTROL_ID_MIDLE, //PLAY/PAUSE/STOP/ANSWER/DECLINE funkcijos....
	  BT_CONTROL_ID_VOLUME_UP, //up
		BT_CONTROL_ID_VOLUME_DOWN, //down
};


typedef struct 
{
	  bool active_task;
    GPIO_TypeDef* pin_PORT;
		uint32_t pin_NR;
		uint32_t on_off_toggle;
    uint32_t delay_on_ms[4]; //iki 4 delay ir pns
		uint32_t delay_off_ms[4];
		uint32_t task_id;

}BT_control_task_t;



void BT_control_gpio_init();
void BT_control_gpio_handler();
void BT_control_gpio_systick();


void BT_control_SET_BT_POWER_ON();
void BT_control_SET_BT_POWER_OFF();
void BT_control_SET_BT_POWER_OFF2();
void BT_control_SET_BT_SKIP_FORWARD();
void BT_control_SET_BT_SKIP_BACKWARD();
void BT_control_SET_PLAY_PAUSE();
void BT_control_SET_BT_STOP(); 
void BT_control_SET_BT_SHUFLE_SONGS();
void BT_control_restart_BT();

void BT_control_SET_BT_VOLUME_UP();
void BT_control_SET_BT_VOLUME_DOWN();

//Call funkcijos
void BT_control_SET_CALL_ANSWER__END__HELD();
void BT_control_SET_CALL_REJECT_HELDREJECT();
void BT_control_SET_RECALL_LAST();



bool BT_control_GET_CALL_ACTIVE_INCOMING_OUTGOING(); 
bool BT_control_GET_AUDIO_ACTIVE();
bool BT_control_GET_BT_ON();
bool BT_control_GET_BT_CONNECTED();
