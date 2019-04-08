/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    MKL25Z128xxx4_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
/* TODO: insert other include files here. */

#include "time.h"
#include "low_power_timer.h"
#include "HW_interrupt.h"
#include "adc.h"
#include "dac.h"
#include "pwm.h"
#include "ESP8266_01.h"
#include "MLX90614.h"

/* TODO: insert other definitions and declarations here. */

// ADC
uint16_t dataADC = 0;

/*
 * @brief   Application entry point.
 */
int main(void)
{
  	/* Init board hardware. */
	BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    SIM->SCGC5 |= (1 << 10); // Clock on PORT B
    SIM->SCGC5 |= (1 << 11); // Clock on PORT C
    SIM->SCGC5 |= (1 << 12); // Clock on PORT D
    SIM->SCGC5 |= (1 << 13); // Clock on PORT E

    // HW interrupt
    HW_interrupt_init();

    // LPTMR
    LPTMR0_init();

    // PIT
    pit_init();

    // ADC
    adc_init();

    // DAC
    dac_init();

	// PWM
	pwm_init();

	// UART --- ESP8266 WiFi transceiver
	ESP8266_init();

	// I2C --- MLX90614 IR thermometer
	MLX90614_init(57600);

	char dataIn[8] = "alessio\r";
	float ambientTemp = 0;
	float objectTemp = 0;
    /* Force the counter to be placed into memory. */
    volatile static int i = 0;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1)
    {
    	if (i==0) // test ESP 8266
    	{
    		ESP8266_queryGetCipStatus(dataOutFromESP);

			ESP8266_setWifiMode(dataOutFromESP, 2);

			ESP8266_queryWifiMode(dataOutFromESP);

			ESP8266_setCipMux(dataOutFromESP, 1);

			ESP8266_queryCipMuxSet(dataOutFromESP);


			ESP8266_setCipServer(dataOutFromESP, 1);	// before send, connect to wifi net and browse ESP IP (192.168.4.1)

			ESP8266_cipSend(dataOutFromESP,"0\r", "7\r");

			ESP8266_sendData(dataIn);
    	}

    	if (task1Flag == 1) // test ADC and DAC
    	{
    		task1Flag = 0;

    		dataADC = adc_read();
    		dacOut(dataADC); // default dac out on PTE30

    		ambientTemp = MLX90614_getTemp(MLX90614_SCALE_CELSIUS, MLX90614_REGISTER_LINEARIZED_TAMBIENT);
    	}

    	if (task2Flag == 1) // test MLX90614
    	{
    		task2Flag = 0;
    		objectTemp = MLX90614_getTemp(MLX90614_SCALE_CELSIUS, MLX90614_REGISTER_LINEARIZED_TOBJECT1);
    	}

    	i++ ;
    }
    return 0 ;
}
