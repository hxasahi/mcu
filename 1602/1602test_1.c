#include<reg52.h>
unsigned char code up[]="Good evening!";
unsigned char code down[]="    By Hx";

sbit rs=P2^3;	//1602������/ָ��ѡ�������
sbit rw=P2^4;	//1602�Ķ�д������
sbit en=P2^5;	//1602��ʹ�ܿ�����

void delay(unsigned int n)   //��ʱ����                     
{
    unsigned int x,y;
    for(x=n;x>0;x--)
        for(y=110;y>0;y--);
}

void lcd_wcmd(unsigned char cmd)  //1602д�����      
{
    rs=0;            //ѡ��ָ��Ĵ���
    rw=0;            //ѡ��д
    P0=cmd;            //������������P2
    delay(5);            //��ʱһС�������1602׼����������
    en=1;           //ʹ���ߵ�ƽ�仯����������1602��8λ���ݿ�
    en=0;
}

void lcd_wdat(unsigned char dat)
{
    rs=1;
    rw=0;
    P0=dat;
    delay(5);
    en=1;
    en=0;
}

void lcd_init()
{
    lcd_wcmd(0x38);       //8λ���ݣ�˫�У�5*7����      
    lcd_wcmd(0x0c);      //������ʾ�����ع�꣬��겻��˸
    lcd_wcmd(0x06);    //��ʾ��ַ��������дһ�����ݺ���ʾλ������һλ
    lcd_wcmd(0x01);    //����
}

void main()            //������
{     
    unsigned char n,m=0;
    lcd_init();                 //Һ����ʼ��
    lcd_wcmd(0x80);   //��ʾ��ַ��Ϊ80H����00H�������ŵ�һλ      
    for(m=0;m<16;m++)     //��table[]�е���������д��1602��ʾ
    {
            lcd_wdat(up[m]);           
            delay(200);
    }
    lcd_wcmd(0x80+0x44); //�����趨��ʾ��ַΪ0xc4,�����ŵ�5λ
    for(n=0;n<8;n++)   //��table1[]�е���������д��1602��ʾ
    {     
           lcd_wdat(down[n]);
           delay(200);
    }
    while(1);        //��̬ͣ��
}