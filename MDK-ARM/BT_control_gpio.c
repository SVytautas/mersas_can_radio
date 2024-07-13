#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "uart2_dbg.h"
#include "main.h"
#include "BT_control_gpio.h"
#include "CD4052.h"

void BT_control_clear_tasks_init();
void BT_control_set_on_delay(uint32_t task_nr, uint32_t delay_nr);
void BT_control_set_off_delay(uint32_t task_nr, uint32_t delay_nr);
void BT_control_add_task(BT_control_task_t task_);
void BT_control_short_add(BT_control_task_t* temp_task);
void BT_control_long_add(BT_control_task_t* temp_task);
void BT_control_long_long_add(BT_control_task_t* temp_task);

#define TASKS_SIZE 8
BT_control_task_t BT_control_tasks[TASKS_SIZE];

void BT_control_gpio_init()
{
	
	//inicializuojame GPIO output pinus
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12 | GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	
	//PB12 POWER_EN_STM
	//BT veikimas 
	//POwer on LONG(1s) kai viskas isjungta
	//POwer off VLONG(2.5s) beveik ant visu eventu
	//taip pat enter pairing gal i pereina i connection sujugima?? VLONG(2.5s)
	//Initiate voice dial SH single (trumpas paspaudimas)
	/*Configure GPIO pin : PB12 */ //POWER BT ON SET 
  GPIO_InitStruct.Pin = GPIO_PIN_12; //power 0, audio Radio/BT PB10
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
	//init Forward button on PIO18
	//AVRCP skip forward su  Funkcija SH SINGLE //trumpas pasaudimas gal apie 300-500ms
	/*Configure GPIO pins : PA1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1; //forward
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/*Configure GPIO pin : PB10 */ //SKIP BACKWARD
	/*Configure GPIO pin : PB9 */ //Volume UP  //Short repeat // kita funkcija prisijungia prie paskutinio ilgai palaikius LONG (1s)
	/*Configure GPIO pin : PB8 */ //Volume Down /Short repeat
	//AVRCP skip BACKWARD su  Funkcija SH SINGLE //trumpas pasaudimas gal apie 200-500ms
  GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_9 | GPIO_PIN_8; //BACKWARD
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
	/*Configure GPIO pin : PC13 */ //PLAY/PAUSE
	//AVRCP PLAY/PAUSE su  Funkcija SH SINGLE //trumpas pasaudimas gal apie 200-500ms
	//AVRCP STOP su  Funkcija LONG(1s)
  GPIO_InitStruct.Pin = GPIO_PIN_13; //BACKWARD
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	
	/*Configure GPIO pin : PA2 CALL/INCOMING/OUTGOING state */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	/*Configure GPIO pin : PB3 LED GREEN BT ACTIVE */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
	/*Configure GPIO pin : PC14 Audio active state */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	/*Configure GPIO pin : PC15 BT on state */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//initinam libo konfiguracija
	BT_control_clear_tasks_init();
	
	
	
}


void BT_control_clear_tasks_init()
{
	
	for (uint32_t i=0;i<TASKS_SIZE;i++)
	{
		BT_control_tasks[i].active_task = false;
		BT_control_tasks[i].task_id = BT_CONTROL_ID_ZERO;
		BT_control_tasks[i].pin_NR =0;
		BT_control_tasks[i].pin_PORT = GPIOB;
		BT_control_tasks[i].on_off_toggle = 1;
		
		BT_control_tasks[i].delay_on_ms[0] = 0;
		BT_control_tasks[i].delay_on_ms[1] = 0;
		BT_control_tasks[i].delay_on_ms[2] = 0;
		BT_control_tasks[i].delay_on_ms[3] = 0;
		
		BT_control_tasks[i].delay_off_ms[0] = 0;
		BT_control_tasks[i].delay_off_ms[1] = 0;
		BT_control_tasks[i].delay_off_ms[2] = 0;
		BT_control_tasks[i].delay_off_ms[3] = 0;
	
	}

}


void BT_control_add_task(BT_control_task_t task_)
{
	//pridedamas taskas taskai nekartojami kad nesidubluoti paspaudimai ir pns turi pabaikti viena kad butu pridetas kitas
  for (uint32_t i=0;i<TASKS_SIZE;i++)
	{
		if (BT_control_tasks[i].task_id == task_.task_id)
		{
			return; //task jau yra pridedas
		}
	}
	
	for (uint32_t i=0;i<TASKS_SIZE;i++)
	{
		if (BT_control_tasks[i].active_task == false)
		{
			
			memcpy(&BT_control_tasks[i], &task_,sizeof(BT_control_task_t));
			//pridedam taska ir return
			BT_control_tasks[i].active_task = true;
			
			return; 
		}
	}

}




void BT_control_gpio_handler()
{
	//while greitis


}

uint32_t BT_on_start_MCU_ = TURN_ON_DELAY_TIME;
void BT_control_gpio_systick()
{
	//1Khz systickas 
	for (uint32_t i=0;i<TASKS_SIZE;i++)
	{
	
		if (BT_control_tasks[i].active_task)
		{
			//jeigu task aktyvus
			if (BT_control_tasks[i].on_off_toggle>0)
			{
				//jeigu on tasks  vygdome
				//preiname visus delay ir isjungiame ant 0 pina ir togliname on off tada dirba off rezimas iki 0
				if (BT_control_tasks[i].delay_on_ms[0]>0)
				{
					BT_control_set_on_delay(i, 0);
				}
				else if (BT_control_tasks[i].delay_on_ms[1]>0)
				{
					BT_control_set_on_delay(i, 1);
				}
				else if (BT_control_tasks[i].delay_on_ms[2]>0)
				{
					BT_control_set_on_delay(i, 2);
				}
				else if (BT_control_tasks[i].delay_on_ms[3]>0)
				{
					BT_control_set_on_delay(i, 3);
				}
				else
				{ 
					//PRINTF("taskas ivygdytas %u",BT_control_tasks[i].task_id);
					BT_control_tasks[i].active_task = false;
					BT_control_tasks[i].task_id = BT_CONTROL_ID_ZERO;
					BT_control_tasks[i].on_off_toggle = 0; 
					//uzbaigiam taska
					HAL_GPIO_WritePin(BT_control_tasks[i].pin_PORT,
					BT_control_tasks[i].pin_NR,
					GPIO_PIN_RESET);
					
					
				
				}
			}
			else
			{
				
				if (BT_control_tasks[i].delay_off_ms[0]>0)
				{
					BT_control_set_off_delay(i, 0);
				}
				else if (BT_control_tasks[i].delay_off_ms[1]>0)
				{
					BT_control_set_off_delay(i, 1);
				}
				else if (BT_control_tasks[i].delay_off_ms[2]>0)
				{
					BT_control_set_off_delay(i, 2);
				}
				else if (BT_control_tasks[i].delay_off_ms[3]>0)
				{
					BT_control_set_off_delay(i, 3);
				}
				else
				{ 
					BT_control_tasks[i].active_task = false;
					BT_control_tasks[i].task_id = BT_CONTROL_ID_ZERO;
					BT_control_tasks[i].on_off_toggle = 1; 
					//uzbaigiam taska
					HAL_GPIO_WritePin(BT_control_tasks[i].pin_PORT,
					BT_control_tasks[i].pin_NR,
					GPIO_PIN_RESET);
				
				}
			
			
			}
			
			break; //po viena task atliekam kuris pirmas ta ir atliekam
		
		}
		
	}
	
	
	if (BT_on_start_MCU_>0)
	{
		BT_on_start_MCU_--;
		if (BT_on_start_MCU_==0)
		{
			PRINTF("BT on MCU start \r\n");
			BT_control_SET_BT_POWER_ON();
			//BT_control_SET_BT_POWER_OFF(); //trupti ilgesnis kad tikrai isiujngtu
		}
			
	}
	
	

}

void BT_control_set_on_delay(uint32_t task_nr, uint32_t delay_nr)
{
					HAL_GPIO_WritePin(BT_control_tasks[task_nr].pin_PORT,
														BT_control_tasks[task_nr].pin_NR,
														GPIO_PIN_SET); //jiegu taigiamas ijungiame
						
						BT_control_tasks[task_nr].delay_on_ms[delay_nr]--;
							if (BT_control_tasks[task_nr].delay_on_ms[delay_nr]==0) 
							{ 
								BT_control_tasks[task_nr].on_off_toggle = 0; 
								
								HAL_GPIO_WritePin(BT_control_tasks[task_nr].pin_PORT,
								BT_control_tasks[task_nr].pin_NR,
								GPIO_PIN_RESET); 
							}

}

void BT_control_set_off_delay(uint32_t task_nr, uint32_t delay_nr)
{
					HAL_GPIO_WritePin(BT_control_tasks[task_nr].pin_PORT,
														BT_control_tasks[task_nr].pin_NR,
														GPIO_PIN_RESET); //jeigu taigiamas isjungiame
						
						BT_control_tasks[task_nr].delay_off_ms[delay_nr]--;
							if (BT_control_tasks[task_nr].delay_off_ms[delay_nr]==0) 
							{ 
								BT_control_tasks[task_nr].on_off_toggle = 1; 
							}

}






void BT_control_SET_BT_POWER_ON()
{
		//ijungimui reikia
	//PB12 POWER_EN_STM
	//BT veikimas 
	//POwer on LONG(1s) kai viskas isjungta //pries paleidziant reikia 0
	//POwer off VLONG(2.5s) beveik ant visu eventu
	BT_control_task_t temp_task;

	temp_task.active_task = true;
	temp_task.pin_PORT = GPIOB;
	temp_task.pin_NR = GPIO_PIN_12;
	temp_task.task_id = BT_CONTROL_ID_POWER_ON_OFF;
	temp_task.on_off_toggle = 1;
		
	temp_task.delay_on_ms[0] = 1; //
	temp_task.delay_on_ms[1] = 1700; //SH single
	temp_task.delay_on_ms[2] = 0;
	temp_task.delay_on_ms[3] = 0;
	
	temp_task.delay_off_ms[0] = 300; //siek tiek
	temp_task.delay_off_ms[1] = 200;
	temp_task.delay_off_ms[2] = 1;
	temp_task.delay_off_ms[3] = 0;
	
	BT_control_add_task(temp_task);
	
	CD4052_SET_BT_OUT(); //ijungiam BT
	
}

void BT_control_SET_BT_POWER_OFF()
{
		//ijungimui reikia
	//PB12 POWER_EN_STM
	//BT veikimas 
	//POwer on LONG(1s) kai viskas isjungta
	//POwer off VLONG(2.5s) beveik ant visu eventu
	BT_control_task_t temp_task;

	temp_task.active_task = true;
	temp_task.pin_PORT = GPIOB;
	temp_task.pin_NR = GPIO_PIN_12;
	temp_task.task_id = BT_CONTROL_ID_POWER_ON_OFF;
	temp_task.on_off_toggle = 1;
		
	BT_control_long_long_add(&temp_task);
	
	BT_control_add_task(temp_task);
	
	CD4052_SET_BT_OUT(); //ijungiam BT jei bus isjungtas perjungs per 100ms
	
}

void BT_control_SET_BT_POWER_OFF2()
{
		//ijungimui reikia
	//PB12 POWER_EN_STM
	//BT veikimas 
	//POwer on LONG(1s) kai viskas isjungta
	//POwer off VLONG(2.5s) beveik ant visu eventu
	BT_control_task_t temp_task;

	temp_task.active_task = true;
	temp_task.pin_PORT = GPIOB;
	temp_task.pin_NR = GPIO_PIN_12;
	temp_task.task_id = BT_CONTROL_ID_POWER_ON_OFF2;
	temp_task.on_off_toggle = 1;
		
	temp_task.delay_on_ms[0] = 1; 
	temp_task.delay_on_ms[1] = 2700; //LONG LONG yra 2.5s dedame 2.7
	temp_task.delay_on_ms[2] = 0;
	temp_task.delay_on_ms[3] = 0;
	
	temp_task.delay_off_ms[0] = 100; //
	temp_task.delay_off_ms[1] = 2000; //palaukiam kol isijungs
	temp_task.delay_off_ms[2] = 0;
	temp_task.delay_off_ms[3] = 0;
	
	BT_control_add_task(temp_task);
	
	CD4052_SET_BT_OUT(); //ijungiam BT jei bus isjungtas perjungs per 100ms
	
}


void BT_control_SET_BT_SKIP_FORWARD()
{
		
	//init Forward button on PIO18
	//AVRCP skip forward su  Funkcija SH SINGLE //trumpas pasaudimas gal apie 300-500ms
	/*Configure GPIO pins : PA1 */
	BT_control_task_t temp_task;

	temp_task.active_task = true;
	temp_task.pin_PORT = GPIOA;
	temp_task.pin_NR = GPIO_PIN_1;
	temp_task.task_id = BT_CONTROL_ID_SKIP_FORWARD;
	temp_task.on_off_toggle = 1;
		
	BT_control_short_add(&temp_task);
	BT_control_add_task(temp_task);
	
}

void BT_control_SET_BT_SKIP_BACKWARD()
{
		/*Configure GPIO pin : PB10 */ //SKIP BACKWARD
	//AVRCP skip BACKWARD su  Funkcija SH SINGLE //trumpas pasaudimas gal apie 200-500ms
	BT_control_task_t temp_task;

	temp_task.active_task = true;
	temp_task.pin_PORT = GPIOB;
	temp_task.pin_NR = GPIO_PIN_10;
	temp_task.task_id = BT_CONTROL_ID_SKIP_BACKWARD;
	temp_task.on_off_toggle = 1;
		
	BT_control_short_add(&temp_task);	
	BT_control_add_task(temp_task);
	
}


void BT_control_SET_PLAY_PAUSE()
{
		/*Configure GPIO pin : PC13 */ //PLAY/PAUSE
	//AVRCP PLAY/PAUSE su  Funkcija SH SINGLE //trumpas pasaudimas gal apie 200-500ms
	//AVRCP STOP su  Funkcija LONG(1s)
	BT_control_task_t temp_task;

	temp_task.active_task = true;
	temp_task.pin_PORT = GPIOC;
	temp_task.pin_NR = GPIO_PIN_13;
	temp_task.task_id = BT_CONTROL_ID_MIDLE;
	temp_task.on_off_toggle = 1;
	
	BT_control_short_add(&temp_task);
	BT_control_add_task(temp_task);
	
}




void BT_control_SET_CALL_ANSWER__END__HELD()
{
	/*Configure GPIO pin : PC13 */ //ANSWER , CANCEL END , ir held accept incoming call.
	BT_control_task_t temp_task;

	temp_task.active_task = true;
	temp_task.pin_PORT = GPIOC;
	temp_task.pin_NR = GPIO_PIN_13;
	temp_task.task_id = BT_CONTROL_ID_MIDLE;
	temp_task.on_off_toggle = 1;
		
	BT_control_short_add(&temp_task);
	
	BT_control_add_task(temp_task);
	
}

void BT_control_SET_RECALL_LAST()
{
	/*Configure GPIO pin : PC13 */ //ANSWER , CANCEL END , ir held accept incoming call.
	BT_control_task_t temp_task;

	temp_task.active_task = true;
	temp_task.pin_PORT = GPIOC;
	temp_task.pin_NR = GPIO_PIN_13;
	temp_task.task_id = BT_CONTROL_ID_MIDLE;
	temp_task.on_off_toggle = 1;
	
	if (BT_control_GET_AUDIO_ACTIVE())
	{
		//pirma sustabdome 
		temp_task.delay_on_ms[0] = 350; //
		temp_task.delay_on_ms[1] = 350; //SH single
		temp_task.delay_on_ms[2] = 350;
		temp_task.delay_on_ms[3] = 0;
		
		temp_task.delay_off_ms[0] = 1500; //siek tiek
		temp_task.delay_off_ms[1] = 150;
		temp_task.delay_off_ms[2] = 10;
		temp_task.delay_off_ms[3] = 0;
	}
	else
	{
		temp_task.delay_on_ms[0] = 10; //
		temp_task.delay_on_ms[1] = 350; //SH single
		temp_task.delay_on_ms[2] = 350;
		temp_task.delay_on_ms[3] = 0;
		
		temp_task.delay_off_ms[0] = 100; //siek tiek
		temp_task.delay_off_ms[1] = 150;
		temp_task.delay_off_ms[2] = 10;
		temp_task.delay_off_ms[3] = 0;
	
	}
	
	
	BT_control_add_task(temp_task);
	PRINTF("add RECALL\r\n");
	
}



void BT_control_SET_CALL_REJECT_HELDREJECT()
{
		/*Configure GPIO pin : PC13 */ //REJECT CALL, HELD REJECT
	//AVRCP PLAY/PAUSE su  Funkcija SH SINGLE //trumpas pasaudimas gal apie 200-500ms
	//AVRCP STOP su  Funkcija LONG(1s)
	BT_control_task_t temp_task;

	temp_task.active_task = true;
	temp_task.pin_PORT = GPIOC;
	temp_task.pin_NR = GPIO_PIN_13;
	temp_task.task_id = BT_CONTROL_ID_MIDLE;
	temp_task.on_off_toggle = 1;
		
	BT_control_long_add(&temp_task);
	
	BT_control_add_task(temp_task);
	
}



void BT_control_SET_BT_STOP()
{
		/*Configure GPIO pin : PC13 */ //STOP
	//AVRCP PLAY/PAUSE su  Funkcija SH SINGLE //trumpas pasaudimas gal apie 200-500ms
	//AVRCP STOP su  Funkcija LONG(1s)
	BT_control_task_t temp_task;

	temp_task.active_task = true;
	temp_task.pin_PORT = GPIOC;
	temp_task.pin_NR = GPIO_PIN_13;
	temp_task.task_id = BT_CONTROL_ID_MIDLE;
	temp_task.on_off_toggle = 1;
		
	BT_control_long_add(&temp_task);
	
	BT_control_add_task(temp_task);
	
}


void BT_control_SET_BT_SHUFLE_SONGS()
{
		/*Configure GPIO pin : PB10 */ //SKIP BACKWARD
	/*Configure GPIO pin : PB9 */ //Volume UP  //Short repeat // kita funkcija prisijungia prie paskutinio ilgai palaikius LONG (1s)
	/*Configure GPIO pin : PB8 */ //Volume Down /Short repeat
	BT_control_task_t temp_task;

	temp_task.active_task = true;
	temp_task.pin_PORT = GPIOB;
	temp_task.pin_NR = GPIO_PIN_9;
	temp_task.task_id = BT_CONTROL_ID_VOLUME_UP;
	temp_task.on_off_toggle = 1;
		
	BT_control_long_add(&temp_task);
	
	BT_control_add_task(temp_task);
	
}


void BT_control_SET_BT_VOLUME_UP()
{
	/*Configure GPIO pin : PB9 */ //Volume UP  //Short repeat // kita funkcija prisijungia prie paskutinio ilgai palaikius LONG (1s)
	/*Configure GPIO pin : PB8 */ //Volume Down /Short repeat
	BT_control_task_t temp_task;

	temp_task.active_task = true;
	temp_task.pin_PORT = GPIOB;
	temp_task.pin_NR = GPIO_PIN_9;
	temp_task.task_id = BT_CONTROL_ID_VOLUME_UP;
	temp_task.on_off_toggle = 1;
		
	BT_control_short_add(&temp_task);
	
	BT_control_add_task(temp_task);
	
}

void BT_control_SET_BT_VOLUME_DOWN()
{
	/*Configure GPIO pin : PB9 */ //Volume UP  //Short repeat // kita funkcija prisijungia prie paskutinio ilgai palaikius LONG (1s)
	/*Configure GPIO pin : PB8 */ //Volume Down /Short repeat
	BT_control_task_t temp_task;

	temp_task.active_task = true;
	temp_task.pin_PORT = GPIOB;
	temp_task.pin_NR = GPIO_PIN_8;
	temp_task.task_id = BT_CONTROL_ID_VOLUME_DOWN;
	temp_task.on_off_toggle = 1;
		
	BT_control_short_add(&temp_task);
	
	BT_control_add_task(temp_task);
	
}


void BT_control_short_add(BT_control_task_t* temp_task)
{
	temp_task->delay_on_ms[0] = 1; //
	temp_task->delay_on_ms[1] = 125; //SH single >?? 61neveikia  62veikia 75veikia //125 dvigubas
	//temp_task->delay_on_ms[1] = 350; //SH single >?? 61neveikia  62veikia 75veikia //125 dvigubas
	temp_task->delay_on_ms[2] = 0;
	temp_task->delay_on_ms[3] = 0;
	
	temp_task->delay_off_ms[0] = 20; //siek tiek
	temp_task->delay_off_ms[1] = 1;
	temp_task->delay_off_ms[2] = 0;
	temp_task->delay_off_ms[3] = 0;
	
}

void BT_control_long_add(BT_control_task_t* temp_task)
{
	temp_task->delay_on_ms[0] = 1; //
	temp_task->delay_on_ms[1] = 1300; //long 1s // dedam 1.1s
	temp_task->delay_on_ms[2] = 0;
	temp_task->delay_on_ms[3] = 0;
	
	temp_task->delay_off_ms[0] = 100; //siek tiek
	temp_task->delay_off_ms[1] = 1;
	temp_task->delay_off_ms[2] = 0;
	temp_task->delay_off_ms[3] = 0;
	
}

void BT_control_long_long_add(BT_control_task_t* temp_task)
{
	temp_task->delay_on_ms[0] = 1; 
	temp_task->delay_on_ms[1] = 2700; //LONG LONG yra 2.5s dedame 2.7
	temp_task->delay_on_ms[2] = 0;
	temp_task->delay_on_ms[3] = 0;
	
	temp_task->delay_off_ms[0] = 100; //siek tiek
	temp_task->delay_off_ms[1] = 200;
	temp_task->delay_off_ms[2] = 0;
	temp_task->delay_off_ms[3] = 0;
	
}






bool BT_control_GET_CALL_ACTIVE_INCOMING_OUTGOING()
{
		return HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2);
}

bool BT_control_GET_AUDIO_ACTIVE()
{
		return HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_15);
}

bool BT_control_GET_BT_ON()
{
		return HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14);	
}

bool BT_control_GET_BT_CONNECTED()
{
		return !HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3);	
}



void BT_control_restart_BT()
{
	if (BT_control_GET_BT_ON())
	{
		BT_control_SET_BT_POWER_OFF2(); //isjungiam 
		BT_control_SET_BT_POWER_OFF(); //ijungiam
	}
	else
	{
		BT_control_SET_BT_POWER_ON(); //ijungiam tik
	}
	



}








