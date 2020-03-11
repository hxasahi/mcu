#include<reg51.h>

k1=P3^4;
k2=P3^5;

void delay_s()
{
 int x,y;
 for(x=0;x<=50;x++)
 for(y=0;y<=200;y++);
}

void delay()
{
    int a,b;
	for (a=0;a<=500;a++)
	for (b=0;b<=200;b++);
}

void main()
{
    while(1)
	{
	    if (k1==0){
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
		elif(k2==0){
		P1=0xfe;	/*1,2*/
		delay();
		P1=0xfd;
		delay();
		
		P1=0xfb;   /*3,4*/
		delay();
		P1=0xf7;		
		delay();

		P1=0xef;   /*5,6*/
		delay();
		P1=0xdf;
		delay();

		P1=0xbf;  /*7,8*/
		delay();
		P1=0x7f;  
		delay();
		}
		else{
		P1=0x00;
		delay_s();
		P1=0xff;
		delay_s();
		P1=0x00;
		delay();
		P1=0xff;
		delay();
		}		
	}
} 