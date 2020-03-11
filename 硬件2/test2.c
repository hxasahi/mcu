#include<reg52.h>

unsigned char const zuo[]={0x06,0x0f,0x1e,0x0f,0x06};
unsigned char code shang[]={0xfe,0xfd,0xfb,0xf7,0xef};

sbit DU = P2^6;
sbit WE = P2^7;	  /*后面没有用到*/

void clean()
{
DU=1;  	 /*调用寄存器*/
P0=0X00;  
DU=0;  /*寄存器进入高阻抗状态
         WE=0没有用到是因为DU=0所以WE=0与否已经不重要，对其不影响*/

}

void delay(unsigned int cnt)
{
 while(--cnt);
}

main()
{
 unsigned char i;

 while(1)
      {
 	   clean();	
       P0=zuo[i];
	   P2=shang[i];  
	   i++;
	   if(i==5)    
	     i=0;
      }
}