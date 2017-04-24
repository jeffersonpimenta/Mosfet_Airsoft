#define bit_set(p,m) ((p) |= _BV(m))
#define bit_get(p,m) ((p) & _BV(m))
#define bit_clear(p,m) ((p) &= ~_BV(m))

void adc_init(){
	bit_set(ADCSRA,ADEN);//habilita adc
	bit_set(ADCSRA,ADPS2);
	bit_set(ADCSRA,ADPS0);//seta velocidade
	
	
	//ADMUX=0x05;//escolhe ad
	
	bit_set(ADMUX,REFS0);
	bit_clear(ADMUX,REFS1);//referencia de tensão


	bit_clear(ADMUX,MUX0);
	bit_set(ADMUX,MUX1);
	bit_set(ADMUX,MUX2);
	bit_clear(ADMUX,MUX3);
}


uint8_t adc_read(){
	
	bit_set(ADCSRA,ADSC);
	
	while(bit_get(ADCSRA,ADSC));
	
	return ADCW;
}