/*
 * XIOT_Task.c
 *
 * Created: 6/28/2018 4:34:09 PM
 *  Author: Nourhan
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#ifndef F_CPU 
#define F_CPU 8000000UL 
#endif
#define FOSC 1843200
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
volatile int led_state=0; 
//led_state is a counter which indicates the led state --> the led initially is off ,so even numbers indicates that the led is OFF ,else is ON 


/*As The Processor Reads only digital values ,we have to use ADC to convert Analog Temperature Sensor reading to digital values */

/*ADC Initialization*/
	void ADC_Init(char channel)
 {
	 ADMUX   = channel;									//Select Channel 
	 ADCSRA |= (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);		//Enable ADC ,and determine Division Factor to 8 [Prescaler] 
 }


 /*Read Temperature */
	int  ADC_Read()
 { int adc_value =0;
	 ADCSRA |= (1<<ADSC);								// Start conversion. 
	 while (ADCSRA & (1<<ADSC));						// Wait for conversion to complete. 
	 adc_value = ADCW;									//Read value of sensor and store it in adc_value variable 
	 adc_value=adc_value/2;								//divide adc_value by 2 to get real value 
	 return adc_value;
 }
 
 
/**	Function for displaying Text on Serial Monitor [Virtual Terminal]**/
	void serial_avr(char *str){
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);	
	UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);
	UBRR0=51;		//Setting baud rate to 9600
	for	(unsigned int i=0 ;str[i]!=0;i++){
		UDR0=str[i];
		while ( !( UCSR0A &(1<<UDRE0))) ;
	}
}


//ISR: interrupt service routine to turn ON/OFF the led (Toggle)
ISR(INT1_vect){
	
	 PORTD =PORTD^(1<<PD6); //Toggle the led
	 led_state++; /*initially led_state equaled 0 ,after first press --> led_state=1 which means it is ON now (as it is odd Numebr )*/
	serial_avr(" The switch is pressed, ");
	
	//Check The Value of led_state if it's even or odd 
	if(led_state%2==0)    //led_state is even --> the led is OFF 
		serial_avr(" The Led is OFF .");
		else 
		serial_avr(" The Led is ON .");
}


/* Interrupt initialization [INT1]*/
void INT1_Init(void){
	SREG &=~(1<<7); //disable interrupt in Status Register 
	EICRA|=(1<ISC10); //any logical change in INT1 generates an interrupt request
	EIMSK|=(1<<INT1); //enable external interrupt INT1
	SREG |=(1<<7); //enable interrupt in Status Register 
	sei(); //waiting for interrupt 
	
}


int main(void)
{	
	char temp_string[3], text[25]="  The Temperature is "; 
	//temp_string : to keep temperature in a string type ,to be able to send the temperature to serial_avr function (as the parameter is String )
	
	int ch=0x00; //use channel 0 ADC 
	
	/* PINS Configuration */
	DDRB=0xff;
	DDRD &=~(1<<DDD3); //PD3 is input as this is the INT1 pin
	PORTD |=(1<<PD3);//enable pull up for PD3
	DDRC &=~(1<<DDC0);  //input pin for temp sensor
	PORTC|=(1<<PORTC0);	//enable pull up
	DDRD|=(1<<PD6)|(1<<PD1); //set PD6 as output for led
	PORTD&=~(1<<PORTD6); //initially turn off the led
	INT1_Init(); 
	ADC_Init(ch);
	
    while(1)
    { 
		/*i have used leds on PORTB to read temperature in binary Formula  */
		PORTB=ADC_Read();
		itoa(ADC_Read(),temp_string,10); //Function for Masking integer to String
		serial_avr(text);
		serial_avr(temp_string);
		serial_avr(" . ");
		_delay_ms(3000); //send continuous serial date comes from temperature sensor with 3 second rate 

    }
}
