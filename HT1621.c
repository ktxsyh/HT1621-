//For HT1621 
//date:2017:6:23

#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int

#define HT1621_CS_H CS = 1
#define HT1621_CS_L CS = 0
#define HT1621_RD_H RD = 1
#define HT1621_RD_L RD = 0
#define HT1621_WR_H WR = 1
#define HT1621_WR_L WR = 0
#define Nop()  _nop_(); _nop_ ();_nop_ ();_nop_ ();_nop_ ()

#define Bias 0x52 //Bias 
//#define Bias 0x51
#define SYSDIS  0X00 //Turn off both system oscillator and LCD bias generator  
#define SYSEN  0X01   //Turn on system oscillator 
#define LCDOFF   0X02  //Turn off LCD bias generator 
#define LCDON   0X03 //Turn on LCD bias generator 
#define TIMERDIS   0x04  //Disable time base 
#define WDTDIS   0x05  //Disable WDT time 
#define TIMEREN   0X06  //Enable time base 
#define WDTEN  0x07  //Enable WDT time_out flag output
#define TONEOFF   0X08   //Trun off tone outputs
#define TONEON   0X09  //Turn on tone outputs
#define XTAL32K 0X14  //32.768K HZ oscillator 
#define RC256K  0X18   //System clock source 256K HZ
#define EXT256K  0X1C   //System clock source form external source 

//sbit Flag = P2^0;
sbit CS = P2^1;
//sbit RD = P2^2;
//sbit WR = P2^3;
sbit HT1621_DATA = P2^4;

uchar code HT1621_Tab[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/***************Delay us ************/
//void Delay_us (uchar t)
//{
//	while (--t);
//}

/***************Delay ms ************/
void Delay_ms (uchar tt)
{
	uint i,j;
	for (i=0;i<tt;i++)
		for (j=0;j<65;j++)
			;
}

/**************Write Data**************/
void HT1621Wr_Data (uchar Data,uchar Cnt)
{
	uint i;
	for (i=0;i<Cnt;i++)
	{
		HT1621_WR_L;
		Nop ();
		HT1621_DATA = HT1621_DATA & 0X80;
		Nop ();
		HT1621_WR_H;
		Nop ();
		Data <<= 1;
	}
}

/***************Write Command ************/
void HT1621Wr_Command (uchar cmd)
{
	HT1621_CS_L;
	Nop ();
	HT1621Wr_Data (0x80,4);
	HT1621Wr_Data(cmd,8);
	Nop();
	HT1621_CS_H;
	Nop();
}

/************Write Data to address***********/
void HT1621WrOneData (uchar Addr,uchar Data)
{
	HT1621_CS_L;
	HT1621Wr_Data (0xa0,3);
	HT1621Wr_Data (Addr<<2,6);
	HT1621Wr_Data (Data<<4,4);
	HT1621_CS_H;
	Nop ();
}

/************Write Test ****************/
void  HT1621WrAll_Data (uchar Addr ,uchar *p,uchar Cnt)
{
	uchar i;
	HT1621_CS_L;
	HT1621Wr_Data (0xa0,3);
	HT1621Wr_Data (Addr<<2,6);
	for (i=0;i<Cnt;i++)
	{
		HT1621Wr_Data (*p,8);
		p++;
	}	
	HT1621_CS_H;
	Nop ();
}

/***********TH1621 Init ****************/
void HT1621_Init ()
{
	HT1621_CS_H;
	HT1621_WR_H;
	HT1621_DATA =1;
	Delay_ms (2000);
	HT1621Wr_Command (Bias);
	HT1621Wr_Command (RC256K);
	HT1621Wr_Command (SYSDIS);
	HT1621Wr_Command (WDTDIS);
	HT1621Wr_Command (SYSEN);
	HT1621Wr_Command (LCDON);
	Nop();
}

/**********************MAIN**************/
void main ()
{
	uchar i,j,t;
	HT1621_Init ();
	Delay_ms (1000);
	while (1)
	{
		HT1621WrAll_Data (0,HT1621_Tab,16);
		for (i=0;i<32;i++)
		{
			t = 0x01;
			for (j=0;j<4;j++)
			{
				HT1621Wr_Data (i,t);
				t<<=1;
				t++;
				//Flag = ~Flag;
				Delay_ms (5000);
			}
		}
		
	}
}













