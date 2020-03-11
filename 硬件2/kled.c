#include<reg51.h>

sbit k1 = P3^4;
sbit led_1 = P0^1;
sbit led_2 = P0^2;
sbit led_3 = P0^3;
sbit led_4 = P0^4;
sbit beep = P2^0;

void main()
{
    while(1)
    led_1 = 0;
	led_2 = 0;
	led_3 = 0;
	led_4 = 0;
	beep = 1;		
} 