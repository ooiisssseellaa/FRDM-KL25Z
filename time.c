/*
 * time.c
 *
 *  Created on: 15 mar 2019
 *      Author: Alessio
 */
#include "time.h"


uint32_t ticks = 0;

uint16_t arrival_task0_counter = 0;
uint16_t revival_task0_counter = 0;
uint32_t burstTime_task0 = 0;

uint16_t arrival_task1_counter = 0;
uint16_t revival_task1_counter = 0;
uint32_t burstTime_task1 = 0;

uint16_t arrival_task2_counter = 0;
uint16_t revival_task2_counter = 0;
uint32_t burstTime_task2 = 0;

uint16_t arrival_task3_counter = 0;
uint16_t revival_task3_counter = 0;
uint32_t burstTime_task3 = 0;

uint16_t arrival_task4_counter = 0;
uint16_t revival_task4_counter = 0;
uint32_t burstTime_task4 = 0;

uint16_t arrival_task5_counter = 0;
uint16_t revival_task5_counter = 0;
uint32_t burstTime_task5 = 0;

// BARE METAL O.S. IS A FIFO STATIC - NON-PREEMPTIVE SCHEDULING

uint32_t currTicks = 0;

uint32_t arrival_task0_time = 1000; // threshold of counter in ms
uint32_t revival_task0_time = 1000;

uint32_t arrival_task1_time = 3000;
uint32_t revival_task1_time = 5000;

uint32_t arrival_task2_time = 5000;
uint32_t revival_task2_time = 7000;

uint32_t arrival_task3_time = 100;
uint32_t revival_task3_time = 40000;

uint32_t arrival_task4_time = 160;
uint32_t revival_task4_time = 4000;

uint32_t arrival_task5_time = 2000;
uint32_t revival_task5_time = 4000;

uint8_t arrival_task0_flag = 0;
uint8_t disable_task0_flag = 0;

uint8_t arrival_task1_flag = 0;
uint8_t disable_task1_flag = 0;

uint8_t arrival_task2_flag = 0;
uint8_t disable_task2_flag = 0;

uint8_t arrival_task3_flag = 0;
uint8_t disable_task3_flag = 0;

uint8_t arrival_task4_flag = 0;
uint8_t disable_task4_flag = 0;

uint8_t arrival_task5_flag = 0;
uint8_t disable_task5_flag = 0;


void task_scheduler(void)
{
	arrival_task0_counter++;
	if(arrival_task0_counter == arrival_task0_time)
	{
		arrival_task0_counter = 0;
		arrival_task0_flag = 1;
	}

	revival_task0_counter++;
	if(revival_task0_counter == revival_task0_time && disable_task0_flag == 1)
	{
		revival_task0_counter = 0;
		arrival_task0_counter = 0;
		disable_task0_flag = 0;
	}
	else if(revival_task0_counter == revival_task0_time && disable_task0_flag == 0)
	{
		revival_task0_counter = 0;
	}


	arrival_task1_counter++;
	if(arrival_task1_counter == arrival_task1_time)
	{
		arrival_task1_counter = 0;
		arrival_task1_flag = 1;
	}

	revival_task1_counter++;
	if(revival_task1_counter == revival_task1_time && disable_task1_flag == 1)
	{
		revival_task1_counter = 0;
		arrival_task1_counter = 0;
		disable_task1_flag = 0;
	}
	else if(revival_task1_counter == revival_task1_time && disable_task1_flag == 0)
	{
		revival_task1_counter = 0;
	}


	arrival_task2_counter++;
	if(arrival_task2_counter == arrival_task2_time)
	{
		arrival_task2_counter = 0;
		arrival_task2_flag = 1;
	}

	revival_task2_counter++;
	if(revival_task2_counter == revival_task2_time && disable_task2_flag == 1)
	{
		revival_task2_counter = 0;
		arrival_task2_counter++;
		disable_task2_flag = 0;
	}
	else if(revival_task2_counter == revival_task2_time && disable_task2_flag == 0)
	{
		revival_task2_counter = 0;
	}


	arrival_task3_counter++;
	if(arrival_task3_counter == arrival_task3_time)
	{
		arrival_task3_counter = 0;
		arrival_task3_flag = 1;
	}

	revival_task3_counter++;
	if(revival_task3_counter == revival_task3_time && disable_task3_flag == 1)
	{
		revival_task3_counter = 0;
		arrival_task3_counter = 0;
		disable_task3_flag = 0;
	}
	else if(revival_task3_counter == revival_task3_time && disable_task3_flag == 0)
	{
		revival_task3_counter = 0;
	}


	arrival_task4_counter++;
	if(arrival_task4_counter == arrival_task4_time)
	{
		arrival_task4_counter = 0;
		arrival_task4_flag = 1;
	}

	revival_task4_counter++;
	if(revival_task4_counter == revival_task4_time && disable_task4_flag == 1)
	{
		revival_task4_counter = 0;
		arrival_task4_counter = 0;
		disable_task4_flag = 0;
	}
	else if(revival_task4_counter == revival_task4_time && disable_task4_flag == 0)
	{
		revival_task4_counter = 0;
	}


	arrival_task5_counter++;
	if(arrival_task5_counter == arrival_task5_time)
	{
		arrival_task5_counter = 0;
		arrival_task5_flag = 1;
	}

	revival_task5_counter++;
	if(revival_task5_counter == revival_task5_time && disable_task5_flag == 1)
	{
		revival_task5_counter = 0;
		arrival_task5_counter = 0;
		disable_task5_flag = 0;
	}
	else if(revival_task5_counter == revival_task5_time && disable_task5_flag == 0)
	{
		revival_task5_counter = 0;
	}
}

void PIT_IRQHandler(void)
{
	task_scheduler();

	ticks++;

	GPIOC->PTOR |= (1 << 7);

	PIT->CHANNEL->TFLG |= (1 << 0); // Clear interrupt flag
}

void delay(uint32_t mseconds) // Delay function, based on pit
{
    currTicks = ticks;

    while(((ticks - currTicks) < mseconds))
    {

    }
}

void pit_init(void)
{
   PORTB->PCR[19] |= (1 << 8); // PORTB pin 19 in alt1 mode, green led

   GPIOB->PDDR |= (1 << 19); // PORTB pin 19 set as output

   GPIOB->PDOR |= (1 << 19); // PORTB pin 19 default low

   PORTC->PCR[7] |= (1 << 8); // PORTB pin 19 in alt1 mode, green led

   GPIOC->PDDR |= (1 << 7); // PORTB pin 19 set as output

   GPIOC->PDOR |= (1 << 7); // PORTB pin 19 default low


   SIM->SCGC6 |= (1 << 23);  // enable clock for PIT

   PIT->MCR &= ~(1 << 1); // enable PIT module // Attention!! There are many error in reference manual regarding bit field.

   PIT->CHANNEL->LDVAL |= (25000000 / 1000) - 1; // set time period : desired timing * bus clock -1

   PIT->CHANNEL->TCTRL |= (1 << 1) | (1 << 0); // Timer interrupt enable, Timer enable

   NVIC->IP[5] |= (1 << 22); // preemptive priority 1, min value is 3, max and default is 0

   NVIC->ISER[0] |= (1 << 22);  // interrupt enable in NVIC
}
