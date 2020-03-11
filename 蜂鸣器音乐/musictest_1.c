/*
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
#include <reg52.h>
#define Xtal  11059200  //������ֵ



sbit BUZZ = P1^3;  //��������������

unsigned char keybuf = 0xFF;
unsigned char keybuf4 = 0xFF;
unsigned char keybuf12 = 0xFF;
unsigned char sizeofNote;//���ĵĶ���
               
extern void Delay_ms(unsigned int ms);//��ʱ����

void Play();//���ຯ��

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
