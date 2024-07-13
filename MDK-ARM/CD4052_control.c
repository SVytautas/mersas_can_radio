#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "uart2_dbg.h"
#include "main.h"
#include "BT_control_gpio.h"
#include "CD4052.h"


//void CD4052_if_BT_off_turnON_delay20s();

void CD4052_init()
{
	//inicializuojame GPIO output pinus
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
	//initinam pinus SW A pinas ant PB4. SW_B ant PB5
   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);
	
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
	
	/*Configure GPIO pin : PB13 , PB15 AM button sekimas */
  GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_15;
	//GPIO_InitStruct.Pin =  GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
	//GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	 /*Configure GPIO pin : PB4 PB5  */ 
  GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5; //power 0, audio Radio/BT PB10
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	
	 /*Configure GPIO pin :  PB13 AM button
  GPIO_InitStruct.Pin =  GPIO_PIN_15; //power 0, audio Radio/BT PB10
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	*/ 
	
	
	
	CD4052_SET_BT_OUT();

}



uint8_t source_now=1;
bool lastStateBT_connect = true;

void CD4052_SET_BT_OUT()
{
	 //A -> 1; B -> 1;
	//pinus SW A pinas ant PB4. SW_B ant PB5
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	
	source_now=CD4052_SOURCE_BT;	
	lastStateBT_connect = true;
	
	PRINTF("BT source on\r\n");
}


void CD4052_SET_AUX_OUT()
{
    //A -> 0; B -> 1;
	//pinus SW A pinas ant PB4. SW_B ant PB5
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	
}


void CD4052_SET_RADIO()
{
   //A -> 0; B -> 0;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);
	
	//if (source_now != CD4052_SOURCE_RADIO) {}
	source_now=CD4052_SOURCE_RADIO;
	PRINTF("Radio source on\r\n");	

}


void CD4052_SET_TOGGLE_SOURCE()
{
		if (source_now == CD4052_SOURCE_RADIO)
		{
			source_now=CD4052_SOURCE_BT;
		}
		else if (source_now == CD4052_SOURCE_BT)
		{
			source_now=CD4052_SOURCE_RADIO;
		}
		
		
		
		CD4052_REFRESH_SOURCE();
		
		if (source_now == CD4052_SOURCE_BT)
		{
			//patikriname BT jei isjungtas ijungiame
			CD4052_if_BT_off_turnON_delay20s();
		}

}

void CD4052_REFRESH_SOURCE()
{
		switch (source_now)
		{
			
			case CD4052_SOURCE_RADIO:
				CD4052_SET_RADIO();
			break;
			
			case CD4052_SOURCE_BT:
				CD4052_SET_BT_OUT();
			break;
			
			default:
				CD4052_SET_RADIO();
				break;
		}

}



volatile uint32_t tick1s = TURN_ON_DELAY_TIME + 3500; //isijungimui // kol dar isijungs laikas
uint32_t switch_to_radio_timeout = 0; //10hz

bool over_call = false;
uint32_t last_source = CD4052_SOURCE_BT;

void CD4052_tick_1khz()
{
	
	
	//*
	if (tick1s>0)
	{
		 tick1s--;
		 
	}
	else
	{
				tick1s = 100;
				if (BT_control_GET_BT_ON()==false) 
				{ 
					if (source_now!= CD4052_SOURCE_RADIO) {CD4052_SET_RADIO();} 
					lastStateBT_connect = false; 
					return;
				}
			
		
			///* perjungimas i radija jei BT neprisijungta ir su BT neprisijungta
			if (source_now== CD4052_SOURCE_BT && BT_control_GET_BT_CONNECTED()==false && switch_to_radio_timeout==0) //BT source ir neprisijungta
			{
					PRINTF("on radio start\r\n");
					switch_to_radio_timeout = TURN_ON_TO_RADIO_DELAY; //60s po 1min
			}
			
			if (switch_to_radio_timeout>0)
			{
					switch_to_radio_timeout--;
					if (switch_to_radio_timeout==0)
					{
							if(BT_control_GET_BT_CONNECTED()==false)
							{
									CD4052_SET_RADIO();
									PRINTF("Set Radio Auto\r\n");
							}
					
					}
			
			}
			//*/
			
			if (lastStateBT_connect == false && source_now== CD4052_SOURCE_RADIO && over_call == false) //RADIO source ir paskutinis buvo state neprisijungta
			{
					if(BT_control_GET_BT_CONNECTED()==true)
					{
						CD4052_SET_BT_OUT();
						
						PRINTF("set BT auto\r\n");
					}
			
			}
			
			if(BT_control_GET_BT_CONNECTED()==false)
			{
				//PRINTF("lasss\r\n");
					lastStateBT_connect = false;
			}
			
			
			if (BT_control_GET_CALL_ACTIVE_INCOMING_OUTGOING()==true)
			{
				if (over_call == false) {over_call = true; last_source = source_now;}
				if (source_now!= CD4052_SOURCE_BT)	{CD4052_SET_BT_OUT();} // //setinam BT
					//PRINTF("set BT call\r\n");
			}
			else
			{
					if (over_call == true) {over_call = false; source_now = last_source; CD4052_REFRESH_SOURCE();}
			}
		
		
		
	}


}


void CD4052_if_BT_off_turnON_delay20s()
{
		if (BT_control_GET_BT_ON()==false) { 
			
			BT_control_SET_BT_POWER_ON();
			tick1s = 20000;
		}

}

void CD4052_temporary_switch_to_BT_source()
{
		tick1s = 15000; //15s
		CD4052_SET_BT_OUT();
		

}

bool CD4052_source_now_not_BT()
{
		if (source_now== CD4052_SOURCE_BT)
		{
			return false;
		}
	
		return true;
}



