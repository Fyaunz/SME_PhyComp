#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
//#include "iom328p.h"
#define FOSC 16000000UL // Clock Speed
#define BAUD 19200
#define MYUBRR (FOSC/(BAUD * 16UL))-1

volatile int16_t sebufStart = 0;
volatile int16_t sebufEnd = 0;
volatile uint8_t sebuffer[128];

volatile int16_t recStart = 0;
volatile int16_t recEnd = 0;
volatile uint8_t recbuffer[64];

void USART_init(void) {
    //Set baud rate
    unsigned int ubrr = MYUBRR;
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    // Set frame format: 8data, 2stop bit
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);

    sei();
}

ISR(USART_RX_vect){
    int16_t newrecEnd = (recEnd+1)&63;
    uint8_t data = UDR0;
    recbuffer[recEnd] = data;
    recEnd = newrecEnd;
}

ISR(USART_UDRE_vect){
    if(sebufStart != sebufEnd){
        UDR0 = sebuffer[sebufStart];
        sebufStart = (sebufStart+1)&127;
    } else {
        UCSR0B &= ~(1<<UDRIE0);
    }
}
ISR(ADC_vect){

}
// Ausgabe eines einzelnen Zeichens
void put_c(uint8_t data ) {
    while((sebufStart) > sebufEnd);
    sebuffer[sebufEnd] = data;
    sebufEnd = (sebufEnd+1)&127;
    UCSR0B |= 1<<UDRIE0;
}

// Input eines Zeichens
uint8_t getChar(void) {
    UCSR0B |= (1<<RXCIE0);
    while(recStart == recEnd){}
    uint8_t data = recbuffer[recStart];
    recStart = (recStart+1)&63;
    return data;
}

//Prints a char as binary value
void putBin_char(uint8_t c) {
    for (int i = 7; i >= 0; --i){
        put_c( (c & (1 << i)) ? '1' : '0' );
    }
    // new line with \r because either picocom or wsl are stupid
    putString("\n\r");
}

void putBin_int(int32_t x){
    uint32_t data;
    // pointer magic really really evil 
    data = * (uint32_t *) &x;

    uint32_t buf[32];
    int i;
    char c;
    // fills buf with 0 if x is to short
    for(int j=0;j<32;j++){
        buf[j]=0;
    }
    // writes binary values into buf
    for(i=0; data>0;i++){
        buf[i] = data%2;
        data = data/2;
    }
    // reads binary values and puts spaces after each 8 values
    for(i=31;i>=0;i--){
        c = buf[i]+'0';
        put_c(c);
        if(!(i%8)){
            put_c(' ');
        }

    }
    // new line with \r because either picocom or wsl are stupid
    putString("\n\r");

}

void putHex(int32_t x) {
    uint32_t data;
    // pointer magic really really evil
    data = * (uint32_t *) &x;
    int i = 0;
    int base = 16;
    uint8_t outbuf[8];
    // fill outbuf with 0s for if x is to short
    for(int j=0;j<8;j++){
        outbuf[j] = '0';
    }

    // write hexvalue into outbuf really cleverly
    do{
        outbuf[i] = "0123456789ABCDEF"[data % base];
        i++;
        data = data/base;
    }while( data > 0);

    //read hexvalues from outbuf and but 0x before each 2 values + space
    for(i = 7;i>=0;i--){
        if(i%2){
            if(i != 7)put_c(' ');
            putString("0x");
        }
        put_c(outbuf[i]);
    }
    // new line with \r because either picocom or wsl are stupid
    putString("\n\r");

}

//Ausgabe eines Strings
void putString(uint8_t *s) {
    // print chars until cstring terminates with \0
    while(*s){
        put_c(*s);
        s++;
    }
}

// Ausgabe einer Dezimalzahl
void putDec(int32_t x) {
    unsigned char buf[16];
    //if x is a negative nummber
    if (x<0) {
        put_c('-');
        x = -x;
    }
    if (x==0) {
        put_c('0');
    } else {
        int i=0;
        while (i<16 && x>0) {
            buf[i++] = '0' + (x%10);
            x = x/10;
        }
        i=i-1;
        while (i>= 0) put_c(buf[i--]);
        putString("\n\r");
    }
    // new line with \r because either picocom or wsl are stupid
}
