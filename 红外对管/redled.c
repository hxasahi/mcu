#include<reg52.h>
sbit receive=P2^0;
sbit fm=P2^1;
void main()
{
    fm=0;
    while(1)
	{
        if (receive==0)
	    {
	        P1=0xfC;
			fm=1;
	    }
		else if (receive==1)
		{
		    fm=0;
		    P1=0xf3;
		}
	}
}