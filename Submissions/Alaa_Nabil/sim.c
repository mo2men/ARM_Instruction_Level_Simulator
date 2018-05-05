#include <stdio.h>
#include "shell.h"

//This code is to implement .....

#define pc REGS[15] 
#define linker_Add REGS[14]
#define stack_pointer REGS[13]
#define next_state NEXT_STATE
#define current_state CURRENT_STATE



void process_instruction()
{	//Declaring Variables//

	 uint32_t inst;
	 char cond;
	 char categ;
	 
	 //variables for SWI
	 char bottom_byte;
	 
	 //variables for SDT
	 uint32_t mem_offset; 
	 uint32_t sdt_sd_reg;
	 uint32_t sdt_base_reg;
	 char offset_type;
	 char load_store_bit;
	 char write_back_bit;
	 char byte_word_bit;
	 char up_down_bit;
		// char indexing_bit;
	 uint32_t calculated_mem_add;
	 
	 //variables for B & BL
	 char link_bit;
	 int32_t offset;
	 
	 //variables for Data Processing
	 char opcode;
	 uint32_t dp_dest_reg;
	 uint32_t dp_first_operand;
	 uint32_t dp_second_operand;
	 char immediate_bit;
	 char rotate_value;
	 uint32_t imm;
	 
	 //variables for Multiplication
	 //for signed multiplication
	 int32_t mul_operand_1;  //Rm 
	 int32_t mul_operand_2; //Rs  
	 uint32_t mul_dest_reg; //Rd
	 //ignore Rn
	 char acc_bit;
	 char flags_bit;
	 char negative_flag_bit;
	 char zero_flag_bit;
	 
	 char dp_or_mul_bit;
	 
	 next_state.pc = current_state.pc +4;
	 inst = mem_read_32(current_state.pc); 
	 cond = (inst>>28);
	 categ = ((inst >> 26)&3);
	 if (cond == 0x0E)
	 {
		switch (categ)
		{
		
	 case 0: //Data Processing or Multiplying
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
																		/*Multiplication*/ 
	 dp_or_mul_bit = ((inst>>4)&0x0F); 														
	 acc_bit = ((inst>> 21)&1);
	 mul_dest_reg = (( inst>>16)&0x0F);
	 mul_operand_1 = (inst & 0x0F);
	 mul_operand_2 =  ((inst>>8 )&0x0F);
	 flags_bit = ((inst>>20)&1);
	 negative_flag_bit = ((current_state.CPSR >>31)&1);
	 zero_flag_bit = ((current_state.CPSR >>30)&1);
	if (dp_or_mul_bit == 9)
	{		
	 if (acc_bit == 0) //Multiplying only {MUL instruction}
	 {
		if ((mul_dest_reg != mul_operand_1) && (current_state.pc != mul_dest_reg) && (current_state.pc != mul_operand_1) 
			&& (current_state.pc != mul_operand_2))  //ensuring operands restrictions
		
		{
			next_state.REGS[mul_dest_reg] = mul_operand_1 * mul_operand_2;
		}
		
		
		if(flags_bit == 1)
		{
			if (next_state.REGS[mul_dest_reg] == 0)
			{
				zero_flag_bit |= 1;  //so not sure
//==== mo'men ====== you should set the CPSR ===== //				

			}
			
			negative_flag_bit = ((next_state.REGS[mul_dest_reg]>>31)&1);
		}
		
		
	 }
	}
	
	 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
																/*Data Processing*/
	 else 
	 {
		opcode = ((inst>> 21)& 0x0F); 
		dp_dest_reg = ((inst>>12)&0x0F);
		dp_first_operand = ((inst>>16)&0x0F);
		immediate_bit = ((inst>>25)&1);
		
		if (immediate_bit == 1) //second operand is a rotated immediate value
		{
			rotate_value = ((inst>>8)&0x0F);
			imm = (inst & 0x0FF);
			dp_second_operand = (imm>>rotate_value)|(imm<<(32 - rotate_value)); //from the internet
		}
		else if (immediate_bit ==0)//second operand is a shifted register
		{
			
		}
		
		switch (opcode)
		{
			case (2): //subtraction {SUB instruction}
			
				next_state.REGS[dp_dest_reg] = dp_first_operand - dp_second_operand;
					break;
			
			
			case (4): //Addition {ADD instruction}
			
				next_state.REGS[dp_dest_reg] = dp_first_operand + dp_second_operand;
					break;
			
				
		}
		
	 }
	 
	 
			break;	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
															/*Single Data Transfer*/
	 case 1: 
//==== mo'men ====== This part of the code was almost perfect ===== //				

	 mem_offset = (inst & 0xFFF);
	 sdt_sd_reg = ((inst >>12)&0x0F);
	 sdt_base_reg = ((inst>>16)&0x0F);
	 offset_type = ((inst>>25 )& 1);
	 load_store_bit = ((inst>>20)&1);
	 byte_word_bit = ((inst>>22)&1);
	 up_down_bit =((inst>>23)&1);
	 write_back_bit = ((inst>> 21)&1);
	 
	 switch (up_down_bit)
	 {
		 case 1:
	 calculated_mem_add = sdt_base_reg + mem_offset;
		break;
		case 0:
	 calculated_mem_add = sdt_base_reg - mem_offset;
		break;
	 }
	 
	 if (offset_type == 0) //immediate offset
	 {
		if (load_store_bit == 0)//store 
		{
			if(byte_word_bit == 0)//word store{STR instruction}
			{
				mem_write_32(calculated_mem_add,current_state.REGS[sdt_sd_reg]); 
			}
			else if (byte_word_bit == 1)//byte store {STRB instruction}
			{
				
			}
		}
	
		else if (load_store_bit == 1) //load
		{
			if(byte_word_bit == 0)//word load {LDR instruction}
			{
				next_state.REGS[sdt_sd_reg] = mem_read_32(calculated_mem_add);
			}
			else if (byte_word_bit == 1)//byte load {LDRB instruction}
			{
				
			}
		}
	
			
	 }
	 else if (offset_type == 1)
	 {
		 //don't know
	 }
	 
	 if (write_back_bit == 1)
	 {
		 next_state.REGS[sdt_base_reg] = calculated_mem_add;
	 }
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
	 break;
	 
	 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
														/*Branch and Branch with Link*/
	 case 2:
//==== mo'men ====== Perfect ===== //				

	 link_bit = ((inst >>24)&1);
	 offset = inst & 0x00FFFFFF;
	 offset = offset<<2;
	 //for sign extension
	 if (((offset >>25 )&1) == 1) //-ve offset
	 {
		offset |= 0xFC000000; 
	 }
	 next_state.pc = current_state.pc + offset + 8;
	 if (link_bit == 1) //branch with link instruction {BL} 
	 {
		 next_state.linker_Add = current_state.pc; //putting the old value of pc in inker add reg
	 }
	 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	 break;
	 
	 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
															/*Software Interrupt*/
	 case 3: 
//==== mo'men ====== Perfect ===== //				

		bottom_byte = (inst & 0xFF);
		if (bottom_byte == 0x0A)
		{
			
			RUN_BIT = 0;
		
		}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	 break;
		
	 }
	 
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	 }
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	
}










































														/*TUT*/



    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
	 
	 
	 /*NEXT_STATE.REGS[15] = CURRENT_STATE.REGS[15]+4;
	 uint32_t inst = mem_read_32(CURRENT_STATE.REGS[15]); //READING PC TO KNOW THE ADDRESS OF THE NEXT INSTRUCTION  {TARGET INSTRUCTION}...THE FN RETURNS THE INSTRUCTION ITSELF
	 //TO specify certain bits add with zeros except ones in target bits OR shiftting then and
	 char categ = (inst>>26)&3;
	 int32_t offset = inst & 0x00FFFFFF; //offset is a signed two's complement

	 switch (categ)
	 {
case 0: //data processing or multiply


break;

case 1: //single data transfer


break;

case 2: //branch

offset = (offset<<2);
if (1 ==((offset >>25)&1))
offset |= 0xFC000000;
NEXT_STATE.REGS[15] = CURRENT_STATE.REGS[15]+offset +8;
if (1  == ((inst >>24)&1))//branh with link
NEXT_STATE.REGS[14] = CURRENT_STATE.REGS[15]; //putting the value of current PC in next linker 

break;


case 3: //SWI
	if(10 == (inst&0x000000FF))
	{
		RUN_BIT = 0;
	}

break;	
	 
default:
	printf ("dfghjk");
	 }	 
	 
}
	 */
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	
