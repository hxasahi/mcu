/*12M¾§Õñ*/
#include<reg52.h>
unsigned int sum;
sbit led = P1^1;
void main()
{
TMOD = 0x01;
TH0 = 60;
TL0 = 176;
EA = 1;
ET0 = 1;
TR0 = 1;
while(1){
	if (sum == 20 ){
	    led = ~led;
		sum = 0;
	}
}
}
void T0_time() interrupt 1
{
    TH0 = 60;
	TL0 = 176;
	sum++;
}