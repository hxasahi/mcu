#include<reg52.h>
unsigned char code up[]="Good evening!";
unsigned char code down[]="    By Hx";

sbit rs=P2^3;	//1602的数据/指令选择控制线
sbit rw=P2^4;	//1602的读写控制线
sbit en=P2^5;	//1602的使能控制线

void delay(unsigned int n)   //延时函数                     
{
    unsigned int x,y;
    for(x=n;x>0;x--)
        for(y=110;y>0;y--);
}

void lcd_wcmd(unsigned char cmd)  //1602写命令函数      
{
    rs=0;            //选择指令寄存器
    rw=0;            //选择写
    P0=cmd;            //把命令字送入P2
    delay(5);            //延时一小会儿，让1602准备接收数据
    en=1;           //使能线电平变化，命令送入1602的8位数据口
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
    lcd_wcmd(0x38);       //8位数据，双列，5*7字形      
    lcd_wcmd(0x0c);      //开启显示屏，关光标，光标不闪烁
    lcd_wcmd(0x06);    //显示地址递增，即写一个数据后，显示位置右移一位
    lcd_wcmd(0x01);    //清屏
}

void main()            //主函数
{     
    unsigned char n,m=0;
    lcd_init();                 //液晶初始化
    lcd_wcmd(0x80);   //显示地址设为80H（即00H，）上排第一位      
    for(m=0;m<16;m++)     //将table[]中的数据依次写入1602显示
    {
            lcd_wdat(up[m]);           
            delay(200);
    }
    lcd_wcmd(0x80+0x44); //重新设定显示地址为0xc4,即下排第5位
    for(n=0;n<8;n++)   //将table1[]中的数据依次写入1602显示
    {     
           lcd_wdat(down[n]);
           delay(200);
    }
    while(1);        //动态停机
}