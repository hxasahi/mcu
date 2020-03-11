#include<reg51.h>

sbit led_1 = P0^1;

void main()
{
    while(1)
    led_1 = 0;
} 