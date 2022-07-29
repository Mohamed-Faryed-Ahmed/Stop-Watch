/*
 ============================================================================
 Name        : interface__1.c
 Author      : Mohamed Faryed
 Date        : 9/1/2022
 Copyright   : eng.mohamedfaryed@gmail.com
 Description : clock
 ============================================================================
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#define F_CPU 1000000
//internal  1 MH
#define CKSEL0  1
#define CKSEL1  0
#define CKSEL2  0
#define CKSEL3  0
/////////////////
// fast clock
#define SUT0 1
#define SUT1 0
/////////////////
void inti_tim1(void)
{
	//top  value stored in the  OCR1A or ICR1
    TCNT1  = 0;                                   //Set Timer initial value to 0
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);             //compare mode
	OCR1A  = 15624;                               //set value
	TCCR1B = (1<<WGM12) | (1<< CS10) | (1<<CS11); //enable and clkI/O/1024
	TIMSK=(1<< OCIE1A);                           //enable compare interrupt for timer 1
}

void inti_int0(void)
{
	DDRD &=~(1<<2);      //input D2 for INT0
	PORTD |=(1<<2);      //pull up D2
	MCUCR |=(1<<ISC01);  //falling edge
	MCUCR &=~(1<<ISC00); //falling edge
	GICR|=(1<<INT0);     //enable interrupt0
}
void inti_int1(void)
{
	DDRD &=~(1<<3);      //input D3 for INT1
	MCUCR |=(1<<ISC10);  //rising edge
	MCUCR &=~(1<<ISC11); //rising edge
	GICR|=(1<<INT1);     //enable interrupt1
}
void inti_int2(void)
{
	MCUCR |=(1<<ISC2);   //rising edge
	GICR|=(1<<INT2);     //enable interrupt2
}
///////////////////////////////////////////////////
static uint8_t s=0,m=0,h=0,flage=0;
int main(void)
{
	DDRC |=0x0f;               //output C0:3
	DDRA |=0x3f;               //output A0:5
	PORTC &=0xf0;              //clear first 4bits in C
	PORTA &=0xc0;              //clear first 6bits in A
	DDRB &=~(1<<2);            //input B2 for INT2
	SFIOR &=~(1<<2);           //pull enable

	inti_tim1();
	inti_int0();
	inti_int1();
	inti_int2();
	SREG |=(1<<7);
	//============================================================//
		while(1)
		{
			PORTA =0x01;
			PORTC =(PORTC & 0XF0) | ((s%10) & 0x0f);
			_delay_ms(1);

			PORTA =0x02;
			PORTC =(PORTC & 0XF0) | (((s/10)%100) & 0x0f);
			_delay_ms(1);
			/////////////////////
			PORTA = 0x04;
			PORTC =(PORTC & 0XF0) | ((m%10) & 0x0f);
			_delay_ms(1);

			PORTA = 0x08;
			PORTC =(PORTC & 0XF0) | (((m/10)%100) & 0x0f);
			_delay_ms(1);
			/////////////////////
			PORTA = 0x10;
			PORTC =(PORTC & 0XF0) | ((h%10) & 0x0f);
			_delay_ms(1);

			PORTA = 0x20;
			PORTC =(PORTC & 0XF0) | (((h/10)%100) & 0x0f);
			_delay_ms(1);
	        //=========================================================//
			if(1==flage)
			{
				s++;
				if(60==s)
				{
					s=0;
					m++;
					if(60==m)
						{
							m=0;
							h++;
							if(6==h)
								{
									h=0;
								}
						}
				  }
			flage=0;
			}
	}
			//=========================================================//
		return 0;
	}

	/*
	 * //============================================================//
	while(1)
	{
		PORTA =0x01;
		PORTC =(PORTC & 0XF0) | (s1 & 0x0f);
		_delay_ms(1);

		PORTA =0x02;
		PORTC =(PORTC & 0XF0) | (s2 & 0x0f);
		_delay_ms(1);
		/////////////////////
		PORTA = 0x04;
		PORTC =(PORTC & 0XF0) | (m1 & 0x0f);
		_delay_ms(1);

		PORTA = 0x08;
		PORTC =(PORTC & 0XF0) | (m2 & 0x0f);
		_delay_ms(1);
		/////////////////////
		PORTA = 0x10;
		PORTC =(PORTC & 0XF0) | (h1 & 0x0f);
		_delay_ms(1);

		PORTA = 0x20;
		PORTC =(PORTC & 0XF0) | (h2 & 0x0f);
		_delay_ms(1);
        //=========================================================//
		if(1==flage)
		{
			s1++;
			if(10==s1)
			{
				s1=0;
				s2++;
				if(6==s2)
				{
					s2=0;
					m1++;
					if(10==m1)
					{
						m1=0;
						m2++;
						if(6==m2)
						{
							m2=0;
							h1++;
							if(10==h1)
							{
								h1=0;
								h2++;
								if(13==h2)
								{
									h2=0;
									h1=0;
								}
							}
						}
					}

				}
			}
			flage=0;
		}

     }

		//=========================================================//
	return 0;
}
*/
ISR(TIMER1_COMPA_vect)
{
	flage=1;
}

ISR(INT0_vect) //reset
{
	s=0,m=0,h=0;
	SFIOR |=(1<<PSR10);  //clear timer

}
ISR(INT1_vect) //stop
{
	TCCR1B &=~(1<< CS10) & ~(1<< CS11) & ~(1<<CS12);
}
ISR(INT2_vect) //resume
{
	TCCR1B = (1<<WGM12) | (1<< CS10) | (1<<CS11); //enable and clkI/O/1024
}


