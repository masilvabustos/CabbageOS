
#ifndef __stm32f100xx_h_INCLUDED__
#define __stm32f100xx_h_INCLUDED__

#define GPIO_MODE_INPUT 	00 << 0
#define GPIO_MODE_OUTPUT_10MHz 	01 << 0
#define GPIO_MODE_OUTPUT_2MHz 	02 << 0
#define GPIO_MODE_OUTPUT_50MHz	03 << 0

#define GPIO_CONF_GP_PUSH_PULL	00 << 2
#define GPIO_CONF_GP_OPEN_DRAIN	01 << 2
#define GPIO_CONF_AF_PUSH_PULL	02 << 2
#define GPIO_CONF_AF_OPEN_DRAIN	03 << 2


extern uint32_t	GPIOA_CRL;
extern uint32_t	GPIOA_CRH;
extern uint32_t	GPIOA_IDR;
extern uint32_t	GPIOA_ODR;
extern uint32_t	GPIOA_BSSR;
extern uint32_t	GPIOA_BRR;
extern uint32_t	GPIOA_LCKR;

extern uint32_t	GPIOC_CRL;
extern uint32_t	GPIOC_CRH;
extern uint32_t	GPIOC_IDR;
extern uint32_t	GPIOC_ODR;
extern uint32_t	GPIOC_BSSR;
extern uint32_t	GPIOC_BRR;
extern uint32_t	GPIOC_LCKR;

extern uint32_t RCC_CR		;
extern uint32_t RCC_CFGR	;
extern uint32_t RCC_CIR		;
extern uint32_t RCC_APB2RSTR	;
extern uint32_t RCC_APB1RSTR 	;
extern uint32_t RCC_AHBENR	;
extern uint32_t RCC_APB2ENR	;
extern uint32_t RCC_APB1ENR	;
extern uint32_t RCC_BDCR	;
extern uint32_t RCC_CSR		;
extern uint32_t RCC_CFGR2	;

 
extern uint32_t IOPCEN		;

#endif
