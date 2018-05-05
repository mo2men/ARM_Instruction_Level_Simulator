#include <stdio.h>
#include "shell.h"
 

void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
	char N_flag=((CURRENT_STATE.CPSR>>31)&1);
    char Z_flag=((CURRENT_STATE.CPSR>>30)&1);
     char C_flag=((CURRENT_STATE.CPSR>>29)&1);
     char V_flag=((CURRENT_STATE.CPSR>>28)&1);
	 unsigned int inst ;
	 int disp;
	 char i=0;
	 int op1,op2,base_reg,address,offset;
	 unsigned char shift;
	 inst=mem_read_32(CURRENT_STATE.REGS[15]);
	 NEXT_STATE.REGS[15]=CURRENT_STATE.REGS[15]+4;
	 switch((inst>>28)&15)
	 {
//==== mo'men ====== Checking condition is perfect ===== //				

		case 0:
		//EQ
		
		if(Z_flag==1)
		{
			i=1;
		}
		
        break;		
		case 1:
		//NE
		
		if(Z_flag==0)
		{
			i=1;
		}
		
        break;
		
		case 10:
		// GE
		if(N_flag==V_flag)
		{
			i=1;
		}
		
		
        break;
		case 11:
		// LT
		if(N_flag!=V_flag)
		{
			i=1;
		}
		
		
        break;
		case 12:
		// GT
		if((Z_flag==0)&&(N_flag==V_flag))
		{
			i=1;
		}
		
		
        break;
		case 13:
		// LE
		if((Z_flag==1)||(N_flag!=V_flag))
		{
			i=1;
		}
		
		
        break;
		
		case 14:
		// AL
		
		i=1;
		
        break;
		
		
	 }
	 
	 
	 if(i==1)
	 {
	 switch((inst>>26)&3)
	 //for checking inst type 
	 {
		 case 1:
		 
		 base_reg=CURRENT_STATE.REGS[(inst>>16)&15];
	     if((inst>>25)&1)
	     {
		 //offset register
		 offset=CURRENT_STATE.REGS[inst&15];
		 shift=(inst>>7)&31;
		 switch((inst>>5)&3)
			 {
				 //check shift type
				case 0:
				// LSL
				
				offset=offset<<shift;
				
				
				break;
				case 1:
				// LSR
				
				offset=(unsigned int)offset>>shift;
				
				break;
				case 2:
				// ASR
				
				offset=offset>>shift;
				
				break;
				case 3:
				// ROR
				
				if(shift==0)
				{
					//RRX
					NEXT_STATE.CPSR&=0xDFFFFFFF;
					NEXT_STATE.CPSR|=((offset&1)<<29);
					offset=((unsigned int)offset>>1)|(C_flag<<31);
					
				}
				else
				{
					for(int x=1;x<=shift;x++)
					{
						
					offset=((unsigned int)offset>>1)|((offset&1)<<31);
					}
				}
				break;
			 }
		 
	 }
	 else
	 {
		 //Immediate offset
		offset=inst&0xFFF; 
		 
	 }
	 
	 
	 if((inst>>23)&1)
	 {
		 //Up
		address=base_reg+offset; 
	
	 }
	 else
	 {
		 //Down
		 address=base_reg-offset;
		 
	 }
		
	 switch((inst>>20)&1)
	 {
		 //ldr or str
		 case 0:
		 //store
		 if((inst>>22)&1)
		 {  //Byte
			mem_write_32(address,(unsigned char)CURRENT_STATE.REGS[(inst>>12)&15]); 
		 }
		 else
		 {
			//Word 
			mem_write_32(address,CURRENT_STATE.REGS[(inst>>12)&15]);
		 }
		 break;
		 case 1:
		 //load
		 if((inst>>22)&1)
		 {  //Byte
			NEXT_STATE.REGS[(inst>>12)&15]=(unsigned char)mem_read_32(address); 
		 }
		 else
		 {
			//Word 
			NEXT_STATE.REGS[(inst>>12)&15]=mem_read_32(address);
		 }
		 break;
	 }	
		 
		 
		 break;
		 case 3:
		 // if SWI
		 if((inst&0xFF)==10)
		 {
			RUN_BIT=0; 
			 
		 }
		 
		 break;
		 
		 case 2:
		 //if B/BL
		 
		 disp=inst&0xFFFFFF;// disp is offset
		 disp=disp<<2 ;
	     if((disp>>25)&1)
		 {
			disp|=0xFC000000;
		
		  }
		 
		 NEXT_STATE.REGS[15]=CURRENT_STATE.REGS[15]+disp+8;
		 
	 if((inst>>26)&1)
	 {
		NEXT_STATE.REGS[14]= CURRENT_STATE.REGS[15];
		 
	 }
		 
		 break;
		
		
		 case 0:
		 // Data Processing or Multiply
		 if((((inst>>25)&1)==0)&&(((inst>>4)&15)==9))
		 {  //Multiply
			 op1=inst&15;
			 op2=(inst>>8)&15;
			 if((inst>>21)&1)
			 {  //Multiply accumulate
			 
			    NEXT_STATE.REGS[(inst>>16)&15]=(op1 * op2)+CURRENT_STATE.REGS[(inst>>12)&15];
			 }
			 else{
				 //Multiply
				NEXT_STATE.REGS[(inst>>16)&15]=op1 * op2; 
			 }
			 
		 }
		 
		 else{
		 op1=CURRENT_STATE.REGS[(inst>>16)&15];
		 switch((inst>>25)&1)
		 {
			 // check I bit
			 case 0:
			 // op2 register
			 op2=CURRENT_STATE.REGS[inst&15];
			 
			 switch((inst>>4)& 1)
			 {
				 // shift
				case 0:
				// shift unsigned 5bit Immediate
				shift=(inst>>7)&31;
				
				break;
				case 1:
				// shift register
				shift=CURRENT_STATE.REGS[(inst>>8)&15];
				
				break; 
				 
			 }
			 
			 switch((inst>>5)&3)
			 {
				 //check shift type
				case 0:
				// LSL
				
				op2=op2<<shift;
				
				
				break;
				case 1:
				// LSR
				
				op2=(unsigned int)op2>>shift;
				
				break;
				case 2:
				// ASR
				
				op2=op2>>shift;
				
				break;
				case 3:
				// ROR
				
				if(shift==0)
				{
					//RRX
					NEXT_STATE.CPSR&=0xDFFFFFFF;
					NEXT_STATE.CPSR|=((op2&1)<<29);
					op2=((unsigned int)op2>>1)|(C_flag<<31);
					
				}
				else
				{
					for(int x=1;x<=shift;x++)
					{
						
					op2=((unsigned int)op2>>1)|((op2&1)<<31);
					}
				}
				break;
			 }
			 
			 
			 break;
			 case 1:
			 //op2 Immediate value
			 
			 op2=inst&0xFF;
			 shift=((inst>>8)&15);
			 if(shift!=0)
			 {
				 
			 for(int x=1;x<=2*shift;x++)
				{
						
					op2=((unsigned int)op2>>1)|((op2&1)<<31);
					
				}
			 
			 }
			 
			 break; 
			 
		 }
		 
		 switch((inst>>21)&0xF)
		 {
			 case 0:
			 //AND
			 NEXT_STATE.REGS[(inst>>12)&15]=op1 & op2;
			 break;
			 //XOR
			 case 1:
			 NEXT_STATE.REGS[(inst>>12)&15]=op1 ^ op2;
			 
			 break;
			 case 2:
			 // SUB
			 NEXT_STATE.REGS[(inst>>12)&15]=op1 - op2;
			 break;
			 case 3:
			 //RSB
			 NEXT_STATE.REGS[(inst>>12)&15]=op2 - op1;
			 
			 break;
			 case 4:
			 //ADD
			  NEXT_STATE.REGS[(inst>>12)&15]=op1 + op2;
			 break;
			 
			 case 5:
			 //ADC
		     NEXT_STATE.REGS[(inst>>12)&15]=op1 + op2 +C_flag;
			 
			 break;
			 case 6:
			 //SBC
		     NEXT_STATE.REGS[(inst>>12)&15]=op1+C_flag - (op2+1);
			 break;
			 case 7:
			 //RSC
			 NEXT_STATE.REGS[(inst>>12)&15]=op2+C_flag - (op1+1);
			 break;
			 case 8:
			 // TST
			 NEXT_STATE.CPSR&=0x7FFFFFFF;// for N_flag
			 NEXT_STATE.CPSR|=((op1&op2)&0x80000000);
			 
			 if((op1&op2)==0)
			 {
				 NEXT_STATE.CPSR|=(~(0xBFFFFFFF));
			 }
			 else
			 {
				  NEXT_STATE.CPSR&=0xBFFFFFFF;
			 }	 
				 
			 break;
			 case 9:
			 // TEQ
			 NEXT_STATE.CPSR&=0x7FFFFFFF;// for N_flag
			 NEXT_STATE.CPSR|=((op1^op2)&0x80000000);
			 
			 if((op1^op2)==0)
			 {
				 NEXT_STATE.CPSR|=(~(0xBFFFFFFF));
			 }
			 else
			 {
				  NEXT_STATE.CPSR&=0xBFFFFFFF;
			 }	 
				 
			 
			 
			 break;
			 
			 case 10:
			 // CMP
			 NEXT_STATE.CPSR&=0x7FFFFFFF;// for N_flag
			 NEXT_STATE.CPSR|=((op1-op2)&0x80000000);
			 
			 if((op1-op2)==0)
			 {
				 NEXT_STATE.CPSR|=(~(0xBFFFFFFF));
			 }
			 else
			 {
				  NEXT_STATE.CPSR&=0xBFFFFFFF;
				 
			 }	 
				 
			 
			 
			 break;
			 case 11:
			 // CMN
			 NEXT_STATE.CPSR&=0x7FFFFFFF;// for N_flag
			 NEXT_STATE.CPSR|=((op1+op2)&0x80000000);
			 
			 if((op1+op2)==0)
			 {
				 NEXT_STATE.CPSR|=(~(0xBFFFFFFF));
			 }
			 else
			 {
				  NEXT_STATE.CPSR&=0xBFFFFFFF;
				  
			 }	 
				 
			 
			 
			 break;
			 
			 
			 case 12:
			 //ORR
			 NEXT_STATE.REGS[(inst>>12)&15]=op2 | op1;
			 break;
			 case 13:
			 //MOV
			 NEXT_STATE.REGS[(inst>>12)&15]=op2;
			 break;
			 case 15:
			 //MVN
			 NEXT_STATE.REGS[(inst>>12)&15]=op2^0xFFFFFFFF;
			 break;
			 
			 
			 
		 }
		 
		 
		 }
		 
		 break;
		 
	 }
	 
	 } 
	 
}
