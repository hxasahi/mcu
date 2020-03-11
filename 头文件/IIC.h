sbit SDA = P2^0;
sbit SCL = P2^1;
/*void           *    IIC_Start(void)
  void           *    IIC_Stop(void)
  void           *    IIC_Ack(unsigned char ackbit)
  bit            *    IIC_WaitAck(void)
  void           *    IIC_SendByte(unsigned char byt)
  unsigned int   *    IIC_RecByte(void)
  unsigned int   *    ReadADC()
*/
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    _nop_();
    SDA = 0;
    _nop_();
    SCL = 0;
}

void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    _nop_();
    SDA = 1;
}

void IIC_Ack(unsigned char ackbit)  //��ackbitΪ1ʱ����ʾ��Ƭ���Դ��豸���������ݵ�Ӧ��
{ //��ackbitΪ0ʱ����ʾ�������������һ���ֽڣ���˲���Ӧ�𣬽���ͨ��
    if(ackbit)
    {
        SDA = 0;
    }
    else
    {
        SDA = 1;
    }
    _nop_();
    SCL = 1;
    _nop_();
    SCL = 0;
    SDA = 1;
    _nop_();
}

bit IIC_WaitAck(void)
{
    SDA = 1;
    _nop_();
    SCL = 1;
    _nop_();
    if(SDA)  //��SCLΪ�ߵ�ƽ�ڼ䣬��Ϊ�����豸δ��SDA���ͣ�����Ĭ��δ���յ�Ӧ�𣬽���IICͨ��
    { 
        SCL = 0;
        IIC_Stop();
        return 0;
    }
    else  //���յ�Ӧ�𣬷���1��������һ�������ֽڵĴ���
    {
        SCL = 0;
        return 1;
    }

}

void IIC_SendByte(unsigned char byt)
{
    unsigned char i;
    for(i=0;i<8;i++)
    { 
        if(byt&0x80)
        {
            SDA = 1;
        }
        else
        {
            SDA = 0;
        }
        _nop_();
        SCL = 1;
        byt <<= 1;
        _nop_();
        SCL = 0;
    }
}

unsigned int IIC_RecByte(void)
{
    unsigned int da;
    unsigned char i;

    for(i=0;i<8;i++)
    { 
        SCL = 1;
        _nop_();
        da <<= 1;
        if(SDA)
        da |= 0x01;
        SCL = 0;
        _nop_();
    }

    return da;
}

unsigned int ReadADC()//ReadADC(unsigned char Chl)
{
    unsigned int Data;
	IIC_Start();	           //д��оƬ��ַ
	IIC_SendByte(0x90);	 
	IIC_WaitAck();	 
 	IIC_SendByte(0x01);	   //д��ѡ���ͨ����������ֻ�õ������룬��ֲ�����Ҫ�������
	IIC_WaitAck();	                 
	IIC_Stop();	  
	
	IIC_Start();	  
	IIC_SendByte(0x91);     //�����ַ
	IIC_WaitAck();
	Data = IIC_RecByte();   //������
	IIC_Ack(0);
	IIC_Stop();
	return Data;	  
}

