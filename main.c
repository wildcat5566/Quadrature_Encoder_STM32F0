#include "stm32f0xx.h"          
#include "stm32f0xx_gpio.h"     // Pins Interface
#include "stm32f0xx_rcc.h"      // Enable peripheral clock
#include "stm32f0xx_exti.h"     // External interruption
#include "stm32f0xx_syscfg.h"   // NVIC Settings
#include "stm32f0xx_misc.h"     // Connect exti and gpio

int xa, xb;                     // Encoder channel A 
int stt, dstt, stt_prev;
int count = 0;

void XA_config(void){ //A0
		
	GPIO_InitTypeDef GPIO_InitStruct;                  // GPIO Struct oobject
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;             // Use pin#0
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;          // Pinmode = In
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;     // 50 MHZ (max) clock freq
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;        // Push pull mode
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;      // No pull up / down
	GPIO_Init(GPIOA, &GPIO_InitStruct);                // Init pin A0
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;         // #0 pins connected to line #0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn; // 0 & 1 Lines share a handler
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;    // Choose priority among 0~3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void XB_config(void){ //A3
		
	GPIO_InitTypeDef GPIO_InitStruct2;
	GPIO_InitStruct2.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct2.GPIO_Mode = GPIO_Mode_IN;         
	GPIO_InitStruct2.GPIO_Speed = GPIO_Speed_50MHz;     
	GPIO_InitStruct2.GPIO_OType = GPIO_OType_PP;        
	GPIO_InitStruct2.GPIO_PuPd = GPIO_PuPd_NOPULL;             
	GPIO_Init(GPIOA, &GPIO_InitStruct2);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource3);
	
	EXTI_InitTypeDef EXTI_InitStructure2;
	EXTI_InitStructure2.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure2.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure2.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure2.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure2);
	
	NVIC_InitTypeDef NVIC_InitStructure2;
	NVIC_InitStructure2.NVIC_IRQChannel = EXTI2_3_IRQn;
	NVIC_InitStructure2.NVIC_IRQChannelPriority = 1;
	NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure2);
}

int main(void) {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	XA_config();
	XB_config();
	
	xa = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
	xb = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
	stt_prev = (xa != xb) + 2 * xb;

	for(;;) {} // wait for interruption
}

void EXTI0_1_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) { // interrupt flag set
        xa = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
				xb = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
				stt = (xa != xb) + 2 * xb;
				dstt = (stt - stt_prev + 4) % 4;
				count += (dstt % 2)*(dstt - 2);
				stt_prev = stt;
				EXTI_ClearFlag(EXTI_Line0); 
        EXTI_ClearITPendingBit(EXTI_Line0);      // Clear interrupt flag
    }
}
 
void EXTI2_3_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
        xa = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
				xb = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
				stt = (xa != xb) + 2 * xb;
				dstt = (stt - stt_prev + 4) % 4;
				count += (dstt % 2)*(dstt - 2);
				stt_prev = stt;
				EXTI_ClearFlag(EXTI_Line3); 
        EXTI_ClearITPendingBit(EXTI_Line3); 
    }
}

