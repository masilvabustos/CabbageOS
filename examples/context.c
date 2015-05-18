
#include <stm32f100xx.h>

#include <ucontext.h>

ucontext_t uc;

void delay(int t)
{
	while(t--);
}

int stack[100];

void toggle()
{
	GPIOC_ODR ^= 0x0100;
}

int f()
{
	delay(500000);
	toggle();
	setcontext(&uc);
}

void main()
{
	RCC_APB2ENR |= 0x10;

	GPIOC_CRH = 0x44444422;

	getcontext(&uc);
	delay(500000);
	toggle();
	f();
}

