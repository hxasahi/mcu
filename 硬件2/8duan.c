#include<reg52.h>

sbit duan=P2^6;
sbit wei=P2^7;

void main()
{
    wei = 1;
	P0=0xfe;
	wei = 0;
		
	duan = 1;
	P0=0x4f;
	duan = 0;
    while(1);	
}

