#include "SegmentLCD.h"

char dispnum[6]={0x00,0x00,0x00,0x00,0x00,0x00};

// Truth table order: AFEHBGCD
/*0,1,2,3,4,5,6,7,8,9,*/	  
const char num[]={0xeb,0x0a,0xad,0x8f,0x4e,0xc7,0xe7,0x8a,0xef,0xcf}; 


//**********************************************************
// Send data
//**********************************************************
void SendBit_1621(unsigned char sdat,unsigned char cnt) //data 的高cnt 位写入HT1621，高位在前 

{ 
	unsigned char i; 
	for(i=0;i<cnt;i++) 
	{ 
		LCD_WR0;
		delayMicroseconds(20); 
		if(sdat&0x80)
		{
			LCD_DATA1; 
		}
		else 
		{
			LCD_DATA0; 
		}
		delayMicroseconds(20); 
		LCD_WR1;
		delayMicroseconds(20); 
		sdat<<=1; 
	} 
	delayMicroseconds(20);  
}




//**********************************************************
// Send command
//**********************************************************
void SendCmd_1621(unsigned char command)			// Send command 

{ 
	LCD_CS0; 
	SendBit_1621(0x80,4);    //写入标志码“100”和9 位command 命令，由于 
	SendBit_1621(command,8); //没有使有到更改时钟输出等命令，为了编程方便 
	LCD_CS1;                     //直接将command 的最高位写“0” 
} 

//**********************************************************
//发送指令和数据
//**********************************************************
void Write_1621(unsigned char addr,unsigned char sdat) 
{ 
	addr<<=2; 
	LCD_CS0; 
	SendBit_1621(0xa0,3);     //写入标志码“101” 
	SendBit_1621(addr,6);     //写入addr 的高6位 
	SendBit_1621(sdat,8);    //写入data 的8位 
	LCD_CS1; 
}

//**********************************************************
// Write a value to a specific position
//**********************************************************
void Write_1621_data(unsigned char num,unsigned char sdat) 

{ 
	if(num < 6)
	{
		Write_1621(2*num,sdat);
	}
}

//**********************************************************
// Clear display
//**********************************************************
void HT1621_all_off(unsigned char num) 

{ 
	unsigned char i; 
	unsigned char addr=0; 
	for(i=0;i<num;i++) 
	{ 
		Write_1621(addr,0x00); 
		addr+=2; 
	} 
}

//**********************************************************
// All on
//**********************************************************
void HT1621_all_on(unsigned char num) 

{ 
	unsigned char i; 
	unsigned char addr=0; 
	for(i=0;i<num;i++) 
	{ 
		Write_1621(addr,0xff); 
		addr+=2; 
	} 
}

//**********************************************************
// All on, display the same number
//**********************************************************
void HT1621_all_on_num(unsigned char num,unsigned char xx) 

{ 
	unsigned char i; 
	unsigned char addr=0; 
	for(i=0;i<num;i++) 
	{ 
		Write_1621(addr,xx); 
		addr+=2; 
	} 
} 

//**********************************************************
// Initialize 1621
//**********************************************************
void Init_1621(void) 

{
	pinMode(LCD_DATA, OUTPUT); 
	pinMode(LCD_WR, OUTPUT); 
	pinMode(LCD_CS, OUTPUT); 
	LCDoff();
	SendCmd_1621(Sys_en);
	SendCmd_1621(RCosc);    
	SendCmd_1621(ComMode);  
	LCDon();
}

//**********************************************************
// LCD off
//**********************************************************
void LCDoff(void) 

{  
	SendCmd_1621(LCD_off);  
} 

//**********************************************************
// LCD on
//**********************************************************
void LCDon(void) 

{  
	SendCmd_1621(LCD_on);  
}


//**********************************************************
// Test the segments of the second digit, for writing the num array
//**********************************************************
void Displaybihua(void) 

{ 	
	Write_1621(2,0x01);
	delay(350) ;
	Write_1621(2,0x02);
	delay(350) ;
	Write_1621(2,0x04);
	delay(350) ;
	Write_1621(2,0x08);
	delay(350) ;
	Write_1621(2,0X10);
	delay(350) ;
	Write_1621(2,0x20);
	delay(350) ;
	Write_1621(2,0x40);
	delay(350) ;
	Write_1621(2,0x80);
	delay(350) ;
}

//**********************************************************
// Display all 8 digits + decimal point
//**********************************************************	
void Displayall8(void)

{
	unsigned char i;
	HT1621_all_off(6);
	for(i=0;i<6;i++)
	{
		Write_1621(2*i,0xff);
		delay(50) ;
	}
}

//**********************************************************
// Display array data
//**********************************************************
//void Displaydata(long int t,int p,char bat1,char bat2,char bat3)//屏显示，bat1，bat2，bat3 右侧电池

//{
//	unsigned char i;
//	dispnum[5]=num[t/100000];
//	dispnum[4]=num[(t/10000)%10];
//	dispnum[3]=num[(t/1000)%10];
//	dispnum[2]=num[(t/100)%10];
//	dispnum[1]=num[(t/10)%10];
//	dispnum[0]=num[t%10];
//	switch(p)
//	{
//		case 1:
//		     sbi(dispnum[0], 7);
//		     break;
//		case 2:
//		     sbi(dispnum[1], 7);
//		     break;
//		case 3:
//		     sbi(dispnum[2], 7);
//		     break;
//		default:
//			cbi(dispnum[0], 7);
//			cbi(dispnum[1], 7);
//			cbi(dispnum[2], 7);
//		break;
//	}
//	if(bat1==1)  
//		sbi(dispnum[3], 7);
//	else
//		cbi(dispnum[3], 7);
//	if(bat2==1)  
//		sbi(dispnum[4], 7);
//	else
//		cbi(dispnum[4], 7);
//	if(bat3==1)  
//		sbi(dispnum[5], 7);
//	else
//		cbi(dispnum[5], 7);
//	
//	for(i=0;i<6;i++) 
//	{
//		Write_1621(i*2,dispnum[i]);
//	}
//}       



