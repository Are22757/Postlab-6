//Lisbeth Ivelisse Arévalo Girón

#define F_CPU 16000000


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "UART.h"

const uint16_t poten_delay = 5;

void initADC(void);
volatile char recibeCHAR = 0;
uint8_t		adc_poten = 0;
uint8_t dato=0;
int ACT2=0, ACT=0;
//Lista
uint8_t listapoten[10]={0b00000000,0b00000001,0b00000010, 0b00000011, 0b00000100, 0b00000101,0b00000110,0b00000111,0b00001000,0b00001001};



int main(void)
{
	//cli();
	
	// Seleccionar el puerto B como salida
	DDRB |= 0xFF;
	
	
	// Seleccionar PC0 y PC1 como salidas
	DDRC |= 0b00000011;
	
	
	//Configuración de interrupciones
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT0)| (1 << PCINT1);
	
	
	//Se inicia la función ADC
	initADC();
	
	
	//Se inicia la función UART
	UART_init();
	

	
	
    sei();
	
	while (1)
	{
		ADCSRA |= (1<<ADSC);
		
		UART_escribir("\n\r*************************Bienvenido al sistema de seleccion*************************\n\r");
		UART_escribir("Por favor seleccione una opcion\n\r");
		UART_escribir("Op.1: Leer potenciometro\n\r");
		UART_escribir("Op.2: Enviar Ascci\n\r");
		UART_escribir("Op.3: Salir\n\r");
			
		dato = UART_read();			// Se leerán los datos de RX
		
		if(dato != 0)
		{
			switch(dato)
			{
				case '1':
			    UART_escribir("Leyendo el potenciometro...\n\r");
				
				// Dividir el carácter recibido en dos partes
				uint8_t lower_bits = dato & 0b00111111; // Los 6 bits menos significativos
				uint8_t upper_bits = (dato >> 6) & 0b00000011; 

				// Se muestran los 6 bits menos significativos en PORTB
				PORTB = lower_bits;

				// Mostrar los 2 bits en los pines PC0 y PC1 de PORTC
				PORTC = (PORTD & ~0b00000011) | (upper_bits << 6);
				PORTB = listapoten[adc_poten];
				main();
				break;
				
				case '2':
				UART_escribir("Enviando Ascii...\n\r");
		
				UART_escribir("Ingrese el ascii que desea mostrar en los LEDS\n\r");
				ACT2 = 1;    // Se activa para mostrar el valor en LEDS
				main();
				break;
				
				default:
				UART_escribir("Saliendo de la ejecucion\n\r");
				main();
				break;
			}
		}
	}
}



void initADC(void)
{
	ADMUX = 0;
	//referencia AVCC = 5V
	ADMUX |= (1<<REFS0);
	
	//Justificación a la izquierda
	ADMUX |= (1<<ADLAR);
	
	//Seleccionar ADC7
	ADMUX |= (1<<MUX2) | (1<<MUX1) | (1<<MUX0);
	
	ADCSRA=0;
	
	//Habilitando la interrupción del ADC
	ADCSRA |= (1 << ADIE);
	
	//Habilitamos prescaler de 16M/128 Fadc = 125kHz
	ADCSRA |= (1 << ADPS2)| (1 << ADPS1)|(1 << ADPS0);
	
	//Habilitando el ADC
	ADCSRA |= (1 << ADEN);
}

ISR(ADC_vect)
{
	uint8_t adc_value = ADCH;
	adc_poten = adc_value 
	
	adc_poten = ADCH;
	ADCSRA |= (1<<ADIF);  //Apagar la bandera con un 1
}


ISR(USART_RX_vect)
{
	recibeCHAR = UDR0; // Almacena el carácter recibido
	
	if (ACT2 == 1){    //Se se eligio enviar un caracter
		// Dividir el carácter recibido en dos partes
		uint8_t lower_bits = recibeCHAR & 0b00111111; // Los 6 bits menos significativos
		uint8_t upper_bits = (recibeCHAR >> 6) & 0b11; // Los 2 bits más significativos

		// Mostrar los 6 bits menos significativos en PORTB
		PORTB = lower_bits;

		// Mostrar los 2 bits más significativos en los pines PD6 y PD7 de PORTD
		PORTD = (PORTD & ~0b11000000) | (upper_bits << 6);
		ACT2 = 0;   //Salir de este if
		ACT = 0;  //Entrar al menu principal
	}