System Requirements :
1. Turn ON and OFF the Led using a Switch 
2. if the switch is pressed send “The Switch is pressed” to the serial monitor only one time also send the LED states ”ON” or “OFF”
3. Measure temperature continuously and send it to the serial monitor every 3 seconds  ,and display it on another PORT(PORTB) in a       Binary presentation using leds 
_________________________________________________________________________________________________________________________________

1. Turn ON and OFF the Led using a Switch:   

Firstly, PINS Configuration ( i have used output pins for the led and input pins for switch and temperature sensor )


	/* PINS Configuration */
	
  	DDRB=0xff;                //PORTB is Output for binary presentation of Temperature 
	DDRD &=~(1<<DDD3);        //PD3 is input for the swtich ,as this is the INT1 pin
	PORTD |=(1<<PD3);         //enable pull up for PD3
	DDRC &=~(1<<DDC0);        //input pin for temp sensor
	PORTC|=(1<<PORTC0);	      //enable pull up
	DDRD|=(1<<PD6)|(1<<PD1);  //set PD6 as output for led
	PORTD&=~(1<<PORTD6);      //initially turn off the led
  

2. if the switch is pressed send “The Switch is pressed” to the serial monitor only one time also send the LED states ”ON” or “OFF”:  
when switch is pressed the interrupt is enabled and the ISR of INT1 is executed ,in which turn ON or OFF the Led and send the state of the led to the serial monitor and indicate that the switch is pressed 


	 ISR(INT1_vect){
	 PORTD =PORTD^(1<<PD6);           //Toggle the led
	 led_state++;    
	 /*initially led_state equaled 0 ,after first press --> led_state=1 which means it is ON now (as it is odd Numebr )*/
	serial_avr(" The switch is pressed, ");
	//Check The Value of led_state if it's even or odd 
	if(led_state%2==0)    //led_state is even --> the led is OFF 
		serial_avr(" The Led is OFF .");
		else 
		serial_avr(" The Led is ON .");
		}



serial_avr Function takes String parameter and display it on the serial monitor [Virtual Terminal] using USART Transmitter 


	void serial_avr(char *str){
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);	
	UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);
	UBRR0=51;		//Setting baud rate to 9600
	for	(unsigned int i=0 ;str[i]!=0;i++){
		UDR0=str[i];
		while ( !( UCSR0A &(1<<UDRE0))) ;
	}
	}



3. Measure temperature continuously and send it to the serial monitor every 3 seconds  ,and display it on another PORT(PORTB) in a       Binary formula using leds:  
using LM Temperature Sensor which reads temperature in analog value, so we have to use ADC to Convert This Analog Value to Digital Value and display this Digital value on the virtual terminal and display its binary value on PORTB.
first, initialize ADC and Select The ADC Channel, Then Read ADC Reading. The two Functions to do This :


	/*ADC Initialization*/
	void ADC_Init(char channel)
 	{
	 ADMUX   = channel;								               	//Select Channel 
	 ADCSRA |= (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);		//Enable ADC ,and determine Division Factor to 8 [Prescaler] 
	 }


 	/*Read Temperature */
	int  ADC_Read()
 	{ 
  	 int adc_value =0;
	 ADCSRA |= (1<<ADSC);			  		// Start conversion. 
	 while (ADCSRA & (1<<ADSC));				// Wait for conversion to complete. 
	 adc_value = ADCW;					//Read value of sensor and store it in 	adc_value variable 
	 adc_value=adc_value/2;				  	//divide adc_value by 2 to get real value 
	 return adc_value;
 	}
 
 

Every 3 seconds, display the temperature in a binary presentation on PORTB using Leds. and send the temperature to the serial monitor, and because of the serial_avr Function takes a string paramter ,i have used itoa function to convert the temperature form integer to string data type 


 	while(1)
   	 { 
		
		PORTB=ADC_Read();                   //display the temperature in a binary presentation on PORTB using Leds.
		itoa(ADC_Read(),temp_string,10);    //Function for Masking integer to String
		serial_avr(text);
		serial_avr(temp_string);
		serial_avr(" . ");
		_delay_ms(3000);                    //send continuous serial date comes from temperature sensor with 3 second rate 

    }


