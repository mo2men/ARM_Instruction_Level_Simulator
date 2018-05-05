#include <stdio.h>
#include "shell.h"
#define threebytes 0x00ffff
#define  offsetcheck 0xfe0000
#define	 full 0xffffffff
void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
	 NEXT_STATE.REGS[15]=CURRENT_STATE.REGS[15]+4;
	 uint32_t ins=mem_read_32(CURRENT_STATE.REGS[15]);
	 uint32_t cond=((ins>>28)&15); //condition on instruction
	 uint32_t check_cond= (((CURRENT_STATE.CPSR)>>28)&15);	 // flags
	 uint32_t check_ins= ((ins>>26)&3);
	 uint32_t check_ins_SWI=((ins>>24)&3);
	 uint32_t check_ins_Branch=((ins>>24)&3);
	 uint32_t check_ins_mul=((ins>>21)&31);
	 uint32_t check_ins_muli=((ins>>4)&15);
	 uint32_t check_ins_DT=((ins>>20)&37); //load/store byte/word immediate, bit 20 22 & 25
	 uint32_t base=((ins>>16)&15);
		uint32_t target=((ins>>12)&15);
		uint32_t dataoffset=ins&0x00000fff;
		uint32_t trueoffset;
		uint32_t address;
		uint32_t x;
		uint32_t w;
		uint32_t r;
		uint64_t ww;
		uint32_t opcode=(ins>>21)&15;
		uint32_t imm=(ins>>25)&1;
		uint32_t set=(ins>>20)&1;
	 int batee5a=1; //compiler r5m
	 switch (cond) 
	 { case 0 :
		if ((check_cond&4)==4)
	 
			break;
	 
		else
	
			goto end;
//==== mo'men ====== never use goto in high level language ===== //				
//==== mo'men ====== if you use goto more than once your code will be spakety ===== //				

	 case 1:
		if ((check_cond&4)==4)
	 
		 
			goto end;
		else
			
			break;
 
	case 2:	
			if ((check_cond&2)==2)
			 break;
	 
			else
	
			goto end;
	case 3:		
			if ((check_cond&2)==2)
			 
				goto end;
			else
			
				break;
			
	case 4:	 
	if ((check_cond&8)==8)
			 
				break;
		else
			
				goto end;
	
   case 5:
   if ((check_cond&8)==8)
			 
				goto end;
		else
			
				break;
	case 6:
	if ((check_cond&1)==1)
			 
				break;
		else
			
				goto end;
	
  case 7:
  
   if ((check_cond&8)==8)
			 
				goto end;
		else
			
				break;
	case 8:
	if (((check_cond&4)==0)&&((check_cond&2)==2))
			 
				break;
		else
			
				goto end;

  case 9:
  if (((check_cond&4)==4)||((check_cond&2)==0))
			 
				break;
		else
			
				goto end;

case 10:
  if ((((check_cond>>3)&1))==(check_cond&1))
			 
				break;
		else
			
				goto end;

 case 11:
  if (((check_cond>>3)&1)!=(check_cond&1))
			 
				break;
		else
			
				goto end;
	
case 12:
  if (((check_cond&4)==0)&&(((check_cond>>3)&1)==(check_cond&1)))
			 
				break;
		else
			
				goto end;	


case 13:
 if (((check_cond&4)==4)||(((check_cond>>3)&1)!=(check_cond&1)))
	 
				break;

		else
			
				goto end;

default :

 break;

}
 
 
	 switch (check_ins)
	 {
		case 0:
		batee5a=3;
		uint32_t op1=(ins>>16)&15;
		uint32_t op2=ins&0x00000fff;
		uint32_t des=(ins>>12)&15;
		uint32_t operand2;
		if (0==imm)
		{
			uint32_t op2reg=CURRENT_STATE.REGS[(op2&15)];
			if (0==((op2>>4)&1))
			{
			if (0==((op2>>5)&3))
			{uint32_t b=(op2>>7)&31;
			
			operand2=op2reg<<b;
				
			}
			else if((1==((op2>>5)&3))||(2==((op2>>5)&3)))
			{uint32_t b=(op2>>7)&31;
			operand2=op2reg>>b;
				
			}
			else if (3==((op2>>5)&3))
			{
				uint32_t b=(op2>>7)&31;
		
			int a=0;
			for (a=0;a<b;a++)
			{
				op2reg=((op2reg&1)<<31)+((op2reg>>1)&0x7fffffff);
			}
			operand2=op2reg;
			}
			
		}
			else if (1==((op2>>4)&1))
			{
				if (0==((op2>>5)&3))
			{ uint32_t q=CURRENT_STATE.REGS[((op2>>7)&31)]&255;
			operand2=op2reg<<q;
				
			}
			else if((1==((op2>>5)&3))||(2==((op2>>5)&3)))
			{uint32_t q=CURRENT_STATE.REGS[((op2>>7)&31)]&255;
			operand2=op2reg>>q;
					if ((2==((op2>>5)&3))&&(1==(op2>>31)&1))
			{
//==== mo'men ====== incorrect sign checking of operand2 ===== //				

				operand2=(operand2|(full<<(31-q)));
			}
			}
			else if (3==((op2>>5)&3))
			{
				uint32_t q=CURRENT_STATE.REGS[((op2>>7)&31)]&255;
			int a=0;
			for (a=0;a<q;a++)
			{
				op2reg=((op2reg&1)<<31)+((op2reg>>1)&0x7fffffff);
			}
			operand2=op2reg;
		}
			}
		}
		else if (1==imm)
		{uint32_t rot=2*((op2>>8)&15);
		operand2=op2&255;
		int a;
	     for (a=0;a<rot;a++)
		 {
			 operand2=((operand2&1)<<31)+((operand2>>1)&0x7fffffff);
		 }
		}
		switch (opcode)
			{case 0:
			NEXT_STATE.REGS[des]=CURRENT_STATE.REGS[op1]&operand2;
			break;
			case 1:
		
			NEXT_STATE.REGS[des]=(CURRENT_STATE.REGS[op1]&(~operand2));
			break;
			case 2:
			NEXT_STATE.REGS[des]=CURRENT_STATE.REGS[op1]-operand2;
			 w=NEXT_STATE.REGS[des];
			 r=CURRENT_STATE.REGS[op1];
			 ww=w|0x0000000100000000;
			if (1==set)
			{
				if (0==w)
				{
					NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x4fffffff;
				}
				
				else if (r!=(w+operand2))
				{
					NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x2fffffff;
					if (r!=(ww+operand2))
					{
						NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x1fffffff;
					}
					if (r<operand2)
					{
						NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x8fffffff;
					}
				}
				
			}
			break;
			case 3:
			NEXT_STATE.REGS[des]=operand2-CURRENT_STATE.REGS[op1];
			 w=NEXT_STATE.REGS[des];
			 r=CURRENT_STATE.REGS[op1];
			 ww=w|0x0000000100000000;
			if (1==set)
			{
				if (0==w)
				{
					NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x4fffffff;
				}
				
				else if (operand2!=(w+r))
				{
					NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x2fffffff;
					if (operand2!=(ww+r))
					{
						NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x1fffffff;
					}
					if (r<operand2)
					{
						NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x8fffffff;
					}
				}
				
			}
			break;
			case 4:
			NEXT_STATE.REGS[des]=operand2+CURRENT_STATE.REGS[op1];
			 w=NEXT_STATE.REGS[des];
			 r=CURRENT_STATE.REGS[op1];
			 ww=w|0x0000000100000000;
			if (1==set)
			{
				if (0==w)
				{
					NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x4fffffff;
				}
				
				else if (operand2!=(w-r))
				{
					NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x2fffffff;
					if (operand2!=(ww-r))
					{
						NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x1fffffff;
					}
					if (1==(w>>31))
					{
						NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x8fffffff;
					}
				}
				
			}
			break;
			case 5:
			NEXT_STATE.REGS[des]=operand2+CURRENT_STATE.REGS[op1]+((check_cond>>1)&1);
			 w=NEXT_STATE.REGS[des];
			 r=CURRENT_STATE.REGS[op1];
			 ww=w|0x0000000100000000;
			if (1==set)
			{
				if (0==w)
				{
					NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x4fffffff;
				}
				
				else if (operand2!=(w-r-((check_cond>>1)&1)))
				{
					NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x2fffffff;
					if (operand2!=(ww-r-((check_cond>>1)&1)))
					{
						NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x1fffffff;
					}
					if (1==(w>>31))
					{
						NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x8fffffff;
					}
				}
			}	
			break;
			
			}
			if ((0==check_ins_mul)&&(9==check_ins_muli))
	{
		uint32_t Rm=CURRENT_STATE.REGS[(ins&15)];
		uint32_t Rs=CURRENT_STATE.REGS[((ins>>8)&15)];
		uint32_t Rd=(ins>>16)&15;
		
		uint32_t mul_res=Rm*Rs;
		NEXT_STATE.REGS[Rd]=mul_res;
		if (1==set)
		{
			if ((0==Rm)||(0==Rs))
			{
				NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x4fffffff;
			}
			
			else if (Rm!=(mul_res/Rs))
			{
				NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x2fffffff;
				if ((1==(Rm>>31))||(1==(Rs>>31)))
				{
					NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x1fffffff;
				}
				if (1==mul_res>>31)
				{
					NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x8fffffff;
				}
			}
			
		}
	}
		
		else if (1==check_ins_mul&&(9==check_ins_muli))
		{
			uint32_t Rm=CURRENT_STATE.REGS[(ins&15)];
		uint32_t Rs=CURRENT_STATE.REGS[((ins>>8)&15)];
			uint32_t Rd=(ins>>16)&15;
			uint32_t Rn=CURRENT_STATE.REGS[((ins>>12)&15)];
		
		uint32_t mul_res=Rm*Rs+Rn;
		NEXT_STATE.REGS[Rd]=mul_res;
		if (1==set)
		{
			if (0==mul_res)
			{
				NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x4fffffff;
			}
			
			else if (Rm!=((mul_res-Rn)/Rs))
			{
				NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x2fffffff;
				if ((1==(Rm>>31))||(1==(Rs>>31)))
				{
					NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x1fffffff;
				}
				if (1==mul_res>>31)
				{
					NEXT_STATE.CPSR=CURRENT_STATE.CPSR|0x8fffffff;
				}
			}
			
		}
		}
		break;
		case 1:
		//Single Data Transfer
		switch (check_ins_DT)
		{case 0:
		trueoffset=dataoffset;
		if (1==((ins>>23)&1))
		{  address=CURRENT_STATE.REGS[base]-trueoffset;
			
		}
		else{ 
		address=CURRENT_STATE.REGS[base]+trueoffset;
		}
		 x=CURRENT_STATE.REGS[target];
		 mem_write_32(address,x);
		break;
		case 1:
		trueoffset=dataoffset;
		if (1==((ins>>23)&1))
		{  address=CURRENT_STATE.REGS[base]-trueoffset;
			
		}
		else{ 
		address=CURRENT_STATE.REGS[base]+trueoffset;
		}
		NEXT_STATE.REGS[target]=mem_read_32(address);
		break;		
		case 4:
		trueoffset=dataoffset;
		if (1==((ins>>23)&1))
		{  address=CURRENT_STATE.REGS[base]-trueoffset;
			
		}
		else{ 
		address=CURRENT_STATE.REGS[base]+trueoffset;
		}
		 x=(CURRENT_STATE.REGS[target]&0x000000ff)+((CURRENT_STATE.REGS[target]&0x000000ff)<<8)+((CURRENT_STATE.REGS[target]&0x000000ff)<<16)+((CURRENT_STATE.REGS[target]&0x000000ff)<<24);
			mem_write_32(address,x);
			break;
			case 5:
			trueoffset=dataoffset;
			if (1==((ins>>23)&1))
		{  address=CURRENT_STATE.REGS[base]-trueoffset;
			
		}
		else{ 
		address=CURRENT_STATE.REGS[base]+trueoffset;
		}
		NEXT_STATE.REGS[target]=mem_read_32(address)&0x000000ff;
			break;
			case 32:
			if (0==((dataoffset>>4)&1))
			{
			if (0==((dataoffset>>5)&3))
			{uint32_t q=(dataoffset>>7)&31;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			trueoffset=reg<<q;
				
			}
			else if((1==((dataoffset>>5)&3))||(2==((dataoffset>>5)&3)))
			{uint32_t q=(dataoffset>>7)&31;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			
			trueoffset=reg>>q;
			if ((2==((dataoffset>>5)&3))&&(1==(reg>>31)&1))
			{
				trueoffset=(trueoffset|(full<<(31-q)));
			}	
			}
			else if (3==((dataoffset>>5)&3))
			{
				uint32_t q=(dataoffset>>7)&31;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			int a=0;
			for (a=0;a<q;a++)
			{
				reg=((reg&1)<<31)+((reg>>1)&0x7fffffff);
			}
			
				trueoffset=reg;
			}
			}
			else if (1==((dataoffset>>4)&1))
			{
				if (0==((dataoffset>>5)&3))
			{ uint32_t q=CURRENT_STATE.REGS[((dataoffset>>7)&31)]&255;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			trueoffset=reg<<q;
				
			}
			else if((1==((dataoffset>>5)&3))||(2==((dataoffset>>5)&3)))
			{uint32_t q=CURRENT_STATE.REGS[((dataoffset>>7)&31)]&255;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			trueoffset=reg>>q;
					if ((2==((dataoffset>>5)&3))&&(1==(reg>>31)&1))
			{
				trueoffset=(trueoffset|(full<<(31-q)));
			}
			}
			else if (3==((dataoffset>>5)&3))
			{
				uint32_t q=CURRENT_STATE.REGS[((dataoffset>>7)&31)]&255;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			int a=0;
			for (a=0;a<q;a++)
			{
				reg=((reg&1)<<31)+((reg>>1)&0x7fffffff);
			}
				trueoffset=reg;
			}
			}
				
			
			if (1==((ins>>23)&1))
		{  address=CURRENT_STATE.REGS[base]-trueoffset;
			
		}
		else{ 
		address=CURRENT_STATE.REGS[base]+trueoffset;
		}
		 x=CURRENT_STATE.REGS[target];
		 mem_write_32(address,x);
			break;
			
			case 33:
				if (0==((dataoffset>>4)&1))
			{
			if (0==((dataoffset>>5)&3))
			{uint32_t q=(dataoffset>>7)&31;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			trueoffset=reg<<q;
				
			}
			else if((1==((dataoffset>>5)&3))||(2==((dataoffset>>5)&3)))
			{uint32_t q=(dataoffset>>7)&31;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			trueoffset=reg>>q;
				if ((2==((dataoffset>>5)&3))&&(1==(reg>>31)&1))
			{
				trueoffset=(trueoffset|(full<<(31-q)));
			}	
			}
			else if (3==((dataoffset>>5)&3))
			{
				uint32_t q=(dataoffset>>7)&31;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			int a=0;
			for (a=0;a<q;a++)
			{
				reg=((reg&1)<<31)+((reg>>1)&0x7fffffff);
			}
				trueoffset=reg;
			}
			}
			else if (1==((dataoffset>>4)&1))
			{
				if (0==((dataoffset>>5)&3))
			{ uint32_t q=CURRENT_STATE.REGS[((dataoffset>>7)&31)]&255;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			trueoffset=reg<<q;
				
			}
			else if((1==((dataoffset>>5)&3))||(2==((dataoffset>>5)&3)))
			{uint32_t q=CURRENT_STATE.REGS[((dataoffset>>7)&31)]&255;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			trueoffset=reg>>q;
					if ((2==((dataoffset>>5)&3))&&(1==(reg>>31)&1))
			{
				trueoffset=(trueoffset|(full<<(31-q)));
			}
			}
			else if (3==((dataoffset>>5)&3))
			{
				uint32_t q=CURRENT_STATE.REGS[((dataoffset>>7)&31)]&255;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			int a=0;
			for (a=0;a<q;a++)
			{
				reg=((reg&1)<<31)+((reg>>1)&0x7fffffff);
			}
				trueoffset=reg;
			}
			}
				
			
			if (1==((ins>>23)&1))
		{  address=CURRENT_STATE.REGS[base]-trueoffset;
			
		}
		else{ 
		address=CURRENT_STATE.REGS[base]+trueoffset;
		}
		
		NEXT_STATE.REGS[target]=mem_read_32(address);
			break;
			case 36:
				if (0==((dataoffset>>4)&1))
			{
			if (0==((dataoffset>>5)&3))
			{uint32_t q=(dataoffset>>7)&31;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			trueoffset=reg<<q;
				
			}
			else if((1==((dataoffset>>5)&3))||(2==((dataoffset>>5)&3)))
			{uint32_t q=(dataoffset>>7)&31;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			trueoffset=reg>>q;
					if ((2==((dataoffset>>5)&3))&&(1==(reg>>31)&1))
			{
				trueoffset=(trueoffset|(full<<(31-q)));
			}
			}
			else if (3==((dataoffset>>5)&3))
			{
				uint32_t q=(dataoffset>>7)&31;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			int a=0;
			for (a=0;a<q;a++)
			{
				reg=((reg&1)<<31)+((reg>>1)&0x7fffffff);
			}
				trueoffset=reg;
			}
			}
			else if (1==((dataoffset>>4)&1))
			{
				if (0==((dataoffset>>5)&3))
			{ uint32_t q=CURRENT_STATE.REGS[((dataoffset>>7)&31)]&255;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			trueoffset=reg<<q;
				
			}
			else if((1==((dataoffset>>5)&3))||(2==((dataoffset>>5)&3)))
			{uint32_t q=CURRENT_STATE.REGS[((dataoffset>>7)&31)]&255;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			trueoffset=reg>>q;
					if ((2==((dataoffset>>5)&3))&&(1==(reg>>31)&1))
			{
				trueoffset=(trueoffset|(full<<(31-q)));
			}
			}
			else if (3==((dataoffset>>5)&3))
			{
				uint32_t q=CURRENT_STATE.REGS[((dataoffset>>7)&31)]&255;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			int a=0;
			for (a=0;a<q;a++)
			{
				reg=((reg&1)<<31)+((reg>>1)&0x7fffffff);
			}
				trueoffset=reg;
			}
			}
				
			
			if (1==((ins>>23)&1))
		{  address=CURRENT_STATE.REGS[base]-trueoffset;
			
		}
		else{ 
		address=CURRENT_STATE.REGS[base]+trueoffset;
		}
		 x=(CURRENT_STATE.REGS[target]&0x000000ff)+((CURRENT_STATE.REGS[target]&0x000000ff)<<8)+((CURRENT_STATE.REGS[target]&0x000000ff)<<16)+((CURRENT_STATE.REGS[target]&0x000000ff)<<24);
			mem_write_32(address,x);
			break;
			case 37:
				if (0==((dataoffset>>4)&1))
			{
			if (0==((dataoffset>>5)&3))
			{uint32_t q=(dataoffset>>7)&31;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			trueoffset=reg<<q;
				
			}
			else if((1==((dataoffset>>5)&3))||(2==((dataoffset>>5)&3)))
			{uint32_t q=(dataoffset>>7)&31;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			trueoffset=reg>>q;
					if ((2==((dataoffset>>5)&3))&&(1==(reg>>31)&1))
			{
				trueoffset=(trueoffset|(full<<(31-q)));
			}
			}
			else if (3==((dataoffset>>5)&3))
			{
				uint32_t q=(dataoffset>>7)&31;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			int a=0;
			for (a=0;a<q;a++)
			{
				reg=((reg&1)<<31)+((reg>>1)&0x7fffffff);
			}
				trueoffset=reg;
			}
			}
			else if (1==((dataoffset>>4)&1))
			{
				if (0==((dataoffset>>5)&3))
			{ uint32_t q=CURRENT_STATE.REGS[((dataoffset>>7)&31)]&255;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			trueoffset=reg<<q;
				
			}
			else if((1==((dataoffset>>5)&3))||(2==((dataoffset>>5)&3)))
			{uint32_t q=CURRENT_STATE.REGS[((dataoffset>>7)&31)]&255;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			trueoffset=reg>>q;
					if ((2==((dataoffset>>5)&3))&&(1==(reg>>31)&1))
			{
				trueoffset=(trueoffset|(full<<(31-q)));
			}
			}
			else if (3==((dataoffset>>5)&3))
			{
				uint32_t q=CURRENT_STATE.REGS[((dataoffset>>7)&31)]&255;
			uint32_t reg=CURRENT_STATE.REGS[(dataoffset&15)];
			int a=0;
			for (a=0;a<q;a++)
			{
				reg=((reg&1)<<31)+((reg>>1)&0x7fffffff);
			}
				trueoffset=reg;
			}
			}
				
			
			if (1==((ins>>23)&1))
		{  address=CURRENT_STATE.REGS[base]-trueoffset;
			
		}
		else{ 
		address=CURRENT_STATE.REGS[base]+trueoffset;
		}
		NEXT_STATE.REGS[target]=mem_read_32(address)&0x000000ff;
			break;
			
		}
			break;
		case 2:
		//branch
			if (2==check_ins_Branch)
			{ uint32_t uoffset=((ins&threebytes)<<2);
				uint32_t offset_check=(uoffset>>25);
				if (1==uoffset)
				{uoffset = uoffset|offsetcheck;}
			int offset=uoffset;
				NEXT_STATE.REGS[15]=CURRENT_STATE.REGS[15]+offset+8; //prefetch
			}
			if (3==check_ins_Branch) //Branch with link
			{ uint32_t uoffset=((ins&threebytes)<<2);
				uint32_t offset_check=(uoffset>>25);
				if (1==uoffset)
				{uoffset = uoffset|offsetcheck;}
			int offset=uoffset;
				NEXT_STATE.REGS[14]=CURRENT_STATE.REGS[15];
				NEXT_STATE.REGS[15]=CURRENT_STATE.REGS[15]+offset+8; //due prefetch
	
			}
			break;
		case 3:
		//SWI
		if (3==check_ins_SWI)
		{
			if (0x0a==(ins&0x0000000a))
			{
			CURRENT_STATE.REGS[14]=CURRENT_STATE.REGS[15];
         RUN_BIT=0;
			}
		}
			break;
	 }
	 
	 end:
	batee5a=2;
	 //code ends here
}
