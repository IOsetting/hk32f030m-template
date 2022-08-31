
/**
  ******************************************************************************
  * File Name		   : define.h
  * Description 	   : Main program body
  ******************************************************************************
*/

#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <stdio.h>    //标准输入输出头文件


typedef volatile unsigned char  		byte;
typedef volatile unsigned char  		BOOL;
typedef volatile unsigned char  		u8int;
typedef volatile unsigned short int 		u16int;
typedef volatile unsigned long int 		u32int;

#define FALSE	0
#define TRUE	1

#define BIT0	0x01
#define BIT1	0x02
#define BIT2	0x04
#define BIT3	0x08
#define BIT4	0x10
#define BIT5	0x20
#define BIT6	0x40
#define BIT7	0x80
#define BIT8	0x0100
#define BIT9	0x0200
#define BIT10	0x0400

typedef enum
{
	ONOFF_OFF,
	ONOFF_ON,
}ONOFF_TYPE;

#endif

