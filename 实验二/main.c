#include<DS18B20.h>
#include<ds1302.h>
#include <AT89X52.H>

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

u8 READ[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
u8 WRITE[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

sbit LSA=P2^0;
sbit LSB=P2^1;
sbit LSC=P2^2;
sbit LSD=P2^3;
sbit LSE=P2^4;
sbit LSF=P2^5;
sbit LSG=P2^6;
sbit LSH=P2^7;
sbit k1=P3^2; 
sbit k2=P3^3; 
sbit k3=P3^4; 
sbit k4=P3^5; 
sbit beep=P1^4;

u16 hour,minute,second; 
u16 i,a,temp;  
u8 F_k1=0,F_k2=0;
u8 ahour=12,aminute=0,asecond=20; 

char num=0;
u8 DisplayData[8];
u8 code smgduan[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};

void delay(u16 i)	//延时函数，i=1时，大约延时10us
{
	while(i--);	
}
void datapros0() 	 //时间读取处理转换函数
{
   	Ds1302ReadTime();	
	DisplayData[2] = smgduan[TIME[2]/16];				//时
	DisplayData[3] = smgduan[TIME[2]&0x0f]|0x80;				 
	DisplayData[4] = smgduan[TIME[1]/16];				//分
	DisplayData[5] = smgduan[TIME[1]&0x0f]|0x80;	
	DisplayData[6] = smgduan[TIME[0]/16];				//秒
	DisplayData[7] = smgduan[TIME[0]&0x0f];
	hour=TIME[2];
	minute=TIME[1];
	second=TIME[0];	
}
void datapros1(u16 temp) 	//温度读取处理转换函数
{
   	float tp;  
	if(temp< 0)				//当温度值为负数,因为读取的温度是实际温度的补码，所以减1，再取反求出原码
  	{
		temp=temp-1;
		temp=~temp;
		tp=temp;		   //因为数据处理有小数点所以将温度赋给一个浮点型变量
		temp=tp*0.0625+0.5;	//+0.5是四舍五入
  	}
 	else				   //当温度是正的，那么正数的原码就是补码它本身
  	{			
		tp=temp;
		temp=tp*0.0625+0.5;	
	}
	if(temp>=25)
	{
		beep=1;
		delay(10000000);
	}  

	DisplayData[0] = smgduan[temp / 10];
	DisplayData[1] = smgduan[temp % 10];	
}
void DigDisplay()	 //数码管显示函数
{
	u8 i;
	for(i=0;i<8;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
		    case(0):
				LSA=0;LSB=1;LSC=1;LSD=1;LSE=1;LSF=1;LSG=1;LSH=1; break;//显示第0位
			case(1):
				LSA=1;LSB=0;LSC=1;LSD=1;LSE=1;LSF=1;LSG=1;LSH=1; break;//显示第1位
			case(2):
				LSA=1;LSB=1;LSC=0; LSD=1;LSE=1;LSF=1;LSG=1;LSH=1;break;//显示第2位
			case(3):
				LSA=1;LSB=1;LSC=1; LSD=0;LSE=1;LSF=1;LSG=1;LSH=1;break;//显示第3位
			case(4):
				LSA=1;LSB=1;LSC=1; LSD=1;LSE=0;LSF=1;LSG=1;LSH=1;break;//显示第4位
			case(5):
				LSA=1;LSB=1;LSC=1; LSD=1;LSE=1;LSF=0;LSG=1;LSH=1;break;//显示第5位
			case(6):
				LSA=1;LSB=1;LSC=1; LSD=1;LSE=1;LSF=1;LSG=0;LSH=1;break;//显示第6位
			case(7):
				LSA=1;LSB=1;LSC=1; LSD=1;LSE=1;LSF=1;LSG=1;LSH=0;break;//显示第7位
		}
		if(i==0||i==1)
		{
		    P0=DisplayData[1-i];//发送数据
		    delay(100); //间隔一段时间扫描	
		    P0=0x00;//消隐
		}
		else
		{
		    P0=DisplayData[9-i];//发送数据
		    delay(100); //间隔一段时间扫描	
		    P0=0x00;//消隐
		}
	}		
}
void key1()  
{    
    if(k1==0)   
    {    
        delay(1);    
        if(k1==0)     
        {      
             while(!k1);      
             F_k1++;      
             if(F_k1==4)       
             {        
                 F_k1=0;         
             }     
        }   
    } 
}
void key2()  
{    
    if(k2==0)   
    {    
        delay(1);    
        if(k2==0)     
        {          
             while(!k2);      
             F_k2++;      
             if(F_k2==4)       
             {        
                 F_k2=0;              
             }     
        }   
    } 
}
void key3() 
{  
    if(k3==0)   
    {   
        delay(1);   
        {    
            while(!k3);    
            if(F_k1==1)   
            { 			     
                second++;    
                if(second==60)    
                second=0;
            }   
            if(F_k1==2)    
            {
                minute++;     
                if(minute==60)     
                minute=0;				       
            }   
            if(F_k1==3)    
            {   
                hour++; 
                if(hour==24)     
                hour=0;
            }     
            if(F_k2==1)    
            {     
                asecond++;     
                if(asecond==60)     
                asecond=0;    
            }    
            if(F_k2==2)    
            {     
                aminute++;     
                if(aminute==60)     
                aminute=0;    
            }    
            if(F_k2==3)    
            {     
                ahour++;     
                if(ahour==24)     
                ahour=0;    
            } 
         }  
     } 
}
void key4() 
{  
    if(k4==0)   
    {   
        delay(1);   
        {    
            while(!k4);    
            if(F_k1==1)   
            {   
                second--;    
                if(second==0)    
                second=59;      
            }   
            if(F_k1==2)    
            {     
                minute--;     
                if(minute==0)     
                minute=59;       
            }   
            if(F_k1==3)    
            {    
                hour--; 
                if(hour==0)     
                hour=23;    
            }     
            if(F_k2==1)    
            {     
                asecond--;     
                if(asecond==0)     
                asecond=59;    
            }    
            if(F_k2==2)    
            {     
                aminute--;     
                if(aminute==0)     
                aminute=59;    
            }    
            if(F_k2==3)    
            {     
                ahour--;     
                if(ahour==0)     
                ahour=23;    
            } 
         }  
     } 
}
void main()
{
    u16 n;
    Ds1302Init();
    while(1)
	{  
	    u16 h=0,m=0,s=0;  
	    beep=0;
	    datapros0();	 //数据处理函数
		datapros1(Ds18b20ReadTemp());	 //数据处理函数
		second=(second&0x0f)+(second>>4)*10;  //将秒、分、时的BCD码转化为十进制
		minute=(minute&0x0f)+(minute>>4)*10;
		hour=(hour&0x0f)+(hour>>4)*10;
		key1();
		key2();
		key3();
		key4();
		second=((second/10)<<4)+(second%10);  //将秒、分、时的十进制转化为BCD码
		minute=((minute/10)<<4)+(minute%10);
		hour=((hour/10)<<4)+(hour%10);
		if(F_k1)
		{
		    Ds1302Write(0x80,0x80);//使秒寄存器最高位为1，终止计时
		    TIME[2]=hour;
	        TIME[1]=minute;
	        TIME[0]=second;
	        Ds1302Write(0x8E,0X00);		 //禁止写保护，就是关闭写保护功能
	        for (n=0; n<7; n++)//写入7个字节的时钟信号：分秒时日月周年
	        {
		        Ds1302Write(WRITE[n],TIME[n]);	
	        }
	        Ds1302Write(0x8E,0x80);		 //打开写保护功能
		    for (n=0; n<7; n++)//读取7个字节的时钟信号：分秒时日月周年
	        {
		        TIME[n] = Ds1302Read(READ[n]);
	        } 
		    hour=TIME[2];
 	        minute=TIME[1];
	        second=TIME[0];
		}
		if(asecond>=0&&asecond<=9){if(second==asecond)s=1;}
		if(asecond>=10&&asecond<=19){if(second==asecond+6)s=1;}
		if(asecond>=20&&asecond<=29){if(second==asecond+12)s=1;}
		if(asecond>=30&&asecond<=39){if(second==asecond+18)s=1;}
		if(asecond>=40&&asecond<=49){if(second==asecond+24)s=1;}
		if(asecond>=50&&asecond<=59){if(second==asecond+30)s=1;}
		/*if(aminute>=0&&aminute<=9){if(aminute==aminute)m=1;}
		if(aminute>=10&&aminute<=19){if(aminute==aminute+6)m=1;}
		if(aminute>=20&&aminute<=29){if(aminute==aminute+12)m=1;}
		if(aminute>=30&&aminute<=39){if(aminute==aminute+18)m=1;}
		if(aminute>=40&&aminute<=49){if(aminute==aminute+24)m=1;}
		if(aminute>=50&&aminute<=59){if(aminute==aminute+30)m=1;}
		if(ahour>=0&&ahour<=9){if(ahour==ahour)h=1;}
		if(ahour>=10&&ahour<=19){if(ahour==ahour+6)h=1;}
		if(ahour>=20&&ahour<=24){if(ahour==ahour+12)h=1;}
		if(s==1&&m==1&&h==1)
		{
		    beep=1;
			delay(10000000);
		}  */
		if(s)	 //蜂鸣器响应函数
		{
		    beep=1;
			delay(10000000);
		}  
		if(F_k1)	//时间调整时显示时间
		{
		   DisplayData[2] = smgduan[hour/16];				//时
	       DisplayData[3] = smgduan[hour&0x0f]|0x80;				 
	       DisplayData[4] = smgduan[minute/16];				//分
	       DisplayData[5] = smgduan[minute&0x0f]|0x80;	
	       DisplayData[6] = smgduan[second/16];				//秒
	       DisplayData[7] = smgduan[second&0x0f];
		}						
		if(F_k2) 	//定时调整时显示定时
		{

		   DisplayData[2] = smgduan[ahour/10];				//时
	       DisplayData[3] = smgduan[ahour%10]|0x80;				 
	       DisplayData[4] = smgduan[aminute/10];			//分
	       DisplayData[5] = smgduan[aminute%10]|0x80;	
	       DisplayData[6] = smgduan[asecond/10];			//秒
	       DisplayData[7] = smgduan[asecond%10];		   
		} 
		DigDisplay();//数码管显示函数
	}
}	 
