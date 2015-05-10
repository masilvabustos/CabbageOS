
#include "gpio.h"

#define _area(name)  __attribute__ ((section("." #name "_registers"),align(4)))


#ifdef STM32F100xx_LD

_area(gpio) union gpio_pcr_u	GPIOA_CRL;
_area(gpio) union gpio_pcr_u	GPIOA_CRH;
_area(gpio) union gpio_idr_u	GPIOA_IDR;
_area(gpio) union gpio_odr_u	GPIOA_ODR;
_area(gpio) union gpio_bssr_u	GPIOA_BSSR;
_area(gpio) union gpio_brr_u	GPIOA_BRR;
_area(gpio) union gpio_lckr_u	GPIOA_LCKR;

#endif


