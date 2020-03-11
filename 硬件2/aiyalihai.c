#include<reg51.h>

void delay_s()
{
 int x,y;
 for(x=0;x<=50;x++)
 for(y=0;y<=200;y++);
}

void main()
{
    while(1)
	{	    
        P1=0xfe; /*1*/
		delay_s();
		P1=0xfd; /*2*/
		delay_s();
		P1=0xfb; /*3*/
		delay_s();
		P1=0xf7; /*4*/
		delay_s();
		P1=0xef; /*5*/
		delay_s();
		P1=0xdf; /*6*/
		delay_s();
		P1=0xbf;  /*7*/
		delay_s();
		P1=0x7f;  /*8*/
		delay_s();
	}
} 