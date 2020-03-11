/*
晶振值33.1776MHz
功能简介：K16控制暂停与继续
          K4 上一曲
                                        K12下一曲
    数码管显示当前歌曲序号
****************************
歌曲分别是：
纸短情长
两只老虎
祝你生日快乐歌
小星星
****************************/
#include <reg52.h>
#define Xtal  11059200  //赋晶振值



sbit BUZZ = P1^3;  //蜂鸣器控制引脚

unsigned char keybuf = 0xFF;
unsigned char keybuf4 = 0xFF;
unsigned char keybuf12 = 0xFF;
unsigned char sizeofNote;//节拍的多少
               
extern void Delay_ms(unsigned int ms);//延时函数

void Play();//演奏函数

unsigned int code NoteFrequ[] = {  //中音1-7低音1-7和高音1-7对应频率列表
    523,  587,  659,  698,  784,  880,  988,  //中音1-7
    1047, 1175, 1319, 1397, 1568, 1760, 1976,  //高音1-7
          262, 294, 330, 349, 392, 440, 494,300//低音1-7  最后一个纯属凑数
};
unsigned int code NoteReload[] = { //中音1-7和高音1-7对应的定时器重载值
    65536 - (Xtal/12) / (523*2),  //中音1
    65536 - (Xtal/12) / (587*2),  //2
    65536 - (Xtal/12) / (659*2),  //3
    65536 - (Xtal/12) / (698*2),  //4
    65536 - (Xtal/12) / (784*2),  //5
    65536 - (Xtal/12) / (880*2),  //6
    65536 - (Xtal/12) / (988*2),  //7
    65536 - (Xtal/12) / (1047*2), //高音1
    65536 - (Xtal/12) / (1175*2), //2
    65536 - (Xtal/12) / (1319*2), //3
    65536 - (Xtal/12) / (1397*2), //4
    65536 - (Xtal/12) / (1568*2), //5
    65536 - (Xtal/12) / (1760*2), //6
    65536 - (Xtal/12) / (1976*2), //7
          65536 - (Xtal/12) / (262*2), //低音1
    65536 - (Xtal/12) / (294*2), //2
    65536 - (Xtal/12) / (330*2), //3
    65536 - (Xtal/12) / (349*2), //4
    65536 - (Xtal/12) / (392*2), //5
    65536 - (Xtal/12) / (440*2), //6
    65536 - (Xtal/12) / (494*2), //7
                65536 - (Xtal/12) / (300*2), //纯属凑数
};

unsigned char *SongNote, *SongBeat;//指针
unsigned char code XiaoXingXingNote[] = {   //小星星
        1, 1, 5, 5,    6, 6, 5,    4, 4, 3, 3,    2, 2, 1,  
        5, 5, 4, 4,    3, 3, 2,    5, 5, 4, 4,    3, 3, 2 ,
        1, 1, 5, 5,    6, 6, 5,    4, 4, 3, 3,    2, 2, 1, 0xff    };
bit enable = 1;   //蜂鸣器发声使能标志
bit tmrflag = 0;  //定时器中断完成标志
unsigned char T0RH = 0xFF;  //T0重载值的高字节
unsigned char T0RL = 0x00;  //T0重载值的低字节


void main()
{
   
    EA = 1;       //使能全局中断
    TMOD = 0x11;  //配置T0，T1工作在模式1
    TH0 = T0RH;   
    TL0 = T0RL;
    ET0 = 1;      //使能T0中断
          ET1 = 1;      //使能T1中断
          TR1 = 1;      //启动T1
          ENLED = 0x00; //使能总线收发器
   
}
/* 演奏函数 */
void Play()//演奏函数
{
    unsigned char beat;   //当前节拍索引
    unsigned char note;   //当前节拍对应的音符
    unsigned int time = 0;      //当前节拍计时
    unsigned int beatTime = 0;  //当前节拍总时间
    unsigned int soundTime = 0; //当前节拍需发声时间

    for (beat=0; beat<sizeofNote; )  //用节拍索引作为循环变量
    {
                                if(breakflage == 1)
                                        break;
        while (!tmrflag);  //每次定时器中断完成后，检测并处理节拍
        tmrflag = 0;
        if (time == 0)  //当前节拍播完则启动一个新节拍
        {
                                        if(breakflage == 1)
                                                break;
            note = SongNote[beat] - 1;
                                          if(note == 0xfe)
                                                {
                                                        if(Num == All)
                                                                Num = 1;
                                                        else
                                                        Num++;
                                                        break;
                                                }
            T0RH = NoteReload[note] >> 8;
            T0RL = NoteReload[note];
            beatTime = (SongBeat[beat] * NoteFrequ[note]) >> 2;//计算节拍总时间，右移2位相当于除4，移位代替除法可以加快执行速度
            soundTime = beatTime - (beatTime >> 2);//计算发声时间，
            enable = 1;  //指示蜂鸣器开始发声
                                                if (note == 21)//简谱的0 等待
                                                        enable =0;
            time++;
        }
        else  //当前节拍未播完则处理当前节拍
        {
            if (time >= beatTime)  //当前持续时间到达节拍总时间时归零，
            {                      //并递增节拍索引，以准备启动新节拍
                time = 0;
                beat++;
            }
            else  //当前持续时间未达到总时间时，
            {
                time++;   //累加时间计数
                if (time == soundTime)  //到达发声时间后，指示关闭蜂鸣器，
                {                       //插入0.25*总时间的静音间隔，
                    enable = 0;         //用以区分连续的两个节拍
                }
            }
        }
    }
}
/*控制蜂鸣器发声 */
void InterruptTimer0() interrupt 1
{
    TH0 = T0RH;   //重新加载重载值
    TL0 = T0RL;
    tmrflag = 1;
    if (enable)   //使能时反转蜂鸣器控制电平
        BUZZ = ~BUZZ;
    else          //未使能时关闭蜂鸣器
        BUZZ = 1;
}
