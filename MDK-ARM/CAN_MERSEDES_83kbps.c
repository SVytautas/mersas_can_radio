#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "uart2_dbg.h"
#include "main.h"
#include "Mercedes_buttons_control.h"
#include "CAN_MERSEDES_83kbps.h"

volatile bool get_radio_code = false;

void process_can_mesages();


CAN_HandleTypeDef hcan;

static void MX_CAN_Init(void);
/**
  * @brief CAN Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

	/*
Bit 36Mhz
Rate	  accuracy	PRe	NubOFQUanta	Seg1	Seg2	CAN_BTR
	83.333	-0.000072000	24	18	15	2	88.9	 0x001e0017
	83.333	-0.000064000	27	16	13	2	87.5	 0x001c001a
	83.333	-0.000048000	36	12	10	1	91.7	 0x00090023
	83.333	-0.000036000	48	9		7		1	88.9	 0x0006002f
	83.333	-0.000032000	54	8		6		1	87.5	 0x00050035
*/
	
//tampa 32Mhz pakeisti!!!!
	/*
83.3333	-0.0000064000	24	16	13	2	87.5	 0x001c0017
83.3333	-0.0000048000	32	12	10	1	91.7	 0x0009001f
83.3333	-0.0000032000	48	8	6	1	87.5	 0x0005002f
	*/
	
  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 24; //32Mhz
  //hcan.Init.Mode = CAN_MODE_SILENT;
	hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_13TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
	///*
	CAN_FilterTypeDef  sFilterConfig;

	
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;

  if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
  {
    
    Error_Handler();
  }
	//*/
	
	
	if (HAL_CAN_Start(&hcan) != HAL_OK)
  {
		PRINTF("CAN start error\r\n");
    /* Start Error */
    Error_Handler();
  }
	
	/* Activate CAN RX notification */
  if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    /* Notification Error */
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */
  //HAL_CAN_Start(&hcan); //paleidziame can moduli
  /* USER CODE END CAN_Init 2 */
	PRINTF("CAN init\r\n");

}


void CAN_mersas_init(void)
{

	MX_CAN_Init();
}

uint32_t cpounter__ = 0;
void CAN_mersedes_handler()
{
	process_can_mesages();
	
	/*
	cpounter__++;

		if (cpounter__>50000)
		{
			cpounter__ = 0;
		//PRINTF("!");
			
	  CAN_TxHeaderTypeDef TxHeader;
		TxHeader.StdId = 0x321;
  TxHeader.ExtId = 0x01;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.DLC = 2;
  TxHeader.TransmitGlobalTime = DISABLE;
		 

  uint8_t txData[8] = { 0xAA, 0xAA, 0xAA, 0xAA, 0x05, 0x06, 0x07, 0x08} ;
	//HAL_CAN_AddTxMessage(&hcan,&TxHeader,txData,(uint32_t *)CAN_TX_MAILBOX0);
			
		}
		//*/
		/*
		if (HAL_CAN_GetRxFifoFillLevel(&hcan,CAN_RX_FIFO0)>0)
		{
				CAN_RxHeaderTypeDef rxmsg;
				uint8_t rxData[8];

				if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rxmsg, rxData) != HAL_OK) {
					Error_Handler();
				}
				
				PRINTF("Can ID: %x\r\n",rxmsg.StdId);
				//PRINTF("Can ID: %x\r\n",rxmsg.ExtId);
		}
		*/
		
		
		
}

void CAN_mersas_receiveFIFO0_irq_handler(void)
{
	//PRINTF("\r\nget can msg?\r\n");
	//PRINTF("!");

}

//uint32_t counter_msgget = 0;
CAN_RxHeaderTypeDef rxmsg;

typedef struct 
{
    uint32_t CAN_id;
    uint8_t data[8];

}CAN_msg_t;


 CAN_msg_t can_msg_read_buff[256];
uint8_t read_ptr=0;
uint8_t write_ptr=0;

//uintKOMBI_A5

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  /* Get RX message */
					
	//uint8_t rxData[8];
	//if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxmsg, rxData) != HAL_OK) 
	//if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxmsg, can_msg_read_buff[write_ptr].data) != HAL_OK) 
	//{
	//	Error_Handler();
		//PRINTF("Can ID: %x\r\n",rxmsg.StdId);
	//}
	
	//CAN_msg_t tempa;
	//tempa.CAN_id = rxmsg.StdId;
  //memcpy(tempa.data, rxData,8);	
	
	//idedam  can msg
	//can_msg_read_buff[write_ptr++ & (READ_BUFF_SIZE-1)] = tempa;
  //  write_ptr = write_ptr & (READ_BUFF_SIZE-1);
	
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxmsg, can_msg_read_buff[write_ptr].data);
	can_msg_read_buff[write_ptr].CAN_id = rxmsg.StdId;
	write_ptr++;

}

void process_can_mesages()
{
    while (read_ptr!=write_ptr)
    {
			PRINTF("get %x : %x\r\n",can_msg_read_buff[read_ptr].CAN_id, can_msg_read_buff[read_ptr].data[0]); 
			
        //CAN_msg_t process_can_data = can_msg_read_buff[read_ptr++ & (READ_BUFF_SIZE-1)];
        //read_ptr = read_ptr & (READ_BUFF_SIZE-1);
			
        //switch((uint16_t)process_can_data.CAN_id)
				switch(can_msg_read_buff[read_ptr].CAN_id)
        {
            //filer now testui kurie per daug sneka
						case 0x0000: //ECU NAME: UNKNOWN, ID: 0x0000. MSG COUNT: 35
						case 0x0002: //ECU NAME: EZS_A2, ID: 0x0002. MSG COUNT: 4
						case 0x0003: //ECU NAME: EZS_A7, ID: 0x0003. MSG COUNT: 26
						case 0x0016: //ECU NAME: EZS_A11, ID: 0x0016. MSG COUNT: 1
						case 0x010a: //ECU NAME: EZS_A10, ID: 0x010A. MSG COUNT: 6
						case 0x0194: //ECU NAME: KOMBI_A7, ID: 0x0194. MSG COUNT: 3
						case 0x01d8: //ECU NAME: TP_MSS_AGW3, ID: 0x01D8. MSG COUNT: 1
						case 0x0058: //ECU NAME: D_RS_NAVI, ID: 0x058A. MSG COUNT: 1
						case 0x009e: //ECU NAME: TP_MSS_AGW3, ID: 0x01D8. MSG COUNT: 1
						case 0x001e: //ECU NAME: D_RS_NAVI, ID: 0x058A. MSG COUNT: 1
            case 0x0405: //MSG NAME: NM - Netzwerkmanagement, OFFSET 0, LENGTH 64
            case 0x000c: //ECU NAME: KOMBI_A1, ID: 0x000C. MSG COUNT: 28
            case 0x0004: //SAM_H_A1, ID: 0x0004. MSG COUNT: 47
            case 0x0014: //ECU NAME: DBE_A1, ID: 0x0014. MSG COUNT: 19
						case 0x0017: //SAM_V_A2, ID: 0x0017. MSG COUNT: 4
            case 0x0006: //ECU NAME: MRM_A1, ID: 0x0006. MSG COUNT: 22
            case 0x0090: //ECU NAME: SAM_H_A2, ID: 0x0090. MSG COUNT: 3
						case 0x0407: //MSG NAME: NM - Netzwerkmanagement, OFFSET 0, LENGTH 64
            case 0x01d0: //ECU NAME: TP_KOMBI_AGW1, ID: 0x01D0. MSG COUNT: 1
						case 0x0030: //ECU NAME: KLA_A1, ID: 0x0030. MSG COUNT: 20
            case 0x0130: //ECU NAME: DBE_A1, ID: 0x0014. MSG COUNT: 19
						case 0x02b0: //ECU NAME: PTS_A1, ID: 0x02B0. MSG COUNT: 1
            case 0x040a: //ECU NAME: NM_TVR, ID: 0x040A. MSG COUNT: 1
						case 0x0012: //ECU NAME: ARMADA_A1, ID: 0x0012. MSG COUNT: 12
            case 0x040b: //ECU NAME: NM_THR, ID: 0x040B. MSG COUNT: 1 ???????????????????????
						case 0x002c: //ECU NAME: OBF_A1, ID: 0x002C. MSG COUNT: 10
            case 0x0410: //ECU NAME: NM_AAG, ID: 0x0410. MSG COUNT: 1
						case 0x009A: //ECU NAME: THL_A1, ID: 0x009A. MSG COUNT: 5
						case 0x0414: //ECU NAME: NM_KOMBI, ID: 0x0414. MSG COUNT: 1
            case 0x00d4: //ECU NAME: DBE_A3, ID: 0x02D4. MSG COUNT: 1
						case 0x0400: //ECU NAME: NM_EZS, ID: 0x0400. MSG COUNT: 1
						case 0x0402: //ECU NAME: NM_SAM_V, ID: 0x0402. MSG COUNT: 1
						case 0x0390: //ECU NAME: EZS_A8, ID: 0x0390. MSG COUNT: 27
						case 0x000a: //ECU NAME: SAM_V_A1, ID: 0x000A. MSG COUNT: 43
						case 0x0408: //ECU NAME: NM_TVL, ID: 0x0408. MSG COUNT: 1
						case 0x00b2: //ECU NAME: APPL_SG_SB, ID: 0x06B2. MSG COUNT: 1 ??????????????????????
						case 0x0019: //ECU NAME: TVR_A3, ID: 0x0019. MSG COUNT: 10
						case 0x0415: //ECU NAME: NM_MRM, ID: 0x0415. MSG COUNT: 1
						case 0x01b2: //ECU NAME: KG_A1, ID: 0x01B2. MSG COUNT: 15
						case 0x041C: //ECU NAME: NM_ARMADA, ID: 0x041C. MSG COUNT: 1
						case 0x0018: //ECU NAME: TVL_A3, ID: 0x0018. MSG COUNT: 10
						case 0x01A4: //ECU NAME: TP_AGW_KOMBI1, ID: 0x01A4. MSG COUNT: 1
						case 0x001F: //ECU NAME: EZS_A5, ID: 0x001F. MSG COUNT: 45
						case 0x0411: //ECU NAME: NM_KLA, ID: 0x0411. MSG COUNT: 1
						case 0x0413: //ECU NAME: NM_PTS, ID: 0x0413. MSG COUNT: 1
						//case 0x0416: //ECU NAME: NM_AGW, ID: 0x0416. MSG COUNT: 1   //!!!!!!!!!!!!!!!!!sitas gaunamas taip
						//case 0x01a8: //ECU NAME: MRM_A2, ID: 0x01A8. MSG COUNT: 16 //mygtukai
						case 0x009c: //ECU NAME: THR_A1, ID: 0x009C. MSG COUNT: 5
						case 0x0409: //ECU NAME: NM_THL, ID: 0x0409. MSG COUNT: 1
						case 0x0403: //ECU NAME: NM_SAM_H, ID: 0x0403. MSG COUNT: 1
           //PRINTF("get %x : %x\r\n",can_msg_read_buff[read_ptr].CAN_id, can_msg_read_buff[read_ptr].data[0]); 
            break;
						
			case 0x0416: //ECU NAME: NM_AGW, ID: 0x0416. MSG COUNT: 1   //!!!!!!!!!!!!!!!!!sitas gaunamas taip
				get_radio_code = true;
			break;
            
			case 0x01CA: 
			case 0x032A:
			case 0x01a8:						
			//case 0xffff: 
				Mercedes_buttons_get_CAN_buttons_data(can_msg_read_buff[read_ptr].data);
			
					//PRINTF("btn %x : %x",process_can_data.CAN_id, process_can_data.data[0]);
				
					//ECU NAME: KOMBI_A8, ID: 0x032A. MSG COUNT: 25
					//ECU NAME: KOMBI_A5, ID: 0x01CA. MSG COUNT: 25 //LENGHT 32
					/*MSG NAME: KI_STAT - status combi, OFFSET 0, LENGTH 8
						MSG NAME: BUTTON_4_2 - phone End, OFFSET 8, LENGTH 1
						MSG NAME: BUTTON_4_1 - phone Send, OFFSET 9, LENGTH 1
						MSG NAME: BUTTON_3_2 - "-" key, OFFSET 10, LENGTH 1
						MSG NAME: BUTTON_3_1 - "+" Key, OFFSET 11, LENGTH 1
						MSG NAME: BUTTON_2_2 - reserve, OFFSET 12, LENGTH 1
						MSG NAME: BUTTON_2_1 - reserve, OFFSET 13, LENGTH 1
						MSG NAME: BUTTON_1_2 - Previous display, OFFSET 14, LENGTH 1
						MSG NAME: BUTTON_1_1 - Next display, OFFSET 15, LENGTH 1
						MSG NAME: BUTTON_8_2 - reserve, OFFSET 16, LENGTH 1
						MSG NAME: BUTTON_8_1 - reserve, OFFSET 17, LENGTH 1
						MSG NAME: BUTTON_7_2 - reserve, OFFSET 18, LENGTH 1
						MSG NAME: BUTTON_7_1 - reserve, OFFSET 19, LENGTH 1
						MSG NAME: BUTTON_6_2 - reserve, OFFSET 20, LENGTH 1
						MSG NAME: BUTTON_6_1 - reserve, OFFSET 21, LENGTH 1
						MSG NAME: BUTTON_5_2 - reserve, OFFSET 22, LENGTH 1
						MSG NAME: BUTTON_5_1 - reserve, OFFSET 23, LENGTH 1
						MSG NAME: PTT_4_2 - reserve, OFFSET 24, LENGTH 1
						MSG NAME: PTT_4_1 - reserve, OFFSET 25, LENGTH 1
						MSG NAME: PTT_3_2 - reserve, OFFSET 26, LENGTH 1
						MSG NAME: PTT_3_1 - reserve, OFFSET 27, LENGTH 1
						MSG NAME: PTT_2_2 - reserve, OFFSET 28, LENGTH 1
						MSG NAME: PTT_2_1 - reserve, OFFSET 29, LENGTH 1
						MSG NAME: PTT_1_2 - disable Linguatronic, OFFSET 30, LENGTH 1
						MSG NAME: PTT_1_1 - Enable Linguatronic, OFFSET 31, LENGTH 1
				*/
				
				/*
				ECU NAME: MRM_A2, ID: 0x01A8. MSG COUNT: 16
				MSG NAME: WIPPE_4_2 - Button on the bottom right to bottom, OFFSET 0, LENGTH 1
				MSG NAME: WIPPE_4_1 - Button on the right bottom to top, OFFSET 1, LENGTH 1
				MSG NAME: WIPPE_3_2 - Button on the right top to bottom, OFFSET 2, LENGTH 1
				MSG NAME: WIPPE_3_1 - Button on the top right to the top, OFFSET 3, LENGTH 1
				MSG NAME: WIPPE_2_2 - Button on the bottom left to bottom, OFFSET 4, LENGTH 1
				MSG NAME: WIPPE_2_1 - Button on the left bottom to top, OFFSET 5, LENGTH 1
				MSG NAME: WIPPE_1_2 - Button top left to bottom, OFFSET 6, LENGTH 1
				MSG NAME: WIPPE_1_1 - Button top left to top, OFFSET 7, LENGTH 1
				MSG NAME: WIPPE_8_2 - reserve, OFFSET 8, LENGTH 1
				MSG NAME: WIPPE_8_1 - reserve, OFFSET 9, LENGTH 1
				MSG NAME: WIPPE_7_2 - reserve, OFFSET 10, LENGTH 1
				MSG NAME: WIPPE_7_1 - reserve, OFFSET 11, LENGTH 1
				MSG NAME: WIPPE_6_2 - reserve, OFFSET 12, LENGTH 1
				MSG NAME: WIPPE_6_1 - reserve, OFFSET 13, LENGTH 1
				MSG NAME: WIPPE_5_2 - reserve, OFFSET 14, LENGTH 1
				MSG NAME: WIPPE_5_1 - reserve, OFFSET 15, LENGTH 1
				*/
				
				//isirasome 4paketus? bus 32baitai

			break;
            
            default:
            break;
        }
				
			read_ptr++;	
				
        
        
    }
    
    
    
}



void test_button_pres_simu(uint8_t buttons)
{
	can_msg_read_buff[write_ptr].CAN_id = 0x01a8;
	can_msg_read_buff[write_ptr].data[0] = buttons;
	write_ptr++;
	PRINTF("btn %u  %u\r\n ", buttons,write_ptr);
}

//test 
uint32_t test_cnt = 0; //+1s
uint8_t button_ = 0;
void systick1s()
{
	//1Khz
	//PRINTF("rx %u ID: %x\r\n", counter_msgget, ID_last );
	if (test_cnt>0)
	{
		test_cnt--;
		//PRINTF("btn %u\r\n", button_);
		//PRINTF("tst %u\r\n", test_cnt);
		if ((test_cnt%10) == 0) {test_button_pres_simu(button_); }
		if (test_cnt==0)
		{
			test_button_pres_simu(0);
		}
	}
	
	
	/*
	  CAN_TxHeaderTypeDef TxHeader;
		TxHeader.StdId = 0x321;
	  TxHeader.ExtId = 0x11;
		TxHeader.RTR = CAN_RTR_DATA;
		TxHeader.IDE = CAN_ID_STD;
		TxHeader.DLC = 8;
		 

  uint8_t txData[8] = { 0xAA, 0xAA, 0xAA, 0xAA, 0x05, 0x06, 0x07, 0x08} ;
	//HAL_CAN_AddTxMessage(&hcan,&TxHeader,txData,(uint32_t *)CAN_TX_MAILBOX0);
	//*/
	
	
	//PRINTF("fifo %u\r\n", HAL_CAN_GetRxFifoFillLevel(&hcan,CAN_RX_FIFO0));
	//PRINTF("tx %u\r\n", HAL_CAN_GetTxMailboxesFreeLevel(&hcan));
	
	//PRINTF("fifo1 %u\r\n", HAL_CAN_GetRxFifoFillLevel(&hcan,CAN_RX_FIFO1));
//HAL_CAN_GetRxFifoFillLevel(&hcan,CAN_RX_FIFO0);
}

void test_short_fnc_can()
{
		test_cnt = 800;
		button_ = BTN_6_MINUS_KEY;
}

void test_long_fnc_can()
{
		test_cnt = 6000;
		button_ = BTN_6_MINUS_KEY;
		//test_cnta_long = 6000;
}


void test_custom(uint8_t buttons, uint32_t time)
{

		button_ = 1<<(buttons-1);
		test_cnt = time;
	PRINTF("%u\r\n",test_cnt);

}









