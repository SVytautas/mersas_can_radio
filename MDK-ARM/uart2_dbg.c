#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "uart2_dbg.h"
#include "main.h"
#include "BT_control_gpio.h"
#include "CD4052.h"
#include "CAN_MERSEDES_83kbps.h"


#define UART_RX_SIZE 64
static uint8_t uart_rx_buff2[UART_RX_SIZE], uart_rx_buff_rp2=0, uart_rx_buff_wp2=0;
static uint8_t uart_tx_buff[256], uart_tx_buff_rp=0,uart_tx_buff_wp=0;
//komandu priemimo struck
uart_t uart_get=
{
   .gauta_wp = 0,
	 .gauta_rp = 0,
	 .cmd_wp   = 0,
	 .cmd_rp   = 0,
	 .cnt=0,

};

UART_HandleTypeDef huart1;

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
				__HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);
    }
}

void uart2_dbg_put_char(uint8_t charas)
{
   uart_tx_buff[uart_tx_buff_wp++]= charas;
   __HAL_UART_ENABLE_IT(&huart1, UART_IT_TXE);
}


void PRINTF2(const char *fmt, ...) // 
{
    va_list argp;
    va_start(argp, fmt);
    vprint( fmt, argp);
    va_end(argp);
}

///*
void PRINTF(const char *fmt, ...) // 
{
}
//*/




static void MX_USART1_UART_Init(void);
void uart2_dbg_init(void)
{
		MX_USART1_UART_Init();

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  /* USER CODE END USART1_Init 2 */

}

void uart2_dbg_irq_handler(void)
{
     /* USER CODE BEGIN USART1_IRQn 0 */
	//uartas_irq_handler();
    if((__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET) && 
       (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_RXNE) != RESET))
    {
        uart_rx_buff2[uart_rx_buff_wp2++] = (uint8_t)(huart1.Instance->DR & 0x00FF);
			  //PRINTF(".");
        if (uart_rx_buff_wp2>=UART_RX_SIZE) {uart_rx_buff_wp2=0;}
        /* Clear RXNE interrupt flag */
        __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
    }
		
		
    if((__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE) != RESET) &&
       (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_TXE) != RESET))
    {
        if (uart_tx_buff_rp==uart_tx_buff_wp) {
            __HAL_UART_DISABLE_IT(&huart1, UART_IT_TXE);
            __HAL_UART_ENABLE_IT(&huart1, UART_IT_TC);
        } else {
            huart1.Instance->DR = (uint8_t)(uart_tx_buff[uart_tx_buff_rp++] & (uint8_t)0xFF);
           // tx_o++;
        }
    }
    if((__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) != RESET) &&
       (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_TC) != RESET))
    {
      //  tx_busy = 0;
        __HAL_UART_DISABLE_IT(&huart1, UART_IT_TC);
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
					
					case (('O'<<8)|'N'): // 
						BT_control_SET_BT_POWER_ON();
					  PRINTF("ON \r\n");
					break;
					
					case (('O'<<8)|'F'): // 
						BT_control_SET_BT_POWER_OFF();
					  PRINTF("OF \r\n");
					break;
					
					case (('F'<<8)|'1'): // 
						BT_control_SET_BT_SKIP_FORWARD();
					  PRINTF("F1 \r\n");
					break;
					
					case (('B'<<8)|'1'): // 
						BT_control_SET_BT_SKIP_BACKWARD();
					  PRINTF("B1 \r\n");
					break;
					
					case (('P'<<8)|'1'): // 
						BT_control_SET_PLAY_PAUSE();
					  PRINTF("P1 \r\n");
					break;
					
					case (('S'<<8)|'2'): // 
						BT_control_SET_BT_STOP();
					  PRINTF("S2 \r\n");
					break;
					
					case (('C'<<8)|'S'): // call state (veikia)
					  PRINTF("call %u \r\n", BT_control_GET_CALL_ACTIVE_INCOMING_OUTGOING());
					break;
					
					case (('A'<<8)|'A'): // Audio active (veikia)
					  PRINTF("Audio %u \r\n", BT_control_GET_AUDIO_ACTIVE());
					break;
					
					case (('P'<<8)|'A'): // BT active (veikia)
					  PRINTF("Power %u \r\n", BT_control_GET_BT_ON());
					break;
					
					case (('S'<<8)|'R'): // Set radio
						CD4052_SET_RADIO();
					  PRINTF("SET radio\r\n");
					break;
					
					case (('S'<<8)|'A'): // Set AUX
						CD4052_SET_AUX_OUT();
					  PRINTF("SET AUX\r\n");
					break;
					
					case (('S'<<8)|'B'): // Set BT
						CD4052_SET_BT_OUT();
					  PRINTF("SET BT\r\n");
					break;
					
					case (('R'<<8)|'1'): // Set reconnect last
						//BT_control_SET_BT_RECONECT_LAST();
					  PRINTF("Reconect last RL\r\n");
					break;
					
					case (('R'<<8)|'B'): // Set reconnect last
						BT_control_restart_BT();
					  PRINTF("RB \r\n");
					break;
					
					case (('A'<<8)|'N'): // Set reconnect last
						BT_control_SET_CALL_ANSWER__END__HELD();
					  PRINTF("AN \r\n");
					break;
					
					case (('T'<<8)|'T'): // Set reconnect last
						test_short_fnc_can();
					  PRINTF("TT \r\n");
					break;
					
					
					case (('T'<<8)|'L'): // 
						test_long_fnc_can();
					  PRINTF("TL \r\n");
					break;
					
					
					case (('B'<<8)|'C'): // BT connected 
					  PRINTF("BC %u \r\n", BT_control_GET_BT_CONNECTED());
					break;
					
					case (('T'<<8)|'C'): // test custom
						test_custom(uart_get.gauta[uart_get.cmd_rp].skaic[0],(uint32_t)uart_get.gauta[uart_get.cmd_rp].skaic[1]);
					  PRINTF("TC %u %u\r\n", uart_get.gauta[uart_get.cmd_rp].skaic[0],uart_get.gauta[uart_get.cmd_rp].skaic[1] );
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


