#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "uart2_dbg.h"
#include "main.h"
#include "BT_control_gpio.h"
#include "CD4052.h"
#include "Mercedes_buttons_control.h"
#include "bit_manipulation.h"


//#define VOLUME_BT_ON //valdymas garso BT ant vairo jei nera valdymo nuo mages
//#define CONTROL_NEXTPREW_ON_MENU //controle next prew su rodiklelemis
//#define CONTROL_ON_ON_MENU_4_3 //kontroles ijungimas ant 4 ir 3 mygtukos menu nepatogu play/pause
#define CONTROL_OFF_ON_NEXTDISP //isjungimas ant next Display 
#define COTROL_PLAY_PAUSE_ON_LONG_PRESS_VOLUME_DOWN //isjungimas ant next Display mygtuko


const uint8_t SIZE_BUTTONS_LINKS = 16;
buttons_links_t buttons_[SIZE_BUTTONS_LINKS] = {
	
	//next prew songs
	///*
	{ .active_state = false,
		.button_bits_nr= BTN_7_PHONE_SEND, 
	  .button_press_count=0,
		.button_press_count2 =0, 
		.button_event_type = EVENT_SHORT_BUT, 
		.button_funcion_to_exe = FNC_BT_SKIP_FORWARD},
	//*/
	
	{	.active_state = false,
		.button_bits_nr= BTN_8_PHONE_END, 
		.button_press_count=0,
		.button_press_count2 =0, 
		.button_event_type = EVENT_SHORT_BUT,  
		.button_funcion_to_exe = FNC_BT_SKIP_BACKWARD},
	
		#ifdef CONTROL_NEXTPREW_ON_MENU
		//next prew songs su up and down arrows
	{ .active_state = false,
		.button_bits_nr= BTN_2_PREW_DISP, 
	  .button_press_count=0,
		.button_press_count2 =0, 
		.button_event_type = EVENT_SHORT_BUT, 
		.button_funcion_to_exe = FNC_BT_SKIP_FORWARD},
	
	{	.active_state = false,
		.button_bits_nr= BTN_1_NEXT_DISP, 
		.button_press_count=0,
		.button_press_count2 =0, 
		.button_event_type = EVENT_SHORT_BUT,  
		.button_funcion_to_exe = FNC_BT_SKIP_BACKWARD},
		#endif
		
		///*
		{ .active_state = false,
		.button_bits_nr= BTN_7_PHONE_SEND, 
	  .button_press_count=0,
		.button_press_count2 =0, 
		.button_event_type = EVENT_LONG_LONG_EVENT_BUT, 
		.button_funcion_to_exe = FNC_RECALL_CALL},
		//*/
		
		
	
	
	{	.active_state = false,
		.button_bits_nr= BTN_8_PHONE_END, 
		.button_press_count=0,
		.button_press_count2 =0, 
		//.button_event_type = EVENT_LONG_LONG_EVENT_BUT,
		.button_event_type = EVENT_LONG_BUT,  		
		.button_funcion_to_exe = FNC_TOGGLE_SOURCE}, 
	
	#ifdef CONTROL_OFF_ON_NEXTDISP
	//ON OFF funkcija
	{	.active_state = false,
		.button_bits_nr= BTN_1_NEXT_DISP, 
		.button_press_count=0,
		.button_press_count2 =0, 
		.button_event_type = EVENT_LONG_LONG_EVENT_BUT,  		
		.button_funcion_to_exe = FNC_BT_POWER_OFF_ON}, 
#else
		//Source perjungimas
	{	.active_state = false,
		.button_bits_nr= BTN_1_NEXT_DISP, 
		.button_press_count=0,
		.button_press_count2 =0, 
		//.button_event_type = EVENT_LONG_LONG_EVENT_BUT,
		.button_event_type = EVENT_LONG_BUT,  		
		.button_funcion_to_exe = FNC_TOGGLE_SOURCE}, 
		
		
#endif		
		
		//RESTART BLUETOOTH
	{	.active_state = false,
		.button_bits_nr= BTN_2_PREW_DISP, 
		.button_press_count=0,
		.button_press_count2 =0, 
		.button_event_type = EVENT_LONG_LONG_EVENT_BUT,
		//.button_event_type = EVENT_LONG_BUT,  		
		.button_funcion_to_exe = FNC_RESTART_BT},
	
	
	#ifdef CONTROL_ON_ON_MENU_4_3
	//ISJUNGIMAS/IJUNGIMAS jei skambina ir norima kalbeti 
	{	.active_state = false,
		.button_bits_nr= BTN_4, 
		.button_press_count=0,
		.button_press_count2 =0, 
		//.button_event_type = EVENT_LONG_LONG_EVENT_BUT,
		.button_event_type = EVENT_LONG_BUT, 
		.button_funcion_to_exe = FNC_BT_POWER_OFF_ON},
	
		//Play pause funkcija
	{	.active_state = false,
		.button_bits_nr= BTN_3, 
		.button_press_count=0,
		.button_press_count2 =0, 
		.button_event_type = EVENT_SHORT_BUT,  
		.button_funcion_to_exe = FNC_BT_SET_PLAY_PAUSE},
	#endif
	
	#ifdef VOLUME_BT_ON
	//VOLUME CONTROL FOR TESTS
	{	.active_state = false,
		.button_bits_nr= BTN_5_PLIUS_KEY, 
		.button_press_count=0,
		.button_press_count2 =0, 
		.button_event_type = EVENT_SHORT_BUT,  
		.button_funcion_to_exe = FNC_VOLUME_UP},
	
		//VOLUME CONTROL FOR TESTS
	{	.active_state = false,
		.button_bits_nr= BTN_6_MINUS_KEY, 
		.button_press_count=0,
		.button_press_count2 =0, 
		.button_event_type = EVENT_SHORT_BUT,  
		.button_funcion_to_exe = FNC_VOLUME_DOWN},
	#endif	
		//*/
  #ifdef COTROL_PLAY_PAUSE_ON_LONG_PRESS_VOLUME_DOWN
	{	.active_state = false,
		.button_bits_nr= BTN_6_MINUS_KEY, 
		.button_press_count=0,
		.button_press_count2 =0, 
		.button_event_type = EVENT_LONG_BUT,  
		.button_funcion_to_exe = FNC_BT_SET_PLAY_PAUSE},
	#endif
	
	#ifndef VOLUME_BT_ON
		{.active_state = false,.button_bits_nr= 0, .button_press_count=0,.button_press_count2 =0, .button_event_type = 0, .button_funcion_to_exe = 0},
		{.active_state = false,.button_bits_nr= 0, .button_press_count=0,.button_press_count2 =0, .button_event_type = 0, .button_funcion_to_exe = 0},
	#endif	
		
	#ifndef CONTROL_NEXTPREW_ON_MENU
		{.active_state = false,.button_bits_nr= 0, .button_press_count=0,.button_press_count2 =0, .button_event_type = 0, .button_funcion_to_exe = 0},
		{.active_state = false,.button_bits_nr= 0, .button_press_count=0,.button_press_count2 =0, .button_event_type = 0, .button_funcion_to_exe = 0},
	#endif		
		
	#ifndef CONTROL_ON_ON_MENU_4_3
		{.active_state = false,.button_bits_nr= 0, .button_press_count=0,.button_press_count2 =0, .button_event_type = 0, .button_funcion_to_exe = 0},
		{.active_state = false,.button_bits_nr= 0, .button_press_count=0,.button_press_count2 =0, .button_event_type = 0, .button_funcion_to_exe = 0},
	#endif
		
	#ifndef COTROL_PLAY_PAUSE_ON_LONG_PRESS_VOLUME_DOWN
		{.active_state = false,.button_bits_nr= 0, .button_press_count=0,.button_press_count2 =0, .button_event_type = 0, .button_funcion_to_exe = 0},
	#endif
	
	{.active_state = false,.button_bits_nr= 0, .button_press_count=0,.button_press_count2 =0, .button_event_type = 0, .button_funcion_to_exe = 0},
	{.active_state = false,.button_bits_nr= 0, .button_press_count=0,.button_press_count2 =0, .button_event_type = 0, .button_funcion_to_exe = 0},
	{.active_state = false,.button_bits_nr= 0, .button_press_count=0,.button_press_count2 =0, .button_event_type = 0, .button_funcion_to_exe = 0},
	//{.active_state = false,.button_bits_nr= 0, .button_press_count=0,.button_press_count2 =0, .button_event_type = 0, .button_funcion_to_exe = 0},
	//{.active_state = false,.button_bits_nr= 0, .button_press_count=0,.button_press_count2 =0, .button_event_type = 0, .button_funcion_to_exe = 0},
	//{.active_state = false,.button_bits_nr= 0, .button_press_count=0,.button_press_count2 =0, .button_event_type = 0, .button_funcion_to_exe = 0},
	//{.active_state = false,.button_bits_nr= 0, .button_press_count=0,.button_press_count2 =0, .button_event_type = 0, .button_funcion_to_exe = 0},
	
};

uint32_t buttons_bit_now;

void Mercedes_buttons_control_init()
{
		for (uint8_t i=0;i<SIZE_BUTTONS_LINKS;i++)
		{
			buttons_[i].event_hit = false;
		}

}


void Mercedes_buttons_control_handler()
{
	//while greitis tikriname ar nera evento
		for (uint8_t i=0;i<SIZE_BUTTONS_LINKS;i++)
		{
			if ( buttons_[i].active_state)
			{
				//PRINTF("a%u\r\n",i);
				//buttons_[i].button_press_count++;
				if (buttons_[i].event_hit==false)
				{
					if (buttons_[i].button_event_type==EVENT_LONG_BUT) //jeigu ilgas paspaudimas apie >2s
					{
						 if (buttons_[i].button_press_count>LONG_PRESS_TIME_EVENT)
						 {
								Mercedes_buttons_control(buttons_[i].button_funcion_to_exe);
							 buttons_[i].event_hit = true;
						 }
						 
					}
					else if (buttons_[i].button_event_type==EVENT_LONG_LONG_EVENT_BUT) //jeigu ilgas ilgas paspaudimas apie 5s
					{
						
						if (buttons_[i].button_press_count>LONG_LONG_PRESS_TIME_EVENT)
						 {
								Mercedes_buttons_control(buttons_[i].button_funcion_to_exe);
							 buttons_[i].event_hit = true;
						 }
					
					}
					else if (buttons_[i].button_event_type==EVENT_VERY_LONG_EVENT_BUT) //apie 25s
					{
						if (buttons_[i].button_press_count>VERY_LONG_PRESS_TIME_EVENT)
						 {
								Mercedes_buttons_control(buttons_[i].button_funcion_to_exe);
							 buttons_[i].event_hit = true;
						 }
					
					}
					
					
				}
				
				
			}
			
		}

}

void Mercedes_buttons_control_systick()
{
//1khz
		for (uint8_t i=0;i<SIZE_BUTTONS_LINKS;i++)
		{
			if ( buttons_[i].active_state)
			{
				buttons_[i].button_press_count++;
			}
			
		}

}


void Mercedes_buttons_get_CAN_buttons_data(uint8_t *data)
{
	PRINTF("b%u\r\n",data[0]);
	//is while eventas :)
	buttons_bit_now = data[0];

	if (buttons_bit_now!=0)
	{
		for (uint8_t i=0;i<SIZE_BUTTONS_LINKS;i++)
		{
			if (buttons_bit_now == buttons_[i].button_bits_nr)
			{
				buttons_[i].active_state = true;
			}
			
		}


	}
	else
	{
		for (uint8_t i=0;i<SIZE_BUTTONS_LINKS;i++)
		{
			
			if (buttons_[i].active_state)
			{
				
						if (buttons_[i].button_event_type==EVENT_SHORT_BUT) //jeigu trumpas paspaudimas
						{
							
								if (buttons_[i].button_press_count>SHORT_PRESS_TIME_MIN && buttons_[i].button_press_count<SHORT_PRESS_TIME_MAX)
							 {
								 PRINTF("btn active short %u press %lu \r\n",i,buttons_[i].button_press_count);
								 //jeigu reikiam rezyje
									Mercedes_buttons_control(buttons_[i].button_funcion_to_exe);

							 }
							
						}
						else
						{
							PRINTF("btn active OTHER %u press %lu \r\n",i,buttons_[i].button_press_count);
						}
				 
			}
			
			buttons_[i].button_press_count=0;
			buttons_[i].active_state = false;
			buttons_[i].event_hit = false;
		}


	}
	

}



void Mercedes_buttons_control(uint32_t event_to_exe)
{
  //is while kolinamas
	switch (event_to_exe)
	{
		/*
		FNC_BT_SKIP_FORWARD,
	FNC_BT_SKIP_BACKWARD,
	FNC_BT_SET_PLAY_PAUSE,
	
	FNC_BT_POWER_OFF_ON,
	FNC_RESTART_BT,
	
	FNC_ANSWER_CALL, //answer,end call, held accept
	FNC_REJECT_CALL,
	
	FNC_TOGGLE_SOURCE,
		*/
		
		
		case FNC_BT_SKIP_FORWARD:
		case FNC_ANSWER_CALL:
			
		if (BT_control_GET_CALL_ACTIVE_INCOMING_OUTGOING())
		{
		  BT_control_SET_CALL_ANSWER__END__HELD();
			PRINTF("MCD Answer\r\n");
		}
		else
		{
			BT_control_SET_BT_SKIP_FORWARD();
			PRINTF("MCD Skip forward\r\n");
		
		}
		
		
		break;
		
		case FNC_BT_SKIP_BACKWARD:
		case FNC_REJECT_CALL:
		
		
		if (BT_control_GET_CALL_ACTIVE_INCOMING_OUTGOING())
		{
				BT_control_SET_CALL_REJECT_HELDREJECT();
				PRINTF("MCD call reject\r\n");
		}
		else
		{
			BT_control_SET_BT_SKIP_BACKWARD();
			PRINTF("MCD Skip backward\r\n");
		}
		break;
		
		case FNC_BT_SET_PLAY_PAUSE:
		
		BT_control_SET_PLAY_PAUSE();
		PRINTF("MCD play/pause\r\n");
		 
		break;
		
		
		case FNC_BT_POWER_OFF_ON:
		
		BT_control_SET_BT_POWER_OFF(); //tinka ir isjungimui ir ijungimui
		CD4052_temporary_switch_to_BT_source(); //laikinai ijungiam BT kitaip nezinosim ar ijungia /isjungia
		PRINTF("MCD power on/off \r\n");
		
		break;
		
		case FNC_RESTART_BT:
		
		BT_control_restart_BT(); //restartina arba ijungia
		CD4052_temporary_switch_to_BT_source(); //laikinai ijungiam BT kitaip nezinosim ar restartina
		
		PRINTF("MCD power restart \r\n");
		
		break;
		
		
		case FNC_RECALL_CALL:
		
		CD4052_if_BT_off_turnON_delay20s();
		BT_control_SET_RECALL_LAST();
		PRINTF("MCD recall last\r\n");
		 
		break;
		
		
		case FNC_TOGGLE_SOURCE:
			
		CD4052_SET_TOGGLE_SOURCE();
		PRINTF("MCD toggle_source \r\n");
		
		break;
		
		case FNC_VOLUME_UP:
			
		if(CD4052_source_now_not_BT()) {break;}
		BT_control_SET_BT_VOLUME_UP();
		PRINTF("Volume up\r\n");
		 
		break;
		
		case FNC_VOLUME_DOWN:
		
		if(CD4052_source_now_not_BT()) {break;}
		BT_control_SET_BT_VOLUME_DOWN();
		PRINTF("Volume down\r\n");
		 
			
		break;
		
	
	
		default:
			break;
	
	}




}


