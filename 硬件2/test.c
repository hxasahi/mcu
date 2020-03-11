#include<reg52.h>

unsigned char const zuo[]={0x06,0x0f,0x1e,0x0f,0x06};
unsigned char code shang[]={0xfe,0xfd,0xfb,0xf7,0xef};

sbit DU = P2^6;
sbit WE = P2^7;

void clean()
{
DU=1;  
P0=0X00;
DU=0;

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
	   delay(200); 
	   i++;
	   if(i==5)    
	     i=0;
      }
}