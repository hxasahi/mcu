#include<reg52.h>

unsigned char const zuo[]={0x06,0x0f,0x1e,0x0f,0x06};
unsigned char code shang[]={0xfe,0xfd,0xfb,0xf7,0xef};

sbit DU = P2^6;
sbit WE = P2^7;	  /*����û���õ�*/

void clean()
{
DU=1;  	 /*���üĴ���*/
P0=0X00;  
DU=0;  /*�Ĵ���������迹״̬
         WE=0û���õ�����ΪDU=0����WE=0����Ѿ�����Ҫ�����䲻Ӱ��*/

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