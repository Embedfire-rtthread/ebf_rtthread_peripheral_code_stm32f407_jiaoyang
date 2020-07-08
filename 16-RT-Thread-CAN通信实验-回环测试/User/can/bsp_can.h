#ifndef __CAN_H
#define	__CAN_H

#include "stm32f4xx.h"
#include <stdio.h>

typedef struct __Tx_Packge
{
	CAN_TxHeaderTypeDef *TxHeader;
	uint8_t Payload[8];
}Tx_Packge;

extern CAN_HandleTypeDef Can_Handle;
extern uint8_t Rx_Buff[8];
extern CAN_TxHeaderTypeDef TxHeader;		//����ͷ
extern CAN_RxHeaderTypeDef RxHeader;		//����ͷ
extern Tx_Packge my_Packge;
#define CANx                       CAN1
#define CAN_CLK_ENABLE()           __CAN1_CLK_ENABLE()
#define CAN_RX_IRQ				   CAN1_RX0_IRQn
#define CAN_RX_IRQHandler		   CAN1_RX0_IRQHandler

#define CAN_RX_PIN                 GPIO_PIN_9
#define CAN_TX_PIN                 GPIO_PIN_9
#define CAN_TX_GPIO_PORT           GPIOB
#define CAN_RX_GPIO_PORT           GPIOI
#define CAN_TX_GPIO_CLK_ENABLE()   __GPIOB_CLK_ENABLE()
#define CAN_RX_GPIO_CLK_ENABLE()   __GPIOI_CLK_ENABLE()
#define CAN_AF_PORT                GPIO_AF9_CAN1





/*debug*/

#define CAN_DEBUG_ON         1
#define CAN_DEBUG_ARRAY_ON   1
#define CAN_DEBUG_FUNC_ON    1
   
   
// Log define
#define CAN_INFO(fmt,arg...)           printf("<<-CAN-INFO->> "fmt"\n",##arg)
#define CAN_ERROR(fmt,arg...)          printf("<<-CAN-ERROR->> "fmt"\n",##arg)
#define CAN_DEBUG(fmt,arg...)          do{\
                                         if(CAN_DEBUG_ON)\
                                         printf("<<-CAN-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                       }while(0)

#define CAN_DEBUG_ARRAY(array, num)    do{\
                                         int32_t i;\
                                         uint8_t* a = array;\
                                         if(CAN_DEBUG_ARRAY_ON)\
                                         {\
                                            printf("<<-CAN-DEBUG-ARRAY->>\n");\
                                            for (i = 0; i < (num); i++)\
                                            {\
                                                printf("%02x   ", (a)[i]);\
                                                if ((i + 1 ) %10 == 0)\
                                                {\
                                                    printf("\n");\
                                                }\
                                            }\
                                            printf("\n");\
                                        }\
                                       }while(0)

#define CAN_DEBUG_FUNC()               do{\
                                         if(CAN_DEBUG_FUNC_ON)\
                                         printf("<<-CAN-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)






static void CAN_GPIO_Config(void);
static void CAN_NVIC_Config(void);
static void CAN_Mode_Config(void);
static void CAN_Filter_Config(void);
void CAN_Config(void);
void CAN_SetMsg(void);
void Init_RxMes(void);

#endif







