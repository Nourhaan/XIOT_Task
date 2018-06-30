# XIOT_Task [Embedded Systems Development Task ]

Tools :
- AtmelStudio6 
- Proteus 

Components :
- ATmega328P 
- Temperature Sensor LM35
- Led 
- Switch 
- Capacitor 100uf
- Virtual Terminal 
- Power 5 Volt 
- Ground

Project instruction :
- open design using proteus 
- open C file usinng AtmlStudio and dubug it using f7 key 
- double click on ATmega328P in proteus simulation and browse to select the hex file of C file [it's created after dubugging the C file ]
- run the simulation, and if the virtual terminal is not appeared, right click on it and choose Virtual Terminal option 
- press the switch to Turn the Led ON or OFF ,the State of the Led will be displayed on the virtual terminal 
- the temperature will be displayed on PORTB in its Binary Formula usind leds and on Virtual Terminal 

---> ON & OFF Led State 
i have used led_State counter initially equals 0 to determine the state of the Led if it is ON or OFF.
The counter is incremented by 1 each time the switch is pressed, then check if the counter is an even number or odd, if the counter is even number then the led is OFF ,but if the counter is odd number then the led is ON.

---> Decrease the Temperature Sensor ,the Reading is fixed to the value before decrement
also i used PORTB to Display the Temperature in a binary formula using leds in addition to displaying it on the Virtual terminal every 3 seconds.
but i faced a problem when i decreases the temperature ,the new reading doesn't be updated , the temperature sensor reads and display the last reading before decrementing. but increasing is readed correctly .

---> Showing Data on Virtual Terminal using USART Transmitter 
i have used avr_serial which takes a string parameter and shows it on the virtual terminal by using USART Transmitter.
and because of that i used the itoa function which takes the temperature in integer data type and convert it to string data type then send this string to avr_serial function to  display the temperature on virtual terminal 
