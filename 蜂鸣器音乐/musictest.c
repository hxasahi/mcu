/****************************
�������  ���ֺ�
****************************
����ֵ33.1776MHz
���ܼ�飺K16������ͣ�����
          K4 ��һ��
                                        K12��һ��
    �������ʾ��ǰ�������
****************************
�����ֱ��ǣ�
ֽ���鳤
��ֻ�ϻ�
ף�����տ��ָ�
С����
****************************/
#include <stc12c5a60s2.h>
#define Xtal 33177600  //������ֵ
#define All 4          //��������


sbit BUZZ = P1^3;  //��������������
sbit KEY = P2^4;   //��������
sbit KEY4 = P2^7;
sbit KEY12 = P2^5;
sbit ADDR0 = P2^0; //�������ʾ����
sbit ADDR1 = P2^1;
sbit ADDR2 = P2^2;
sbit ENLED = P1^1; //�����շ���

bit KeySta = 1;  //��ǰ����״̬
bit KeySta4 = 1;  
bit KeySta12 = 1;
bit backup = 1;  //Ĭ�ϵ���ʱʱһ
bit backup4 = 1;
bit backup12 = 1;
bit cntkey = 0;  //�ж���ͣ���ǲ���
bit breakflage = 0;//�ð����ı������־

unsigned char keybuf = 0xFF;
unsigned char keybuf4 = 0xFF;
unsigned char keybuf12 = 0xFF;
unsigned char Num = 1; //�ڼ��׸�
unsigned char sizeofNote;//���ĵĶ���
               
extern void Delay_ms(unsigned int ms);//��ʱ����

void PlayControl(unsigned char Num);//����ѡ����
void Play();//���ຯ��

unsigned char code LedChar[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};//�������ʾ�ַ�ת����
unsigned int code NoteFrequ[] = {  //����1-7����1-7�͸���1-7��ӦƵ���б�
    523,  587,  659,  698,  784,  880,  988,  //����1-7
    1047, 1175, 1319, 1397, 1568, 1760, 1976,  //����1-7
          262, 294, 330, 349, 392, 440, 494,300//����1-7  ���һ����������
};
unsigned int code NoteReload[] = { //����1-7�͸���1-7��Ӧ�Ķ�ʱ������ֵ
    65536 - (Xtal/12) / (523*2),  //����1
    65536 - (Xtal/12) / (587*2),  //2
    65536 - (Xtal/12) / (659*2),  //3
    65536 - (Xtal/12) / (698*2),  //4
    65536 - (Xtal/12) / (784*2),  //5
    65536 - (Xtal/12) / (880*2),  //6
    65536 - (Xtal/12) / (988*2),  //7
    65536 - (Xtal/12) / (1047*2), //����1
    65536 - (Xtal/12) / (1175*2), //2
    65536 - (Xtal/12) / (1319*2), //3
    65536 - (Xtal/12) / (1397*2), //4
    65536 - (Xtal/12) / (1568*2), //5
    65536 - (Xtal/12) / (1760*2), //6
    65536 - (Xtal/12) / (1976*2), //7
          65536 - (Xtal/12) / (262*2), //����1
    65536 - (Xtal/12) / (294*2), //2
    65536 - (Xtal/12) / (330*2), //3
    65536 - (Xtal/12) / (349*2), //4
    65536 - (Xtal/12) / (392*2), //5
    65536 - (Xtal/12) / (440*2), //6
    65536 - (Xtal/12) / (494*2), //7
                65536 - (Xtal/12) / (300*2), //��������
};

unsigned char *SongNote, *SongBeat;//ָ��
unsigned char code XiaoXingXingNote[] = {   //С����
        1, 1, 5, 5,    6, 6, 5,    4, 4, 3, 3,    2, 2, 1,  
        5, 5, 4, 4,    3, 3, 2,    5, 5, 4, 4,    3, 3, 2 ,
        1, 1, 5, 5,    6, 6, 5,    4, 4, 3, 3,    2, 2, 1, 0xff    };

unsigned char code XiaoXingXingBeat[] = {
        4, 4, 4, 4,    4, 4, 8,    4, 4, 4, 4,    4, 4, 8,
        4, 4, 4, 4,    4, 4, 8,    4, 4, 4, 4,    4, 4, 8,
        4, 4, 4, 4,    4, 4, 8,    4, 4, 4, 4,    4, 4, 8,};
unsigned char code TwoTigerNote[] = {    //��ֻ�ϻ�������
        1,   2,   3, 1,    1,   2,   3, 1,   3, 4, 5,   3, 4, 5,
        5,6, 5,4, 3, 1,    5,6, 5,4, 3, 1,   1, 5, 1,   1, 5, 1,0xff};

unsigned char code TwoTigerBeat[] = {    //��ֻ�ϻ����ı�4��ʾһ�ģ�1����1/4�ģ�8����2��
        4,   4,   4, 4,    4,   4,   4, 4,   4, 4, 8,   4, 4, 8,
        3,1, 3,1, 4, 4,    3,1, 3,1, 4, 4,   4, 4, 8,   4, 4, 8,
    };
unsigned char code HappybirthdayNote[] = {  //���տ��ָ�
        5, 5, 6, 5, 8, 7, 22, 5, 5, 6, 5, 9, 8, 5, 5, 12, 10, 8, 7, 6, 11, 11,  10, 8, 9, 8,0xff };
unsigned char code HappybirthdayBeat[] = {
        2, 2, 4, 4, 4, 4, 4, 2, 2,  4, 4, 4, 8, 2, 2, 4, 4, 4, 4, 8, 2, 2, 4, 4, 4, 8};       
unsigned char code         ZhiduanqingchangNote[] = { //ֽ���鳤
              //8, 9, 10, 9, 8, 6, 22, 11, 10, 9, 22, 7, 8, 9, 7, 7, 5, 22, 7, 9, 8, 8, 3, 4, 2, 3, 1, 1, 5, 2,  //ǰ��
        19, 3, 3, 3, 2, 3, 1, 2, 2, 2, 1, 2, 5, 1, 1, 1, 20, 1, 20, 19,     2, 2, 2, 3, 20, 19, 4, 4, 4, 3, 4, 1, 2, 2, 2, 1, 2, 5,
               1, 1, 1, 20, 1, 20, 3, 2, 2, 1, 1, 22, 22, 22, 1, 1, 2,             3, 3, 3, 22, 3, 2, 1, 21, 6, 5, 22, 5, 6, 7, 8, 3, 3, 22, 8, 7, 8,
              7, 3, 5, 5, 6, 8, 5, 6, 22, 6, 5, 4,5, 3, 2, 1, 20, 1,               3, 2, 2, 1, 2, 5, 1, 2, 5, 22, 22, 22, 1, 1, 2,
        3, 3, 2, 3, 22, 3, 2, 1, 5, 5, 3, 5, 22, 5, 6, 7,                   8, 8, 8, 8, 8, 7, 6, 7, 6, 3, 5, 5, 5, 6, 8, 5, 6, 22, 6, 5, 4,
        5, 3, 2, 1, 6, 1, 3, 2, 2, 1, 2, 1, 20, 1, 22,                      22, 20, 1, 3, 2, 2, 22, 2, 1, 2, 1, 1, 22, 20, 1, 1, 0xff};       
unsigned char code ZhiduanqingchangBeat[] = {
              //2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 8, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 8, 8, 12, 4, 5, 4, 8, 8, 4,   //ǰ��
        8, 2, 2, 2, 2, 2, 5, 2, 2, 2, 2, 2, 5, 2, 2, 2, 2, 2, 4, 2,       2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 4, 5, 2, 2, 2, 2, 2, 5,
        2, 2, 2, 2, 2, 5, 2, 2, 2, 2, 8, 4, 4, 2, 2, 2, 2,                4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2,
        4, 2, 5, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 5, 1, 1,             2, 2, 2, 2, 2, 4, 2, 16, 16,4, 4, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,                   2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 4, 2, 2, 2, 4, 2, 2, 2, 2, 2,
        2, 4, 2, 5, 1, 1, 2, 2, 2, 2, 2, 4, 1, 12, 4,                     2, 1, 1, 2, 2, 8, 2, 1, 1, 2, 2, 8, 2, 2, 12, 16};
bit enable = 1;   //����������ʹ�ܱ�־
bit tmrflag = 0;  //��ʱ���ж���ɱ�־
unsigned char T0RH = 0xFF;  //T0����ֵ�ĸ��ֽ�
unsigned char T0RL = 0x00;  //T0����ֵ�ĵ��ֽ�


void main()
{
   
    EA = 1;       //ʹ��ȫ���ж�
    TMOD = 0x11;  //����T0��T1������ģʽ1
    TH0 = T0RH;   
    TL0 = T0RL;
    ET0 = 1;      //ʹ��T0�ж�
          ET1 = 1;      //ʹ��T1�ж�
          TR1 = 1;      //����T1
          ENLED = 0x00; //ʹ�������շ���
          ADDR2=1; ADDR1=0; ADDR0=1;//ѡ�����ұߵ������
          P23 = 0;      //����KEY
   
    while (1)
    {
                        P0 = LedChar[Num]; //�������ʾ��ǰ�������
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
/* ���ຯ�� */
void Play()//���ຯ��
{
    unsigned char beat;   //��ǰ��������
    unsigned char note;   //��ǰ���Ķ�Ӧ������
    unsigned int time = 0;      //��ǰ���ļ�ʱ
    unsigned int beatTime = 0;  //��ǰ������ʱ��
    unsigned int soundTime = 0; //��ǰ�����跢��ʱ��

    for (beat=0; beat<sizeofNote; )  //�ý���������Ϊѭ������
    {
                                if(breakflage == 1)
                                        break;
        while (!tmrflag);  //ÿ�ζ�ʱ���ж���ɺ󣬼�Ⲣ�������
        tmrflag = 0;
        if (time == 0)  //��ǰ���Ĳ���������һ���½���
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
            beatTime = (SongBeat[beat] * NoteFrequ[note]) >> 2;//���������ʱ�䣬����2λ�൱�ڳ�4����λ����������Լӿ�ִ���ٶ�
            soundTime = beatTime - (beatTime >> 2);//���㷢��ʱ�䣬
            enable = 1;  //ָʾ��������ʼ����
                                                if (note == 21)//���׵�0 �ȴ�
                                                        enable =0;
            time++;
        }
        else  //��ǰ����δ��������ǰ����
        {
            if (time >= beatTime)  //��ǰ����ʱ�䵽�������ʱ��ʱ���㣬
            {                      //������������������׼�������½���
                time = 0;
                beat++;
            }
            else  //��ǰ����ʱ��δ�ﵽ��ʱ��ʱ��
            {
                time++;   //�ۼ�ʱ�����
                if (time == soundTime)  //���﷢��ʱ���ָʾ�رշ�������
                {                       //����0.25*��ʱ��ľ��������
                    enable = 0;         //����������������������
                }
            }
        }
    }
}
/*���Ʒ��������� */
void InterruptTimer0() interrupt 1
{
    TH0 = T0RH;   //���¼�������ֵ
    TL0 = T0RL;
    tmrflag = 1;
    if (enable)   //ʹ��ʱ��ת���������Ƶ�ƽ
        BUZZ = ~BUZZ;
    else          //δʹ��ʱ�رշ�����
        BUZZ = 1;
}
/*����ɨ��*/
void InterruptTimer1() interrupt 3 //�������ֲο�֮ǰ������ ���мƴι��ܵ����
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
