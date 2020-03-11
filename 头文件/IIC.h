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

void IIC_Ack(unsigned char ackbit)  //当ackbit为1时，表示单片机对从设备发送来数据的应答
{ //当ackbit为0时，表示主机接收了最后一个字节，因此不再应答，结束通信
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
    if(SDA)  //在SCL为高电平期间，因为接收设备未将SDA拉低，所以默认未接收到应答，结束IIC通信
    { 
        SCL = 0;
        IIC_Stop();
        return 0;
    }
    else  //接收到应答，返回1，继续下一个数据字节的传输
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
	IIC_Start();	           //写入芯片地址
	IIC_SendByte(0x90);	 
	IIC_WaitAck();	 
 	IIC_SendByte(0x01);	   //写入选择的通道，本程序只用单端输入，差分部分需要自行添加
	IIC_WaitAck();	                 
	IIC_Stop();	  
	
	IIC_Start();	  
	IIC_SendByte(0x91);     //读入地址
	IIC_WaitAck();
	Data = IIC_RecByte();   //读数据
	IIC_Ack(0);
	IIC_Stop();
	return Data;	  
}

