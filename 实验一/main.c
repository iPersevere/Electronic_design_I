//#include <reg52.h>			 
#include <REGX52.H>

typedef unsigned int u16;	  
typedef unsigned char u8;

u8 code smgwei0[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe,0xff};
u8 code smgduan0[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07};//显示0~7的值
					
u8 code smgwei1[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,0xff};
u8 code smgduan1[]={0x07,0x7d,0x6d,0x66,0x4f,0x5b,0x06,0x3f};//显示0~7的值

u16 code copy[]={0xff,0x7f,0x3f,0x1f,0x0f,0x07,0x03,0x01}; 
u8 i,j,k,t;
bit ldelay=0;
void delay(u16 i)       //延时函数，一个1相当于10us
{
	while(i--);	
}
void Display1()
{
    EA=1;
	EX0=1;
	IT0=1;
}
void Int0()interrupt 0
{
    P0=0xff;
    P2=0xff;
	delay(10000);
    for(i=0;i<=8;i++)
	{
		P0=smgwei1[i];
		if(i!=8)
		delay(10000); //间隔一段时间扫描	
	}

	for(i=0;i<=8;i++)
	{
		P2=smgwei1[i];
		if(i!=8)
		delay(10000); //间隔一段时间扫描	
	}
	for(i=0;i<=8;i++)
	{
		P2=smgwei0[i];
		if(i!=8)
		delay(10000); //间隔一段时间扫描	
	}
	for(i=0;i<=8;i++)
	{
		P0=smgwei0[i];
		if(i!=8)
		delay(10000); //间隔一段时间扫描	
	}

    P0=0xff;
    P2=0xff;
	delay(10000);
    for(i=0;i<8;i++)
	{
	    P2=smgwei0[i];
		P0=smgwei1[i];		
		if(i!=8)
		delay(10000); //间隔一段时间扫描	
	}
	for(i=0;i<8;i++)
	{
	    P2=smgwei1[i];
		P0=smgwei0[i];		
		if(i!=8)
		delay(10000); //间隔一段时间扫描	
	}
}
void Display2()
{
    EA=1;
	EX1=1;
	IT1=1;
}
void Int1()interrupt 2
{    
     u8 m=8,n=8;
	 i=0;
	 P0=0xff;
     P2=0xff;
	 delay(10000);
	 while(i<8)
	 {	    
	    u16 temp=0xfe&copy[i];
		for(k=0;k<=8;k++)
	    {
		    P0=smgwei1[k];
		    if(k!=8)
		    delay(10000); //间隔一段时间扫描	
	    }
        for(j=0;j<m;j++)
        {
		    
		    P2=temp;
            temp=((temp<<1)|0x01)&copy[i];
	    	delay(10000);			        
        }
        m--;
	    i++;
    }
	P2=0;
	i=0;
	while(i<8)
    {
	    u16 temp=0xfe&copy[i];
        for(j=0;j<n;j++)
        {
		    P0=temp;
            temp=((temp<<1)|0x01)&copy[i];
	    	delay(10000);			        
        }
        n--;
	    i++;
    }
}
void timer0()interrupt 1
{
    TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	t++;
	if(t==5)
	{
	   t=0;
	   ldelay=1;
	}
}
void main()
{
    TMOD=0x01;
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
	EA=1;
	TR0=1;
	ET0=1;
	IP=0x05;
	while(1)
	{
	    if(ldelay)
		{
		    ldelay=0; 
			P2=smgwei0[i];
		    P0=smgduan0[i];//发送段码
			i++;
			if(i>=8)
			{
			   P2=smgwei1[i-8];
		       P0=smgduan1[i-8];//发送段码
			}
			if(i==16)i=0;

		}
		Display1();	   //LED流水灯显示,LED左右同时显示
	    Display2();	   //LED加法灯显示
	}		
}

