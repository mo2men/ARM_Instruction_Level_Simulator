#include <stdio.h>
#include "shell.h"
#include "utils.h"
void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
	 uint32_t add_mem;
	 uint32_t base_reg;
	 uint32_t inst;
	 int32_t offset;
	 int Reg;
	 int Op1,Op2;
	 uint32_t Imm;
	 long long int Op;
	 char flag=1;
	 char shift;
	 char i;
	 
	 NEXT_STATE.REGS[15]=CURRENT_STATE.REGS[15]+4;
	 
	 inst=mem_read_32(CURRENT_STATE.REGS[15]);
	 
	 
	 
	 int x=((inst>>26)& 3);
	 
	 switch((inst>>28)& 15){
		 //1st This is for checking conditions//
		 case 0:
		 // EQ   //
		 if((GET_BIT(CURRENT_STATE.CPSR,30)& 1)==1)
		 {
			
			 flag=0;
		 }
		 break;
		 
		 case 1:
		 //  NE  //
		 if((GET_BIT(CURRENT_STATE.CPSR,30)& 1)==0)
		 {
			 
			 flag=0;
		 }
		 break;
		 
		 case 10:
		 //  GE  //
		 if((GET_BIT(CURRENT_STATE.CPSR,31))==(GET_BIT(CURRENT_STATE.CPSR,28)))
		 {
			 flag=0;
		 }
		 break;
		 
		 case 11:
		 //  LT  //
		 if(((GET_BIT(CURRENT_STATE.CPSR,31)))!=(GET_BIT(CURRENT_STATE.CPSR,28)))
		 {
			 flag=0;
		 }
		 break;
		 
		 case 12:
		 //  GT   //
		 if(((GET_BIT(CURRENT_STATE.CPSR,31))==(GET_BIT(CURRENT_STATE.CPSR,28)))&&((((CURRENT_STATE.CPSR)>>30)& 1)==0))
		 {
			 
			 flag=0;
		 }
		 
		 break;
		 
		 case 13:
		 //  LE  //
		 if(((GET_BIT(CURRENT_STATE.CPSR,31))!=(GET_BIT(CURRENT_STATE.CPSR,28)))||((((CURRENT_STATE.CPSR)>>30)& 1)==1))
		 {
			 
			 flag=0;
		 }
		 
		 break;
		 
		 case 14:
		 //  AL  //
		 flag=0;
		 
		 break;
	 }
	 
	 if(!flag)
		 
		 //2nd Here Check Instruction after checking condition
	 {
	 
	 
	 switch(x){
		 
		 case 3:
		 //  SWI   //
		 if((inst&0x000000FF)==(10)){
		 RUN_BIT=0;
		 
		 }
		 
		 break;
		 
		 case 2:
		 	//enter code here for branch/ branch link//
			offset=(inst & 0x00FFFFFF);  //  offset //
			
			offset=(offset<<2);
			
			if(inst&(1<<23))//for negative// 
			{
				offset=offset|(0xFC000000);
				
			}
			
			if(inst & (1<<24))
			{
			NEXT_STATE.REGS[14]=CURRENT_STATE.REGS[15];//save old inst in linker address(r14)//
			}
			
			NEXT_STATE.REGS[15]=CURRENT_STATE.REGS[15]+offset+8;//update PC//

		 break;
		 
		 
		 
		 
		 case 1:
		 //enter code here for LDR/ STR  instructions//
		 
		 base_reg=CURRENT_STATE.REGS[(inst>>16)&15];// Base register like DS & SS
		 
		 switch((inst>>25)&1)
		    {   //for  Type of offset //
			 case 0:
			 // insigned Imm offset//
			 
			 offset=inst&0xFFF;
			 
			 break;
			 case 1:
			 // offset is a shifted REGISTER //
			 offset=CURRENT_STATE.REGS[inst&15];
			 shift=(inst>>7)&31;
			  switch((inst>>5)& 3)
					{
						//check type of shift//
						
						case 0:
						//logical left//
						offset<<=shift;
						
						break;
						
						case 1:
						//logical right//
						offset=(unsigned int)offset>>shift;
						break;
						
						case 2:
						//arithematic right //
						
						offset>>=shift;
						
						
						break;
						
						case 3:
						//rotate right//
						if(shift!=0)
						{
						
						for(i=1;i<=shift;i++)
						 {
							
							offset=((unsigned int)offset>>1)|((GET_BIT(offset,0))<<31);
							
						 }
						}
						else{
							//Rotate   Right  extended    RRx//
							CLR_BIT(NEXT_STATE.CPSR,29);
							NEXT_STATE.CPSR|=((GET_BIT(offset,0))<<29);
							offset=(unsigned int)offset>>1;
							CLR_BIT(offset,31);
							offset|=((GET_BIT(CURRENT_STATE.CPSR,29))<<31);
						}
						
						break;
					}
				break;	
			}
			
	 switch((inst>>23)&1)
	 {
		 // check expansion down / up //
		 case 0:
		 // down expansion like Stack//
		 add_mem=base_reg-offset;
		 
		 break;
		 
		 case 1:
		 // up expansion like DS//
		 
		 add_mem=base_reg+offset;
		 
		 break;
		 
	 }
	 
	 
	 switch((inst>>20)&1)
    {
		 // check LDR or STR
		 case 0:
		 // STR Memory Write
		 switch((inst>>22)&1)
		 {// Write Word or Byte
			case 0:
			// Word
			mem_write_32(add_mem,CURRENT_STATE.REGS[(inst>>12)&15]);
			
			break;
			case 1:
			// Byte
			mem_write_32(add_mem,(unsigned char) CURRENT_STATE.REGS[(inst>>12)&15]);
			
			break;
		 }
		 
		 break;
		 
		 case 1:
		 // LDR Memory Read
		 switch((inst>>22)&1)
		 {
			 case 0:
			 //Read Word
			 
			 NEXT_STATE.REGS[(inst>>12)&15]=mem_read_32(add_mem);
			 
			 break;
			 case 1:
			 //Read Byte
			 
			 NEXT_STATE.REGS[(inst>>12)&15]=(unsigned char)mem_read_32(add_mem);
			 
			 break;
			 
			 
		 }
		 break;
	} 
		 
		 
		 
		 break;
		 
		 case 0:
		 //for  Data  Processing or MUL instruction//
	   if((((inst>>4)&15)==9)&&((GET_BIT(inst,25))==0))
	  { //MUL instruction
  
		Op1=CURRENT_STATE.REGS[inst&15];
		Op2=CURRENT_STATE.REGS[(inst>>8)&15];
		Reg=CURRENT_STATE.REGS[(inst>>12)&15];
		
		NEXT_STATE.REGS[(inst>>16)&15]=Op1*Op2;
		
		Op=(long long int)Op1*Op2;
		
		if((inst>>21)&1)
		{
			NEXT_STATE.REGS[(inst>>16)&15]+=Reg;
			Op+=Reg;
		
		}
		
		
	  }
		 
	     
	  else{  //   Data Processing   //
			 Op1=CURRENT_STATE.REGS[(inst>>16)& 15];
		      switch((inst>>25)& 1)
		      //1st  for       Operand2    type//
		      {
		        case 0:
				//  for  Op2  is  a  register//
				Reg=CURRENT_STATE.REGS[inst& 15];
				
				 switch((inst>>4)& 1){
					case 0:
					//take shift from immediate value 5bit unsigned//
					shift=(inst>>7)&31;
					
					break;
					
					case 1:
					//take shift from REGISTER value 5bit unsigned//
					
					shift=CURRENT_STATE.REGS[(inst>>8)&15];
					break;
					
					}
					
					
					
				    switch((inst>>5)& 3)
					{
						//check type of shift//
						
						case 0:
						//logical left//
			
						Reg<<=shift;
						
						Op2=Reg;
						break;
						
						case 1:
						//logical right//
						
						Reg=(unsigned int)Reg>>shift;
						
						
						Op2=Reg;
						break;
						
						case 2:
						//arithematic right //
						
						Reg>>=shift;
						Op2=Reg;
						break;
						
						case 3:
						//rotate right//
						if(shift!=0){
						
						for(i=1;i<=shift;i++){
							
							Reg=((unsigned int)Reg>>1)|((GET_BIT(Reg,0))<<31);
							
						}
						}
						else{
							//Rotate   Right  extended    RRx//
							CLR_BIT(NEXT_STATE.CPSR,29);
							NEXT_STATE.CPSR|=((GET_BIT(Reg,0))<<29);
							Reg=(unsigned int)Reg>>1;
							CLR_BIT(Reg,31);
							Reg|=((GET_BIT(CURRENT_STATE.CPSR,29))<<31);
						}
						
					    Op2=Reg;
						break;
					}
					
				break;
				
				
				
				case 1:
				//  for  Op2  is  immediate//
				Imm=(inst&0xFF);
				shift=(inst>>8)&15;
				if(shift!=0){
						
						for(i=1;i<=2*shift;i++){
							
							Imm=((unsigned int)Imm>>1)|((GET_BIT(Imm,0))<<31);
							
						}
						}
				Op2=Imm;		
				
				break;
				      }
					  
			
		      switch((inst>>21)&15)
				//2nd     for  Op Code//
				{
				case 0:
				// AND inst //
				NEXT_STATE.REGS[(inst>>12)&15]=Op1&Op2;
				Op=(long long int)Op1&Op2;
				break;
				case 1:
				// XOR inst //
				NEXT_STATE.REGS[(inst>>12)&15]=Op1^Op2;
				Op=(long long int)Op1^Op2;
				break;
				case 2:
				// SUB inst //
				NEXT_STATE.REGS[(inst>>12)&15]=Op1-Op2;
				Op=(long long int)Op1-Op2;
				break;
				case 3:
				// RSUB inst //
				NEXT_STATE.REGS[(inst>>12)&15]=Op2-Op1;
				Op=(long long int)Op2-Op1;
				break;
				
				case 4:
				// for Add inst//
				NEXT_STATE.REGS[(inst>>12)&15]=Op1+Op2;
				Op=(long long int)Op1+Op2;
				break;
				case 5:
				// for Add with carry//
				NEXT_STATE.REGS[(inst>>12)&15]=Op1+Op2+(GET_BIT(CURRENT_STATE.CPSR,29));
				Op=(long long int)Op1+Op2+(GET_BIT(CURRENT_STATE.CPSR,29));
				break;
				case 6:
				// for Sub with carry//
				NEXT_STATE.REGS[(inst>>12)&15]=Op1-(Op2+(GET_BIT(CURRENT_STATE.CPSR,29)))-1;
				Op=(long long int)Op1-(Op2+(GET_BIT(CURRENT_STATE.CPSR,29)))-1;
				break;
				case 7:
				// for RSub with carry//
				NEXT_STATE.REGS[(inst>>12)&15]=Op2-(Op1+(GET_BIT(CURRENT_STATE.CPSR,29)))-1;
				Op=(long long int)Op2-(Op1+(GET_BIT(CURRENT_STATE.CPSR,29)))-1;
				break;
				case 8:
				// TST INST //
				inst|=(1<<20);// to set condition code bit//
				flag=1;
				if((Op1 & Op2)==0){
					NEXT_STATE.CPSR=SET_BIT(CURRENT_STATE.CPSR,30); 
					}
				else{  
				    NEXT_STATE.CPSR=CLR_BIT(CURRENT_STATE.CPSR,30);
					
				}
				CLR_BIT(CURRENT_STATE.CPSR,31);
				NEXT_STATE.CPSR=CURRENT_STATE.CPSR|((GET_BIT(Op1 & Op2,31))<<31);
				break;
				
				case 9:
				// TEQ INST //
				inst|=(1<<20);// to set condition code bit//
				flag=1;
				if((Op1 ^ Op2)==0){
					NEXT_STATE.CPSR=SET_BIT(CURRENT_STATE.CPSR,30); 
					}
				else{  
				    NEXT_STATE.CPSR=CLR_BIT(CURRENT_STATE.CPSR,30);
					
				}
				
				CLR_BIT(CURRENT_STATE.CPSR,31);
				NEXT_STATE.CPSR=CURRENT_STATE.CPSR|((GET_BIT(Op1 ^ Op2,31))<<31);
					
				
				break;
				
				
				
				case 10:
				//for CMP inst //
				inst|=(1<<20);// to set condition code bit//
				flag=1;
				
				if((Op1-Op2)==0){
					NEXT_STATE.CPSR=SET_BIT(CURRENT_STATE.CPSR,30);
				
					}
				else{  
				NEXT_STATE.CPSR=CLR_BIT(CURRENT_STATE.CPSR,30);
				
				}
				CLR_BIT(NEXT_STATE.CPSR,31);
				NEXT_STATE.CPSR|=((GET_BIT((Op1 - Op2),31))<<31);//SET the N flag
				
				Reg=Op1 - Op2;
				Op=(long long int)Op1 - Op2;
				CLR_BIT(NEXT_STATE.CPSR,29);
				NEXT_STATE.CPSR|=((GET_BIT(Op,32))<<29);//SET the C flag
				
				CLR_BIT(NEXT_STATE.CPSR,28);//SET the V flag 
				NEXT_STATE.CPSR|=(((GET_BIT(Op,63))^(GET_BIT(Reg,31)))<<28);
			
				
				break;
				case 11:
				//for CMN inst //
				inst|=(1<<20);// to set condition code bit//
				flag=1;
				
				if((Op1+Op2)==0){
					NEXT_STATE.CPSR=SET_BIT(CURRENT_STATE.CPSR,30); 
					}
				else{  
				NEXT_STATE.CPSR=CLR_BIT(CURRENT_STATE.CPSR,30);
				
				}
				CLR_BIT(NEXT_STATE.CPSR,31);
				NEXT_STATE.CPSR|=((GET_BIT(Op1 + Op2,31))<<31);//SET the N flag
				
				Reg=Op1 + Op2;
				Op=(long long int)Op1 + Op2;
				
				CLR_BIT(NEXT_STATE.CPSR,29);
				NEXT_STATE.CPSR|=((GET_BIT(Op,32))<<29);//SET the C flag
				
				CLR_BIT(NEXT_STATE.CPSR,28);//SET the V flag 
				NEXT_STATE.CPSR|=(((GET_BIT(Op,63))^(GET_BIT(Reg,31)))<<28);
			
				
				break;
				
				case 12:
				// AND inst //
				NEXT_STATE.REGS[(inst>>12)&15]=Op1|Op2;
				Op=(long long int)Op1|Op2;
				break;
				
		        case 13:
				//for MOV inst //
				
				NEXT_STATE.REGS[(inst>>12)&15]=Op2; 
		        Op=(long long int)Op2;
		 
		        break;
				
				case 15:
				//for   MVN   inst //
				
				NEXT_STATE.REGS[(inst>>12)&15]=Op2^0xFFFFFFFF;
				Op=(long long int)Op2^0xFFFFFFFF;
				break;
		        }
	  }
				if((GET_BIT(inst,20))&& (flag==0))
				{
					CLR_BIT(NEXT_STATE.CPSR,31);
				    NEXT_STATE.CPSR|=((GET_BIT(Op,31))<<31);//SET the N flag
					
					if(Op==0)
					{
					// Z Flag//	
                    SET_BIT(NEXT_STATE.CPSR,30);
					}
			
					else
					{
						CLR_BIT(NEXT_STATE.CPSR,30);	
					}
					
				CLR_BIT(NEXT_STATE.CPSR,29);
				NEXT_STATE.CPSR|=((GET_BIT(Op,32))<<29);//SET the C flag
				
				CLR_BIT(NEXT_STATE.CPSR,28);//SET the V flag 
				NEXT_STATE.CPSR|=(((GET_BIT(Op,63))^(GET_BIT(Op,31)))<<28);
				
				}
		 
		break;
	    }
	 
	} 
}
