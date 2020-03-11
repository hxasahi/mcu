/****************************
表白神器  音乐盒
****************************
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
#include <stc12c5a60s2.h>
#define Xtal 33177600  //赋晶振值
#define All 4          //歌曲总数


sbit BUZZ = P1^3;  //蜂鸣器控制引脚
sbit KEY = P2^4;   //按键定义
sbit KEY4 = P2^7;
sbit KEY12 = P2^5;
sbit ADDR0 = P2^0; //数码管显示控制
sbit ADDR1 = P2^1;
sbit ADDR2 = P2^2;
sbit ENLED = P1^1; //总线收发器

bit KeySta = 1;  //当前按键状态
bit KeySta4 = 1;  
bit KeySta12 = 1;
bit backup = 1;  //默认弹起时时一
bit backup4 = 1;
bit backup12 = 1;
bit cntkey = 0;  //判断暂停还是播放
bit breakflage = 0;//用按键改变歌曲标志

unsigned char keybuf = 0xFF;
unsigned char keybuf4 = 0xFF;
unsigned char keybuf12 = 0xFF;
unsigned char Num = 1; //第几首歌
unsigned char sizeofNote;//节拍的多少
               
extern void Delay_ms(unsigned int ms);//延时函数

void PlayControl(unsigned char Num);//歌曲选择函数
void Play();//演奏函数

unsigned char code LedChar[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};//数码管显示字符转换表
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

unsigned char code XiaoXingXingBeat[] = {
        4, 4, 4, 4,    4, 4, 8,    4, 4, 4, 4,    4, 4, 8,
        4, 4, 4, 4,    4, 4, 8,    4, 4, 4, 4,    4, 4, 8,
        4, 4, 4, 4,    4, 4, 8,    4, 4, 4, 4,    4, 4, 8,};
unsigned char code TwoTigerNote[] = {    //两只老虎音符表
        1,   2,   3, 1,    1,   2,   3, 1,   3, 4, 5,   3, 4, 5,
        5,6, 5,4, 3, 1,    5,6, 5,4, 3, 1,   1, 5, 1,   1, 5, 1,0xff};

unsigned char code TwoTigerBeat[] = {    //两只老虎节拍表，4表示一拍，1就是1/4拍，8就是2拍
        4,   4,   4, 4,    4,   4,   4, 4,   4, 4, 8,   4, 4, 8,
        3,1, 3,1, 4, 4,    3,1, 3,1, 4, 4,   4, 4, 8,   4, 4, 8,
    };
unsigned char code HappybirthdayNote[] = {  //生日快乐歌
        5, 5, 6, 5, 8, 7, 22, 5, 5, 6, 5, 9, 8, 5, 5, 12, 10, 8, 7, 6, 11, 11,  10, 8, 9, 8,0xff };
unsigned char code HappybirthdayBeat[] = {
        2, 2, 4, 4, 4, 4, 4, 2, 2,  4, 4, 4, 8, 2, 2, 4, 4, 4, 4, 8, 2, 2, 4, 4, 4, 8};       
unsigned char code         ZhiduanqingchangNote[] = { //纸短情长
              //8, 9, 10, 9, 8, 6, 22, 11, 10, 9, 22, 7, 8, 9, 7, 7, 5, 22, 7, 9, 8, 8, 3, 4, 2, 3, 1, 1, 5, 2,  //前奏
        19, 3, 3, 3, 2, 3, 1, 2, 2, 2, 1, 2, 5, 1, 1, 1, 20, 1, 20, 19,     2, 2, 2, 3, 20, 19, 4, 4, 4, 3, 4, 1, 2, 2, 2, 1, 2, 5,
               1, 1, 1, 20, 1, 20, 3, 2, 2, 1, 1, 22, 22, 22, 1, 1, 2,             3, 3, 3, 22, 3, 2, 1, 21, 6, 5, 22, 5, 6, 7, 8, 3, 3, 22, 8, 7, 8,
              7, 3, 5, 5, 6, 8, 5, 6, 22, 6, 5, 4,5, 3, 2, 1, 20, 1,               3, 2, 2, 1, 2, 5, 1, 2, 5, 22, 22, 22, 1, 1, 2,
        3, 3, 2, 3, 22, 3, 2, 1, 5, 5, 3, 5, 22, 5, 6, 7,                   8, 8, 8, 8, 8, 7, 6, 7, 6, 3, 5, 5, 5, 6, 8, 5, 6, 22, 6, 5, 4,
        5, 3, 2, 1, 6, 1, 3, 2, 2, 1, 2, 1, 20, 1, 22,                      22, 20, 1, 3, 2, 2, 22, 2, 1, 2, 1, 1, 22, 20, 1, 1, 0xff};       
unsigned char code ZhiduanqingchangBeat[] = {
              //2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 8, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 8, 8, 12, 4, 5, 4, 8, 8, 4,   //前奏
        8, 2, 2, 2, 2, 2, 5, 2, 2, 2, 2, 2, 5, 2, 2, 2, 2, 2, 4, 2,       2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 4, 5, 2, 2, 2, 2, 2, 5,
        2, 2, 2, 2, 2, 5, 2, 2, 2, 2, 8, 4, 4, 2, 2, 2, 2,                4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2,
        4, 2, 5, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 5, 1, 1,             2, 2, 2, 2, 2, 4, 2, 16, 16,4, 4, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,                   2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 4, 2, 2, 2, 4, 2, 2, 2, 2, 2,
        2, 4, 2, 5, 1, 1, 2, 2, 2, 2, 2, 4, 1, 12, 4,                     2, 1, 1, 2, 2, 8, 2, 1, 1, 2, 2, 8, 2, 2, 12, 16};
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
          ADDR2=1; ADDR1=0; ADDR0=1;//选择最右边的数码管
          P23 = 0;      //启用KEY
   
    while (1)
    {
                        P0 = LedChar[Num]; //数码管显示当前歌曲序号
      PlayControl(Num);                       
                        if(breakflage == 1)
                                breakflage = 0;
                        else
                                Delay_ms(500);
    }
}

void PlayControl(unsigned char Num)
{
        switch(Num)
        {
                case 4:SongNote = XiaoXingXingNote;    SongBeat =  XiaoXingXingBeat;  sizeofNote = sizeof(XiaoXingXingNote);   Play();break;
    case 2:SongNote = TwoTigerNote;        SongBeat =TwoTigerBeat;        sizeofNote = sizeof(TwoTigerNote);       Play();break;
                case 3:SongNote = HappybirthdayNote;   SongBeat =HappybirthdayBeat;   sizeofNote = sizeof(HappybirthdayNote);  Play();break;
                case 1:SongNote = ZhiduanqingchangNote;SongBeat =ZhiduanqingchangBeat;sizeofNote = sizeof(ZhiduanqingchangNote);Play();break;               
                default:break;
        }
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
/*按键扫描*/
void InterruptTimer1() interrupt 3 //按键部分参看之前的帖子 带有计次功能的秒表
{
        TH1 = 0xC9;
        TL1 = 0xEA;
       
          keybuf = (keybuf<<1) | KEY;
    if (keybuf == 0x00)
        KeySta = 0;
    else if (keybuf == 0xFF)
        KeySta = 1;
    else{}
                       
          keybuf4 = (keybuf4<<1) | KEY4;
    if (keybuf4 == 0x00)
        KeySta4 = 0;
    else if (keybuf4 == 0xFF)
        KeySta4 = 1;
    else{}
                       
         keybuf12= (keybuf12<<1) | KEY12;
    if (keybuf12 == 0x00)
        KeySta12 = 0;
    else if (keybuf12 == 0xFF)
        KeySta12 = 1;
    else{}
                       
                              if (KeySta != backup)  
        {
            if (backup == 0)        
                                                {
                                                        cntkey=~cntkey;
                                                        TR0 = cntkey;
                                                }
            backup = KeySta;               
        }
                if (KeySta4 != backup4)
                {
                        if(backup4 ==0)
                        {
                                breakflage = 1;
                                if(Num <All)
                                  Num++;
                          else if(Num == All)
                                        Num =1;
                                P0 = LedChar[Num];
                        }
                        backup4 = KeySta4;
                }               
               
                if (KeySta12 != backup12)
                {
                        if(backup12 ==0)
                        {
                                breakflage = 1;
                                if(Num >1)
                                  Num--;
                          else if(Num == 1)
                                        Num =All;
                                P0 = LedChar[Num];
                        }
                        backup12 = KeySta12;
                }                       
               
}
