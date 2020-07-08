#ifndef _NSC_MAIN_H
#define _NSC_MAIN_H

#include "stm32l1xx.h"
#include "nc_defines.h"

#define GPIO_AF2 0x2

volatile uint8_t DEBUG_VAR = 0;       /* Declare global variables(outside main) "volatile" to force compiler to generate
                                         code that rereads variables stored in memory & not just in registers. */
volatile uint8_t BACKGROUND = 0;
volatile uint8_t SYSTICK = 0;
volatile uint8_t TIM4_VAR = 0;
volatile uint32_t msTicks = 0;        /* Variable to store millisecond ticks */
#endif

void init_TIM4(){
	  /* Configure channel 1 output of timer 4 used as trigger signal to ADC. Square wave output with frequency 1Hz and 50% duty cycle. */
	  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; /* Enable TIM4 clock */
	  /* Output Compare PWM Mode. Timer 4 channel 1 output 1Hz. page 395, 402-403 rm0038.
	     1) Select counter clock(internal, external, and prescaler. Default: SMS=000 TIMx_SMCR register.  */
		TIM4->PSC = 2097000/1000 - 1;     /* MSI 2.097MHz is default internal clock. Timer 4 counter clock 1KHz, Prescaler value */
		/* 2) Write desired data in TIMx_ARR and TIMx_CCRx registers. */
	  TIM4->ARR = 1000-1;               /* Auto-reload value. Timer CNT count from zero to ARR value. */
    TIM4->CCR1 = 499;                 /* CCR1 selected from 4 channels CCRx by assigning value. Output compare register for channel 1.
	                                       Varies duty cycle if CCR1 != ARR, 499/999 is 50% duty cycle. */
	  /* 3) Set the CCxIE and/or CCxDE bits if an interrupt and/or a DMA request is to be generated.
    TIM4->DIER |= TIM_DIER_CC1IE; TIM4->DIER |= TIM_DIER_CC1DE; TIM4->DIER |= (1UL << 6);	TIM4->SR |= (1UL << 0);	*/
	  /* 4) Select output mode, PWM. */
	  TIM4->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; /* OC1M = 110 for PWM output mode 1 on channel 1 */
		TIM4->CCMR1	|= TIM_CCMR1_OC1PE;   /* Enable preload for channel 1 */
		TIM4->CR1 |= TIM_CR1_ARPE;        /* Auto-reload preload enable in ARR */
	  TIM4->CR1 &= ~(TIM_CR1_UDIS);     /* Update UEV Enable. Optional */
	  /* 5) Enable Output Enable Circuit, TIMx->CCER CC1E */
  	TIM4->CCER |= TIM_CCER_CC1E;      /* Capture/Compare 1 output enable */
	  TIM4->EGR |= TIM_EGR_UG;          /* Update Generation, re-initialize timer counter */
		/* 6) Enable the counter by setting the CEN bit in the TIMx_CR1 register. */
	  TIM4->CR1 |= TIM_CR1_CEN;         /* Enable timer 4 counter */
}

void SysTick_Handler(void){           /* SysTick interrupt Handler. */
	  BACKGROUND = 0;
	  TIM4_VAR = 0;
    SYSTICK = 1;
    msTicks++;
}

void GPIO_Pin_Init(){
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;  /* Enable GPIOB clock */
		/* MODER6[1:0] involving bits 12 & 13. 0b0010(0x02): Alternate function mode */
		GPIOB->MODER &= ~(0x03 << 12);			/* Clear mode bits */
		GPIOB->MODER |= 0x02 << 12;					/* Set pin PB.6 as alternate function */
    /* PB.6 selects AFRL6[3:0] from AFRL register & involves bits 24 to 27 rm0038. From stm32l152rb datasheet page 44 table 9,
		we choose AFIO2(PB6 alt func: TIM4_CH1) or 0010(0x02): AF2 alternate function. We borrow from libopencm3 define for AF2 */
		GPIOB->AFR[0] &= ~0xF << (24);			/* Clear bits 24-27 */
		GPIOB->AFR[0] |= GPIO_AF2 << (24);	/* Set 0x02 mask alternate function. For TIM3 alternate function, AFRLy bit y=6, AF2 is
		                                       PC.6. Refer to page 44 of stm32l152rb datasheet and page 176 of rm0038. */
		
		GPIOB->OTYPER &= ~(0x1<<6);					/* Set PB.6 pin as push-pull output type */	
		
		/* Set IO output speed */
		GPIOB->OSPEEDR &= ~(0x03<<(2*6));
		GPIOB->OSPEEDR |= 0x03<<(2*6);
	
		/* Set IO no pull-up pull-down */
		GPIOB->PUPDR |= ~(0x00<<(2*6));	
}

int main(void){
    uint8_t returnCode;

    returnCode = SysTick_Config(SystemCoreClock / 100000);
	
    if(returnCode != 0){
    /* Error Handling */
    }
		
		GPIO_Pin_Init();
		init_TIM4();

    while(1){
        BACKGROUND = 1;
        SYSTICK = 0;
        TIM4_VAR = 0;
        if((TIM4->SR & TIM_SR_CC1IF) != TIM_SR_CC1IF){    /* Check update event flag */
            TIM4_VAR = 1;
            BACKGROUND = 0;
            SYSTICK = 0;
        }
    }
}
