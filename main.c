#include <mega32.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>


#define LED PORTD.0 
#define on 1
#define off 0

#define ADC_VREF_TYPE 0x00  //Volt.Ref


unsigned char str[17];
float R;



float read_adc(unsigned char adc_input)
{
    ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
    delay_us(10);   // Delay needed for the stabilization of the ADC input voltage
    ADCSRA|=0x40; // Start the AD conversion
    while ((ADCSRA & 0x10)==0); // Wait for the AD conversion to complete
    ADCSRA|=0x10;
    return ADCW;
}

void main(void)
{
    DDRA = 0x00;
    PORTA = 0x00;
    
    DDRB = 0x00;
    PORTB = 0x00;
    
    DDRC = 0x00;
    PORTC = 0x00;
    
    DDRD = 0xff;
    PORTD = 0x00;
    
    lcd_init(16);
    
    // ADC Clock frequency: 125.000 kHz
    // ADC Voltage Reference: Int., cap. on AREF
    ADMUX=ADC_VREF_TYPE & 0xff;
    ADCSRA=0x85;
    
    lcd_gotoxy(0, 0);
    lcd_puts("Hello, Welcome!");
    delay_ms(3000); 
    lcd_clear();

#asm("sei")

    while (1)
    {
        R = read_adc(1);   
            
        if(R == 1023)
        {
            LED = on;
            lcd_putsf("R = inf");
            delay_ms(100);
            lcd_clear();   
        }
        else
        {
            LED = off;
            R = (R * 5) / 1023;
            R = (R * 1000) / (5 - R);
             
            if(R < 1000)
            {
                sprintf(str, "R=%3.2f ohm", R);
                lcd_puts(str); 
                delay_ms(100);
                lcd_clear();
            }
            else
            {
                R /= 1000;
                sprintf(str, "R=%3.2f Kohm", R);
                lcd_puts(str);
                delay_ms(100); 
                lcd_clear();  
            }                     
        }
    }
}
