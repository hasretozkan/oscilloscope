/**
  ******************************************************************************
  * @file    ADC/ADC1_DMA/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32_eval.h"
#include "stm3210c_eval_lcd.h"
#include <string.h>
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"


/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup ADC_ADC1_DMA
  * @{
  */ 


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
 
int x = 0;
int y = 0;
int i = 0;
int frekans = 100;
int sqfrekans = 100;
	char buffer[25];
		float value=0;
	int squf = 0;
	int secim = 0;
	int mazgal = 0;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_InitTypeDef ADC_InitStructure;
__IO uint16_t ADCConvertedValue;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void LCD_Config(void);
void ADC_Config(void);
void EXTI0_Config(void);
void EXTI15_10_Config(void);
void EXTI9_5_Config(void);
void Print_Circle(float value);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	uint16_t valueb=0;
	
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
       
  /* System clocks configuration ---------------------------------------------*/
  RCC_Configuration();

  /* GPIO configuration ------------------------------------------------------*/
  GPIO_Configuration();
	
	LCD_Config();
	LCD_SetTextColor(Blue);
	LCD_SetBackColor(LCD_COLOR_BLACK);
  EXTI0_Config();
	EXTI15_10_Config();
	EXTI9_5_Config();
	
  /* ADC1 configuration ------------------------------------------------------*/
	ADC_Config();

  while (1)
  {
		value=ADC_GetConversionValue(ADC1);
		if(floor(value/1000) != floor(valueb/1000)){
								Print_Circle(3.3*(0.00080566*(value/52.786842)));
		valueb = value;
		}
  }


}

void EXTI9_5_Config(void)
{
	/* Initialize Key Button(PB 09) mounted on STM3210X-EVAL board */
	
  GPIO_InitTypeDef   GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
	
  /* Enable GPIOB clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /* Configure PB.09 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Enable AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  /* Connect EXTI9 Line to PB.09 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);

  /* Configure EXTI9 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI9_5 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

}

void EXTI15_10_Config(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  /* Enable GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  /* Configure PC.13 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Enable AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  /* Connect EXTI13 Line to PC.13 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);

  /* Configure EXTI13 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI15_10 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

}

void EXTI0_Config(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  /* Enable GPIOA clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /* Configure PA.0 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Enable AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  /* Connect EXTI0 Line to PA.0 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);

}
/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  /* ADCCLK = PCLK2/2 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div2); 
#else
  /* ADCCLK = PCLK2/4 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
#endif

  /* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure PC.04 (ADC Channel14) as analog input -------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void ADC_Config(void)
{
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel14 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_239Cycles5);
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 1, ADC_SampleTime_1Cycles5);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
     
  /* Enable again ADC1 */
  ADC_Cmd(ADC1, ENABLE);
}

void LCD_Config(void)
{
	STM3210C_LCD_Init();
  /* Display message on STM3210X-EVAL LCD *************************************/
  /* Clear the LCD */ 
  LCD_Clear(LCD_COLOR_WHITE);

	  
		
	//LCD_DisplayStringLine(LCD_LINE_5, (uint8_t *)MESSAGE5);
}
void Print_Circle(float value)
{
	x = 300;
	LCD_Clear(LCD_COLOR_BLACK);
	LCD_SetTextColor(LCD_COLOR_YELLOW);
	LCD_DrawRect(10,300,120,280);
	
	if(mazgal == 1){
	LCD_DrawLine(30,300,280,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(50,300,280,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(70,300,280,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(90,300,280,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(110,300,280,LCD_DIR_HORIZONTAL);
	LCD_DrawLine(130,300,280,LCD_DIR_HORIZONTAL);

		for(i = 0;i<280;i++){
			if(i%20==0){
							LCD_DrawLine(10,i+20,120,1);
				}
			}
		}
		
	if(secim == 0) {
		//Sinus
		for(i = 0;i<280;i++){			
		if(value*cos(3.14*frekans*i)+70>=10 && value*cos(3.14*frekans*i)+70<=130){
			LCD_DrawLine(10*value*cos(3.14*frekans*i)+70,x,3,1);
		}
		x--;
	}
		
	if(mazgal == 0) {
		sprintf(buffer, "Mazgal: Kapali");
	LCD_DisplayStringLine(LCD_LINE_6, (uint8_t *)buffer);
	} else {
		sprintf(buffer, "Mazgal: Acik");
	LCD_DisplayStringLine(LCD_LINE_6, (uint8_t *)buffer);
	}
	sprintf(buffer, "Dalga Sekli: Sinus");
	LCD_DisplayStringLine(LCD_LINE_7, (uint8_t *)buffer);
	sprintf(buffer, "Gerilim: %2.3f V", value);
	LCD_DisplayStringLine(LCD_LINE_9, (uint8_t *)buffer);
	sprintf(buffer, "Frekans: %d HZ", frekans);
	LCD_DisplayStringLine(LCD_LINE_8, (uint8_t *)buffer);
	} else if (secim == 1) {
		//Kare Dalga
			squf = 0;
		for(i = 0;i<280;i++){
		if(value*cos(3.14*frekans*i)+70>=10 && value*cos(3.14*frekans*i)+70<=130){
			if(squf < frekans/2 && squf >= 0){
								LCD_DrawLine(10*value+70,x,1,1);
							squf++;
			} else if(squf > frekans/2 && squf < frekans) {
				LCD_DrawLine(70-10*value,x,1,1);
							squf++;
			} else if(squf == 0) {
								LCD_DrawLine(70-10*value,x,2*10*value,LCD_DIR_VERTICAL);
				squf++;
			} else if (squf == frekans) {
										LCD_DrawLine(70-10*value,x,2*10*value,LCD_DIR_VERTICAL);
				squf = 0;
			} else if (squf == frekans / 2) {
						LCD_DrawLine(70-10*value,x,2*10*value,LCD_DIR_VERTICAL);
				squf++;
			}
		}
		x--;
	}
			if(mazgal == 0) {
		sprintf(buffer, "Mazgal: Kapali");
	LCD_DisplayStringLine(LCD_LINE_6, (uint8_t *)buffer);
	} else {
		sprintf(buffer, "Mazgal: Acik");
	LCD_DisplayStringLine(LCD_LINE_6, (uint8_t *)buffer);
	}
	sprintf(buffer, "Dalga Sekli: Kare");
	LCD_DisplayStringLine(LCD_LINE_7, (uint8_t *)buffer);
	sprintf(buffer, "Gerilim: %2.3f V", value);
	LCD_DisplayStringLine(LCD_LINE_9, (uint8_t *)buffer);
	sprintf(buffer, "Frekans: %d HZ", sqfrekans);
	LCD_DisplayStringLine(LCD_LINE_8, (uint8_t *)buffer);
	}
	
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
