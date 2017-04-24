/*
 * serial.h
 *
 * Created: 13/01/2017 20:11:10
 *  Author: Jéfferson
 */ 


#define USART_BAUDRATE 2400
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

void uart_init(){
	
 UCSR0B |= (1<<RXEN0)  | (1<<TXEN0);
 UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
 UBRR0H  = (BAUD_PRESCALE >> 8);
 UBRR0L  = BAUD_PRESCALE;

}
	


void uart_tx(unsigned char data){
	while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){}
	UDR0 = data;  
}

unsigned char uart_rx(){
	while( ( UCSR0A & ( 1 << RXC0 ) ) == 0 ){}
	return UDR0;

}