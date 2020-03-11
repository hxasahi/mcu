#include<reg52.h>

sbit k1 = P3^4;
sbit k2 = P3^5;
sbit jd = P2^0;
void main()
{
    while(1){
    if (k1==0){
	    jd = 1;
	    }
	else if (k2==0){
	    jd = 0;
    	}
	}
}      