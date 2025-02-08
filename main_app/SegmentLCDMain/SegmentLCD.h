#ifndef __SEGMENTLCD__H__
#define __SEGMENTLCD__H__

#include <Arduino.h>

// HT1621 command definitions
#define  ComMode    0x52  // 4COM, 1/3 bias  1000 010 1001 0  
#define  RCosc      0x30  // Internal RC oscillator (default on power up) 1000 0011 0000 
#define  LCD_on     0x06  // Turn on LCD bias generator 1000 0000 0 11 0 
#define  LCD_off    0x04  // Turn off LCD display 
#define  Sys_en     0x02  // Enable system oscillator 1000 0000 0010 
#define  CTRl_cmd   0x80  // Write control command 
#define  Data_cmd   0xa0  // Write data command 

// Bit manipulation macros
#define sbi(x, y)  (x |= (1 << y))   // Set bit y in register x
#define cbi(x, y)  (x &= ~(1 <<y ))  // Clear bit y in register x  

// IO pin definitions
#define LCD_DATA 9
#define LCD_WR 8
#define LCD_CS 7

// HT1621 data port definitions
#define LCD_DATA1    digitalWrite(LCD_DATA,HIGH) 
#define LCD_DATA0    digitalWrite(LCD_DATA,LOW) 
#define LCD_WR1      digitalWrite(LCD_WR,HIGH)  
#define LCD_WR0      digitalWrite(LCD_WR,LOW)   
#define LCD_CS1      digitalWrite(LCD_CS,HIGH)  
#define LCD_CS0      digitalWrite(LCD_CS,LOW)

// Function declarations
extern void SendBit_1621(unsigned char sdat,unsigned char cnt); // Write high cnt bits of data to HT1621, MSB first
extern void SendCmd_1621(unsigned char command);            // Send command
extern void Write_1621(unsigned char addr,unsigned char sdat);
extern void HT1621_all_off(unsigned char num);
extern void HT1621_all_on(unsigned char num);
extern void HT1621_all_on_num(unsigned char num,unsigned char xx);
extern void LCDoff(void);
extern void LCDon(void);
extern void Displaybihua(void);
extern void Init_1621(void);
extern void Displayall8(void);
//extern void Displaydata(long int t,int p,char bat1,char bat2,char bat3);    // Display on screen, bat1, bat2, bat3 for right side battery
extern void Write_1621_data(unsigned char num,unsigned char sdat) ;

// External variables
extern char dispnum[6];
extern const char num[]; 

#endif