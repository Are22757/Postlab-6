
#ifndef UART_H_
#define UART_H_

void UART_init(void);						// función para iniciar el USART AVR asíncrono, 8 bits, 9600 baudios,
unsigned char UART_read(void);				// función para la recepción de caracteres
void UART_write(unsigned char);				// función para la transmisión de caracteres
void UART_msg(char*);						// función para la transmisión de cadenas de caracteres (ES EL PUNTERO)

void UART_init(void)
{
	//Paso 1: Establecer pines de entrada y salida
	
	//PD0 = PDX
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	
	//Paso 2: Modificar el registro A registro UCSRA
	UCSR0A = 0;
	
	//Paso 3: Modificar el registro B registro UCSR0B, habilitamos rx y tx
	UCSR0B = 0;
	
	//Habilitar interrupción de recepción
	UCSR0B |=(1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	
	//Paso 4: Configurar UCSR0C > asincrono, pariedad: none, 1 bit Stop, Data bits 8/bits,
	UCSR0C =0;
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	
	//Paso 5: Configurar velocidad de Baudrate: 9600
	UBRR0 =103;
	
}

unsigned char UART_read(void)
{
	if(UCSR0A&(1<<7))						//si el bit7 del registro UCSR0A se ha puesto a 1
	return UDR0;						//devuelve el dato almacenado en el registro UDR0
	else
	return 0;
}

void UART_write(unsigned char caracter)
{
	while(!(UCSR0A&(1<<5)));				// mientras el registro UDR0 esta lleno espera
	UDR0 = caracter;						//cuando el el registro UDR0 esta vacio se envia el caracter
}

void UART_escribir(char* cadena)			//cadena de caracteres de tipo char
{
	while(*cadena !=0x00)
	{
		UART_write(*cadena);				//transmite los caracteres de cadena
		cadena++;							//incrementa la ubicación de los caracteres en cadena
		//para enviar el siguiente caracter de cadena
	}
}

#endif /* UART_H_ */