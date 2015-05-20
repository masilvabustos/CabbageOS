
#include <stm32vldiscovery.h>

void kernel_init();

int main()
{

	IOPCEN = 1;
	GPIOC_CRH = 0x44444422;
	
	int d;
	while(1){
		d = 50000;
		while(d--);
		GPIOC_ODR ^= 0x0100;
	}
		

	return 0;
}
