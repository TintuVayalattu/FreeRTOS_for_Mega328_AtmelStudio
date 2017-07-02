/**
 * \file
 *
 * \brief ADC Basic driver implementation.
 *
 *
 * Copyright (C) 2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 *
 */

#include <adc_basic.h>

adc_irq_cb_t ADC_0_cb = NULL;

/**
 * \brief Initialize ADC interface
 */
int8_t ADC_0_init()
{

	/* Enable clock to write ADC registers */
	PRR &= ~(1 << PRADC);

	ADMUX = (0x00 << REFS0) /* AREF, Internal Vref turned off */
	        | (0 << ADLAR)  /* Left Adjust Result: disabled */
	        | (0x00 << MUX0) /* ADC Single Ended Input pin 0 */;

	ADCSRA = (1 << ADEN)    /* ADC: enabled */
	         | (0 << ADATE) /* Auto Trigger: disabled */
	         | (0 << ADIE)  /* ADC Interrupt: disabled */
	         | (0x01 << ADPS0) /* 2 */;

	ADCSRB = (0x00 << ADTS0) /* Free Running mode */
	         | (0 << ACME) /* Analog Comparator Multiplexer: disabled */;

	DIDR0 = (0 << ADC0D)   /* Digital Input: enabled */
	        | (0 << ADC1D) /* Digital Input: enabled */
	        | (0 << ADC2D) /* Digital Input: enabled */
	        | (0 << ADC3D) /* Digital Input: enabled */
	        | (0 << ADC4D) /* Digital Input: enabled */
	        | (0 << ADC5D) /* Digital Input: enabled */;

	return 0;
}

void ADC_0_enable()
{
	ADCSRA |= (1 << ADEN);
}

void ADC_0_disable()
{
	ADCSRA &= ~(1 << ADEN);
}

void ADC_0_start_conversion(adc_0_channel_t channel)
{
	ADMUX &= ~0x0f;
	ADMUX |= channel;
	ADCSRA |= (1 << ADSC);
}

bool ADC_0_is_conversion_done()
{
	return ((ADCSRA & (1 << ADIF)));
}

adc_result_t ADC_0_get_conversion_result(void)
{
	return (ADCL | ADCH << 8);
}

adc_result_t ADC_0_get_conversion(adc_0_channel_t channel)
{
	adc_result_t res;

	ADC_0_start_conversion(channel);
	while (!ADC_0_is_conversion_done())
		;
	res = ADC_0_get_conversion_result();
	ADCSRA |= (1 << ADIF);
	return res;
}

uint8_t ADC_0_get_resolution()
{
	return 10;
}

void ADC_0_register_callback(adc_irq_cb_t f)
{
	ADC_0_cb = f;
}

ISR(ADC_vect)
{

	if (ADC_0_cb != NULL) {
		ADC_0_cb();
	}
}