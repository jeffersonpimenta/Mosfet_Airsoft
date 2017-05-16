/*
 * GatilhoMega328.c
 *
 * Created: 12/01/2017 22:06:59
 * Author : Jéfferson
 */ 

//#define F_CPU 9600000UL

#define bit_get(p,m) ((p) & _BV(m))
#define bit_set(p,m) ((p) |= _BV(m))
#define bit_clear(p,m) ((p) &= ~_BV(m))

#include "util/delay.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

#define Ptrigger DDRB
#define Trigger PINB
#define Pintrigger PB1//porta do gatilho

#define Omotor DDRB
#define Motor PORTB
#define Pinmotor PB0//porta do controle do motor

#define atraso 0.9

#define  debouncedelay 5

uint8_t tiros,timeon,firemode=0;//variáveis que deverão ficar na eeprom
uint8_t i,check=0;//seta variáveis

int main(void)
{

	bit_clear(Ptrigger,Pintrigger);//seta ddr do gatilho como entrada
	bit_set(Omotor,Pinmotor);//seta ddr do motor como saída
	bit_clear(Motor,Pinmotor);//garante que o motor ficará desligado
	barulho();
	_delay_ms(200);
	barulho();
	
	
while(1){
	check=0;
	do
	{
		check++;
		_delay_ms(4);
	} while (!bit_get(Trigger,Pintrigger) && (check<200));
	
	if (check<200)
	{
		tiros=eeprom_read_byte((uint8_t*)10);//lê a quantidade de tiros configuradas
		timeon=eeprom_read_byte((uint8_t*)11);//carrega o tempo de tiro da memória
		firemode=eeprom_read_byte((uint8_t*)12);//lê a quantidade de tiros configuradas		
		normal();//funcionamento normal
	}else
	{
		conf();//entra no modo de configuração
	}
}	
	
	
	

}


void normal(){
	uint8_t j=0,i=0;//declara contador
		
		if (firemode==1)
		{
			while(1){
						if (bit_get(Trigger,Pintrigger))
						{
							bit_clear(Motor,Pinmotor);
						}else{
							bit_set(Motor,Pinmotor);
						}
					}
		}//modo de espelhamento do estado do gatilho
		
		
		if (firemode==2){//modo economico
			while(1){
				i=0;
				_delay_ms(1);
				while(!bit_get(Trigger,Pintrigger)){//detecta gatilho pressionado
					i++;
					_delay_ms(1);
					if (i>debouncedelay)
					{
						bit_set(Motor,Pinmotor);//liga motor
						
						if ((!bit_get(Trigger,Pintrigger))){
							for (i=0;i<tiros;i++)//quantidade de tiros menos o efetuado
							{
								
							if (i==0)
								{//primeiro tiro, de aceleração
									for (j=0;j<timeon;j++)
									{
										_delay_ms(1);
									}//tempo que cada ciclo leva pra se completar									
									}else{
									
									if (bit_get(Trigger,Pintrigger)) break;//para se o usuário parar os tiros	
										
									for (j=0;j<(timeon*atraso);j++)
									{
										_delay_ms(1);
									}//tempo que cada ciclo leva pra se completar									
									}//segundo tiro, motor acelerado

								}
							bit_clear(Motor,Pinmotor);//desliga motor, pois os tiros configurados foram efetuados						
	
							do 
							{
								_delay_ms(1);
							} while (!bit_get(Trigger,Pintrigger));//espera o gatilho ser liberado
							
							
						}
					}else bit_clear(Motor,Pinmotor);//desliga motor
				}	
			}
		}
		
		
		if (firemode==3)//modo rambo
		{
			while(1){
				i=0;				
				_delay_ms(1);
				while(!bit_get(Trigger,Pintrigger)){//detecta gatilho pressionado
					i++;
					_delay_ms(1);
					if (i>debouncedelay)
					{
						bit_set(Motor,Pinmotor);//liga motor
						
						if ((!bit_get(Trigger,Pintrigger))){
							for (i=0;i<tiros;i++)//quantidade de tiros efetuado
							{
								if (i==0)
								{//primeiro tiro, de aceleração
																	
									for (j=0;j<timeon;j++)
									{
										_delay_ms(1);
									}//tempo que cada ciclo leva pra se completar
									}else{
									for (j=0;j<(timeon*atraso);j++)
									{
										_delay_ms(1);
									}//tempo que cada ciclo leva pra se completar
								}//segundo tiro, motor acelerado
							}
							
							while(!bit_get(Trigger,Pintrigger)){
								for (j=0;j<(timeon*atraso);j++)
								{
									_delay_ms(1);
								}//tempo que cada ciclo leva pra se completar								
							}
							bit_clear(Motor,Pinmotor);//desliga motor
						
					}
				}else bit_clear(Motor,Pinmotor);//desliga motor
			}
		}	
	}
}



void conf(){
	uint8_t flag=1;
	
	
	firemode=0;//limpa firemode
	flag=1;//reseta flag

	do
	{
		barulho();
	} while (!bit_get(Trigger,Pintrigger));	//espera usuário parar de apertar pra entrar no menu

	
	do
	{
		i=0;//reseta variavel
		while(!bit_get(Trigger,Pintrigger)){
			i++;
			_delay_ms(100);
			if (i>30)//quebra o loop se o gatilho for pressionado
			{
				flag=0;//sinaliza final da função
				if (firemode>3) firemode=1;
				if (firemode==0) firemode=1;//evita treta
				eeprom_write_byte((uint8_t*) 12, firemode);//grava na memo
				break;
			}
		}//conta o tempo que o gatilho foi apertado

		if (i>2)//debounce
		{
			if (i>30)//maior que 4 segundos
			{
				for (i=0;i<firemode;i++)
				{
					barulho();
					_delay_ms(500);//indica o modo configurado
				}
				
				}else{
				firemode++;
			}
		}
	} while (flag);//loop de configuração do modo	


	do
	{
		barulho();
	} while (!bit_get(Trigger,Pintrigger));	//espera usuário parar de apertar pra sair do menu
	
	if (firemode!=1)
	{
		
		flag=1;//seta flag do loop

		
		do{
			i=0;
			while(!bit_get(Trigger,Pintrigger)){//detecta gatilho pressionado
				i++;
				_delay_ms(1);
				if (i>debouncedelay)
				{
					bit_set(Motor,Pinmotor);//liga motor
					do
					{
						i++;
						_delay_ms(1);
					} while (!bit_get(Trigger,Pintrigger));
					bit_clear(Motor,Pinmotor);//desliga motor
					flag=0;
				}
			}//dois tiros pra configurar o tempo de tiro
			
			}while(flag);
			
		
		flag=1;//seta flag do loop
		do 
		{
				i=0;
				while(!bit_get(Trigger,Pintrigger)){//detecta gatilho pressionado
					i++;
					_delay_ms(1);
					if (i>debouncedelay)
					{			
						bit_set(Motor,Pinmotor);//liga motor		
						do 
						{
							i++;
							_delay_ms(1);
						} while (!bit_get(Trigger,Pintrigger));
						bit_clear(Motor,Pinmotor);//desliga motor
					
						flag=0;//avisa que houve um pressionamento válido		
						eeprom_write_byte((uint8_t*) 11, i);//salva o delay na eeprom	
						timeon=i;//atualiza variável			
					}
				}
			
		} while (flag);//loop de configuração de tempo de disparo


		_delay_ms(1000);
		do
		{
			barulho();
		} while (!bit_get(Trigger,Pintrigger));//não sai da função até que o gatilho seja liberado
	
	flag=1;//reseta flag	
	tiros=0;
	
		do
		{
			i=0;//reseta variavel
			while(!bit_get(Trigger,Pintrigger)){
				i++;
				_delay_ms(100);			
				if (i>30)
				{
					if (tiros==0) tiros=3;//evita que nenhuma info inválida passe à frente
					flag=0;//sinaliza final da função
					eeprom_write_byte((uint8_t*) 10, tiros);//grava na memo
					break;
				}//se for segurado o gatilho, quebra o loop
			}//conta o tempo que o gatilho foi apertado
		
			if (i>2)//debounce
			{
				if (i>30)//maior que 4 segundos
				{
					for (i=0;i<tiros;i++)
					{
						barulho();
						_delay_ms(500);//indica a quantidade de tiros configuradas
					}
				
				}else tiros++;
			}
		
		
		} while (flag);//loop de configuração de quantidade de disparos

	
		do
		{
			barulho();
		} while (!bit_get(Trigger,Pintrigger));//não sai da função até que o gatilho seja liberado	
	}
}


void barulho(){
				for (uint8_t k=0;k<50;k++)
				{
					bit_set(Motor,Pinmotor);
					_delay_us(100);
					bit_clear(Motor,Pinmotor);
					_delay_ms(1);
				}//faz barulho
}