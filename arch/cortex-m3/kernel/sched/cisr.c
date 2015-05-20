
extern void kernel_init(void);
extern void _start(void);

void Reset(void)
{
	kernel_init();
	_start();
}

void Default_Handler(void)
{
	while(1);
}

#define _handler __attribute__ ((weak, alias("Default_Handler")))

void _handler NMI_Handler()
{
	while (1);
}

void _handler HardFault_Handler()
{
	while (1);
}

void _handler MemManage_Handler();
void _handler BusFault_Handler()
{
	while(1);
}

void _handler UsageFault_Handler()
{
	while(1);
}

void _handler SVC_Handler();
void _handler PendSV_Handler();
void _handler DebugMon_Handler()
{
	while  (1);
}

void _handler SysTick_Handler();

