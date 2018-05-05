#include <stdio.h>
#include "shell.h"

#define Offset 0x00FFFFFF


void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */

	 NEXT_STATE.PC = CURRENT_STATE.PC+4; // Increment Program Counter
	 
	 uint32_t Instruction= mem_read_32(CURRENT_STATE.PC); //Reading Instruction from memory
	 
	 int Inst_Set_Format=((Instruction>>26)&3); // To know which instruction set format 
	 
	  /* Constants */
	 uint32_t Rd;
	 uint32_t Op1;
	 uint32_t Op2;
	 uint32_t Op3;
	 uint32_t OpCode;
	 uint32_t Imm;
	 uint32_t shift_amount;
	 uint32_t shift_op;
	 uint32_t Rotate;
	 uint32_t y;
	 uint32_t z;
	
	//uint32_t Base_Reg;
	//uint32_t Source_Distination_Reg;
	//uint32_t Offset_Value;
	// uint32_t Offset_Reg;
	//uint32_t Shift;
	//uint32_t Value;
	
	 
	 
	 
	switch (Inst_Set_Format)
	{
		case 0: 
		/* Basic feature is data processing*/
		
		Rd  = (Instruction>>12)& 0x0000000F;

		Op1 = (Instruction>>16)& 0x0000000F;
		
		OpCode = ((Instruction>>21) & 0x0000000F) ;	
	
		
		if (((Instruction>>25) & 1)==1) /* Operand 2 is an immidiat value*/
		{
			Imm = (Instruction & 0x000000FF) ;
			
			Rotate = (Instruction>>8) & 0x0000000F;
			
			Imm= (Imm << (32- 2*Rotate));
//==== mo'men ====== This will not implement a rotate ===== //				

			
			if (OpCode==0) //AND
			{
				NEXT_STATE.REGS[Rd]= CURRENT_STATE.REGS[Op1] & Imm;
//==== mo'men ====== What about checking the Flag setting bit ===== //				

			}
			else if (OpCode==1) //EOR
			{
				NEXT_STATE.REGS[Rd]= CURRENT_STATE.REGS[Op1] ^ Imm;
			}
			else if (OpCode==2) //SUB
			{
				NEXT_STATE.REGS[Rd]= CURRENT_STATE.REGS[Op1] - Imm;
			}
			else if (OpCode==4) //ADD
			{
				NEXT_STATE.REGS[Rd]= CURRENT_STATE.REGS[Op1] + Imm;
			}
		/*	else if (OpCode==5) //ADC
			{
				
			}
			else if (OpCode==6) //SBC
			{
				
			}
			else if (OpCode==8) //TST
			{
				
			}
			else if (OpCode==9) //TEQ
			{
				
			}
			else if (OpCode==10) //CMP
			{
				
			}
			else if (OpCode==11) //CMN
			{
			}
			
		*/	
			else if (OpCode==12) //ORR
			{
				NEXT_STATE.REGS[Rd]= CURRENT_STATE.REGS[Op1] | Imm;
			}
			else if (OpCode==13) //MOV
			{
				NEXT_STATE.REGS[Rd]= Imm ;   
			}
			else if (OpCode==14) //BIC
			{
				NEXT_STATE.REGS[Rd]= CURRENT_STATE.REGS[Op1] & (~Imm);
			}
			else if (OpCode==15) //MVN
			{
				NEXT_STATE.REGS[Rd]= ~Imm;
			}
		
		}	
						
		else if (((Instruction>>25) & 1)==0) /* Operad 2 is a register*/
		{
			shift_op= (Instruction >> 4) & 0x000001FF;
			
			shift_amount=(Instruction >> 7) & 0x0000000F;
			
			Op2 = Instruction & 0x0000000F;

			if (((shift_op>>1) & 3) == 0) // Logical left
			{
				NEXT_STATE.REGS[Op2] = CURRENT_STATE.REGS[Op2] << shift_amount; //Operand 2 value
//==== mo'men ====== You shouldn't update the register itself ===== //				
//==== mo'men ====== Just prepare it before entering the ALU ===== //				
//==== mo'men ====== You forgot to update the carry flag ===== //				
			}
			else if (((shift_op>>1) & 3) == 1) // Logical right
			{
				NEXT_STATE.REGS[Op2]  = CURRENT_STATE.REGS[Op2]  >> shift_amount; //Operand 2 value
			}
			else if (((shift_op>>1) & 3) == 2) // Arithmetic right 
			{
				NEXT_STATE.REGS[Op2]  = (CURRENT_STATE.REGS[Op2]  >> shift_amount) | (0xF8000000); //Operand 2 value
			}
			else if (((shift_op>>1) & 3) == 3) // Rotate right
			{
//==== mo'men ====== You Didn't implement the Rotate Right Extended function ===== //				
				y = CURRENT_STATE.REGS[Op2]  >> shift_amount; 
				
				z = CURRENT_STATE.REGS[Op2] << (32-shift_amount);
				
				NEXT_STATE.REGS[Op2] = y | z;
			}
		
			
			if (OpCode==0) //AND
			{
				NEXT_STATE.REGS[Rd]= CURRENT_STATE.REGS[Op1] & CURRENT_STATE.REGS[Op2];
			}
			else if (OpCode==1) //EOR
			{
				NEXT_STATE.REGS[Rd]= CURRENT_STATE.REGS[Op1] ^ CURRENT_STATE.REGS[Op2];
			}
			else if (OpCode==2) //SUB
			{
				NEXT_STATE.REGS[Rd]= CURRENT_STATE.REGS[Op1] - CURRENT_STATE.REGS[Op2];
			}
			else if (OpCode==4) //ADD
			{
				NEXT_STATE.REGS[Rd]= CURRENT_STATE.REGS[Op1] + CURRENT_STATE.REGS[Op2];
			}
		/*	else if (OpCode==5) //ADC
			{
				NEXT_STATE.REGS = Op1 + Op2 + Carry;
			}
			else if (OpCode==6) //SBC
			{
				NEXT_STATE.REGS[Rd] = Op1 - Op2 - 1 + Carry; 
			}
		*/	
			else if (OpCode==8) //TST
			{
//==== mo'men ====== Nope, You should update the flag bits only in the CPSR ===== //				
//==== mo'men ====== Nope, Based on the results update Z,N,V,C flags ===== //				

				NEXT_STATE.CPSR = Op1 & Op2;
			}
			else if (OpCode==9) //TEQ
			{
				NEXT_STATE.CPSR = Op1 ^ Op2;
			}
			else if (OpCode==10) //CMP
			{
				NEXT_STATE.CPSR = Op1 - Op2;
			}
			else if (OpCode==11) //CMN
			{
				NEXT_STATE.CPSR = Op1 + Op2;
			}
				
		
			else if (OpCode==12) //ORR
			{
				NEXT_STATE.REGS[Rd]= CURRENT_STATE.REGS[Op1] | CURRENT_STATE.REGS[Op2];
			}
			else if (OpCode==13) //MOV
			{
				NEXT_STATE.REGS[Rd]= CURRENT_STATE.REGS[Op2] ;   
			}
			else if (OpCode==14) //BIC
			{
				NEXT_STATE.REGS[Rd]= CURRENT_STATE.REGS[Op1] & (~CURRENT_STATE.REGS[Op2]);
			}
			else if (OpCode==15) //MVN
			{
				NEXT_STATE.REGS[Rd]= CURRENT_STATE.REGS[Op2];
			}
			
		}
		
			
		/***********************************/
		/*Other Instruction sets*/
		/* Multiply */
		if (((Instruction>>22) & 15) == 0) 
		{
			if (((Instruction>>4) & 9)==9) // Multiply
			{
				Rd  = (Instruction>>16)& 0x0000000F;

				Op1 = (Instruction & 0x0000000F); //Rm
				
				Op2 = ((Instruction>>8) & 0x0000000F) ; //Rs
				
				Op3 = ((Instruction>>12) & 0x0000000F) ; //Rn
				
				if (((Instruction>>21) & 1) == 0) // MUL
				{
					NEXT_STATE.REGS[Rd]= CURRENT_STATE.REGS[Op1] * Op2 ;
//==== mo'men ====== Update the flags in the CPSR ===== //				
				}
				else if (((Instruction>>21) & 1) == 1) //MULA
				{
					NEXT_STATE.REGS[Rd]= ((CURRENT_STATE.REGS[Op1] * CURRENT_STATE.REGS[Op2]) + CURRENT_STATE.REGS[Op3] ) ;
				}
			}

			
		}
		
		break;
		/******************************************************************************************************************/
		case 1: //Single data transfer
	/*
		Base_Reg= (Instruction>>16) & 0x0000000F;
		Source_Distination_Reg=(Instruction>>12) & 0x0000000F;
	
		if (((Instruction>>25)&1) == 0) //Offset is an immediate value
		{
			Offset_Value= Instruction & 0x00000FFF;
		}
		else if (((Instruction>>25)&1) == 1)
		{
			Offset_Reg= Instruction & 0x0000000F;
			Shift= (Instruction >> 4) & 0x000000FF;
			Shifting part i faild to immplement 
		}
		if (((Instruction>>24)&1) == 0) //Add offset after transfer
		{
			
		}
		else if (((Instruction>>24)&1) == 1) // Add offset before transfer 
		{
			
		}

		if (((Instruction>>23)&1) == 0) //Subtract offset from base
		{
			Value= CURRENT_STATE.REGS[Base_Reg]-Offset_Value;
		}
		else if (((Instruction>>23)&1) == 1) //Add offset to base
		{
			Value= CURRENT_STATE.REGS[Base_Reg]+Offset_Value;
		}
		
		if (((Instruction>>22)&1) == 0) //Transfer word quality
		{
			
		}
		else if (((Instruction>>23)&1) == 1) //Transfer byte quality
		{
			
		}
		
		if (((Instruction>>21)&1) == 0) //No write back
		{
			
		}
		else if (((Instruction>>21)&1) == 1) //Write address into base
		{
			
		}
		
		if (((Instruction>>20)&1) == 0) //Store to memory
		{
			mem_write_32(CURRENT_STATE.REGS[Source_Distination_Reg],Value);
		}
		else if (((Instruction>>20)&1) == 1) //Load from memory
		{
			NEXT_STATE.REGS[Source_Distination_Reg]= mem_read_32(CURRENT_STATE.REGS[Source_Distination_Reg]);
		}
		
		break;
	*/
		/******************************************************************************************************************/
		case 2: //Branching
//==== mo'men ====== Excellent ===== //				

			if (((Instruction>>24)&1)==1) //Branch with link
			{
				NEXT_STATE.REGS[14] = CURRENT_STATE.PC; // To keep the previous value of the PC to return to if needed
				CURRENT_STATE.REGS[14] = (CURRENT_STATE.REGS[14] << 2); //Clearing R14 [1:0]
//==== mo'men ====== you should update the next state link register ===== //				
				int32_t Branch= Instruction & 0x00FFFFFF;
				
				Branch = (Branch << 2);  
				
				if (((Branch>>25)&1)==1)
				{
					Branch = Branch | 0xFC000000;
					NEXT_STATE.PC = CURRENT_STATE.PC + Branch + 8;
				}
				else 
				{
					Branch = Branch | 0x00000000;
					NEXT_STATE.PC = CURRENT_STATE.PC + Branch + 8;
				}
			}
			else                        //Branch
			{
				int32_t Branch= Instruction & 0x00FFFFFF;
				
				Branch = (Branch << 2);  
				
				if (((Branch>>25)&1)==1)
				{
					Branch = Branch | 0xFC000000;
					NEXT_STATE.PC = CURRENT_STATE.PC + Branch + 8;
				}
				else 
				{
					Branch = Branch | 0x00000000;
					NEXT_STATE.PC = CURRENT_STATE.PC + Branch + 8;
				}
			}
		
		break;
		/******************************************************************************************************************/
		case 3:
//==== mo'men ====== Perfect ===== //				
		
			if ((Instruction&0x000000FF)==10) //Software interrupt
			{
				RUN_BIT=0;
			
			}
		
		break;
		
		
		
	}		
	 
}
