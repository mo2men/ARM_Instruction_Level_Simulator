#include <stdio.h>
#include "shell.h"

void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
	 
	 int32_t offset;
	 NEXT_STATE.REGS[15]=CURRENT_STATE.REGS[15]+4;
	 
	 inst=mem_read_32(CURRENT_STATE.REGS[15]);
	 char MH = (inst>>26)&3;
	 char flags=0; // related to n,z,c,v flags
	 char RN,Op2,C;
	 
	 switch((inst>>28)& 15){
		 // check conditions
		 case 0:
		 // EQ   Z set
		 if(((CURRENT_STATE.CPSR>>30)& 1)==1)
		 {
			
			 flags=1;
		 }
		 break;
		 
		 case 1:
		 //  NE  Z clear
		 if(((CURRENT_STATE.CPSR>>30)& 1)==0)
		 {
			 
			 flags=1;
		 }
		 break;
		 
		 case 10:
		 //  GE  : Nflag = vflag
		 if(((CURRENT_STATE.CPSR>>31))==((CURRENT_STATE.CPSR>>28)))
		 {
			 flags=1;
		 }
		 break;
		 
		 case 12:
		 //  GT   Z clear AND (N equals V) 
		 if((((CURRENT_STATE.CPSR>>31))==((CURRENT_STATE.CPSR>>28)))&&((((CURRENT_STATE.CPSR)>>30)& 1)==0))
		 {
			 
			 flags=1;
		 }
		 
		 break;
		 
		 case 11:
		 //  LT  :Nflag not equal to vflag
		 if((((CURRENT_STATE.CPSR>>31)))!=((CURRENT_STATE.CPSR>>28)))
		 {
			 flags=1;
		 }
		 break;
		 
		 
		 
		 case 13:
		 //  LE  Z set OR (N not equal to V) 
		 if((((CURRENT_STATE.CPSR>>31))!=((CURRENT_STATE.CPSR>>28)))||((((CURRENT_STATE.CPSR)>>30)& 1)==1))
		 {
			 
			 flags=1;
		 }
		 
		 break;
		 
		 case 14:
		 //  AL  ignored
		 flags=1;
		 
		 break;
	 }
	 
	 if(!flag)
		 //Check Instruction after checking conditions 
	 {
	 
	 switch(MH)
	 {
		 case 0:
		 		 // data processing
		 
		 RN=CURRENT_STATE.REGS[(inst>>16)& 15];
		      switch((inst>>25)& 1)
		      {
		        case 0: // AND
				
				Op2=CURRENT_STATE.REGS[inst& 15];
				
				break;
				
				case 12;
				NEXT_STATE.REGS[(inst>>12)&15]=RN|Op2;
				
				break;
				
				case 1:  // EOR
				
				Op2=(inst&0xFF);
				
				break;
				      }
					  
			
		      switch((inst>>21)&15)
				{
				
				case 2:
				// SUB 
				NEXT_STATE.REGS[(inst>>12)&15]=RN-Op2;
				
				break;
				
				case 4:
				// ADD
				NEXT_STATE.REGS[(inst>>12)&15]=RN+Op2;
				
				break;
				
				case 5:
				// ADC - Rd:= Op1 + Op2 + C
				NEXT_STATE.REGS[(inst>>12)&15]=RN+Op2+C;
				
				break;
				
				case 6:
				// SBC - Rd:= Op1 - Op2 + C-1
				NEXT_STATE.REGS[(inst>>12)&15]=RN+Op2+C-1;
				
				break;
				
				case 7:
				 // RSC - Rd:= Op2 - Op1 + C-1
				NEXT_STATE.REGS[(inst>>12)&15]=Op2-RN+C-1;
					
				case 10:
				//for CMP inst //
				inst|=(1<<20);
				if((Op1-Op2)==0){
					NEXT_STATE.CPSR=((CURRENT_STATE.CPSR>>30)&1); 
					}
				else{  
				NEXT_STATE.CPSR=((CURRENT_STATE.CPSR>>30)&1;
				
				}
				
				break;
				
		        case 13:
				//for MOV inst //
				
				NEXT_STATE.REGS[(inst>>12)&15]=Op2; 
		      
		 
		        break;
				
				case 15:
				//for   MVN   inst //
				
				NEXT_STATE.REGS[(inst>>12)&15]=Op2^0xFFFFFFFF;
		 break;
		 
		 case 1:
		    switch((inst>>20)& 1)
		      {
				  case 0:
				  
				  if ((inst>>22)& 1)==1 {
					  
				  }
				  
				  break;
				  case 1:
				  break;
				  
		 
		 break;
		 
		 case 2:
		 // at branch   L=0;
		 int32_t offset=(inst & 0x00FFFFFF);
		 offset=(offset<<2);
		 if(1==(offset>>25))
		 {
		 offset=offset | 0xFC000000;
		 }
		 NEXT_STATE.REGS[15]=CURRENT_STATE.REGS[15]+8+offset;
		 
		 else// at branch with link  L=1;
		 {
		  if(1==(1 & (inst<<24)){
			int32_t offset=(inst & 0x00FFFFFF);
			offset=(offset<<2);
			
			if(inst&(1<<23)) 
			{
				offset=offset|(0xFC000000);
				
			}
			
			NEXT_STATE.REGS[14]=CURRENT_STATE.REGS[15];
			
			NEXT_STATE.REGS[15]=CURRENT_STATE.REGS[15]+offset+8;

		 }
			 
		 break;
		 
		 case 3:
		 if(10==(inst&0x000000FF))
		 {
			 RUN_BIT=0;
			 NEXT_STATE.REGS[14]=CURRENT_STATE.REGS[15];
		 } 
		 break;
 
	 }	 
 
}
