#include <stdio.h>
#include "shell.h"
#define Offset 0x00FFFFFF
/********************************************
*                 Microprocessor project    *
* prepared by                               *
* Hassan Ashraf 20146404                    *
* Sameh  Ahmed  2014638                     *
*                      Dr/ Mohamed Helmy    *
*					  Eng. mommen abdelhady *
********************************************/           

void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */


	 NEXT_STATE.REGS[15] = CURRENT_STATE.REGS + 4 ; // Increment Program Counter


	 uint32_t Instruction = mem_read_32(CURRENT_STATE.REGS[15]); //Reading Instruction from memory
      int cond = 1 ;
      int Categ =((Instruction >> 26) & 3); // To know which instruction set format
      uint32_t N
      switch (Instruction >>28)
//==== mo'men ====== Checking condition is perfect ===== //				

      case 0 : //EQ

          if(((CURRENT_STATE.CPSR >> 30) & 1 ) == 1 ) {cond = 0; }
          break ;



      case 1 :  //NE

          if(((CURRENT_STATE.CPSR >> 30) & 1 ) == 0 ) {cond = 0; }
          break ;

      case 10 :  //GE

          if(((CURRENT_STATE.CPSR >> 30) & 1 ) == ((CURRENT_STATE.CPSR >> 28) & 1 ) ) {cond = 0; }
          break ;


      case 11 :  //LT

          if(((CURRENT_STATE.CPSR >> 30) & 1 ) != ((CURRENT_STATE.CPSR >> 28) & 1 ) ) {cond = 0; }
          break ;

      case 12 :  //GT

          if( (((CURRENT_STATE.CPSR >> 30) & 1 ) == 0 ) && (((CURRENT_STATE.CPSR >> 30) & 1 ) == ((CURRENT_STATE.CPSR >> 28) & 1 ) ) ) {cond = 0; }
          break ;

      case 13 :  //LE

          if( (((CURRENT_STATE.CPSR >> 30) & 1 ) == 1 ) || (((CURRENT_STATE.CPSR >> 30) & 1 ) != ((CURRENT_STATE.CPSR >> 28) & 1 ) ) ) {cond = 0; }
          break ;

      case 14 :  //AL

           cond = 0;

          break ;


	if(cond == 0) {
        switch (Categ)
	  {

    case 0: //Data Processing OR MUL/MLA
                  uint32_t OP_2 ;
                  uint32_t Rm ;
                  uint32_t RmOP_value  ;
                  uint32_t Rs ;
                  uint32_t RsOP_value  ;
                  uint32_t Rn ;
                  uint32_t RnOP_value ;
                  uint32_t Rd ;
                  uint32_t Rd_Destination_value ;
                  uint32_t S ;
                  uint32_t A_bit ;
                  uint32_t I ;
                  uint32_t Immediate ;
                  uint32_t Shift_Amount ;
                  uint32_t Shift_Amount_address ;
                  uint32_t Shift_Type ;
                  uint32_t Latte ;
                  uint32_t OpCode;
                  uint32_t Address ;

	        int DP_or_MUL = ((Instruction >> 4) & 0x0000000F);

	        if (DP_or_MUL == 9 ) //Multiplication
	    {
//==== mo'men ====== Multiplication is Excellent ===== //				
//==== mo'men ====== However, you forgot to check the Set flag bit ===== //				
//==== mo'men ====== and updating the CPSR flags ===== //				

	               Rm = (Instruction & 0x0000000F);
                   RmOP_value = CURRENT_STATE.REGS[Rm] ;
                   Rs = ((Instruction >> 8)& 0x0000000F);
                   RsOP_value = CURRENT_STATE.REGS[Rs] ;
                   Rn = ((Instruction >> 12)& 0x0000000F);
                   Rd = ((Instruction >> 16)& 0x0000000F);
                   A_bit = ((Instruction >> 21) & 1);
	              if (A_bit == 0) //Multiply only
                    {

                   Rn = Rn & 0 ;   // SETTING Rn To Zero
                  NEXT_STATE.REGS[Rd] =  (RmOP_value * RsOP_value) + Rn ;

	              }
	              else    //MLA
                    {

                  NEXT_STATE.REGS[Rd] =  (RmOP_value * RsOP_value) + Rn ;

	              }
	    }

	    else     // Data Processing
            {

                Rn = ((Instruction >> 16) & 0x0000000F); //OP1
                Rd = ((Instruction >> 12) & 0x0000000F); //DESTINATION
                RnOP_value = CURRENT_STATE.REGS[Rn];

               I = ((Instruction >> 25)& 1) ;
              if (I = 1) //OP2 is Imm
             {
                  Immediate = (Instruction & 0x000000FF);
                  Rot = ((Instruction >> 8) & 0x0000000F);
                  if (Rot != 0) { Immediate = (Immediate << (32 - 2*Rot)); } //BUG
//==== mo'men ====== This will not implement a rotate ===== //				
                  OP_2 = Immediate ;
             }
             else //OP2 is REG
                {
                   Rm = (Instruction & 0x0000000F);
                   OP_2 = CURRENT_STATE.REGS[Rm] ;

                   if (((Instruction >> 4) & 1) == 0 ){

                        Shift_Amount = ((Instruction >> 7) & 31);

                   } else {
                   Shift_Amount_address = ((Instruction >> 8) & 0x0000000F);
                   Shift_Amount = CURRENT_STATE.REGS[Shift_Amount_address];
//==== mo'men ====== Well, not the whole register used in shift only least significant byte ===== //				

                   }

                   Latte = CURRENT_STATE.REGS[Rm];
                   switch (Shift_Type) {
//==== mo'men ====== Shift_Type only declared not defined before ????!!!!!!!! ===== //				

                   case 0 : OP_2 = Latte << Shift_Amount ; break;
                   case 1 : OP_2 = Latte >> Shift_Amount ; break;
                   case 2 :

                       if((Latte >> 31 ) == 1) { OP_2 = (Latte >> Shift_Amount) | 0xFC000000; }
//==== mo'men ====== Sign reserving depend on the amount of shift not const 0xFC000000 ===== //				

                       else { OP_2 = Latte >> Shift_Amount ; }
                        break;
                   case 3 :  if(Shift_Amount != 0) OP_2 = Latte << (32 - Shift_Amount) ; //BUG slide 14
//==== mo'men ====== This won't implement a rotate ===== //				
					   }
                 }
            }
           //OPCode

           OpCode = (Instruction >> 21 ) & 0x0000000F );
           if (((Instruction >> 20)& 1) == 0)
         {

           switch (OpCode)
           {

       case 0 : //AND
           NEXT_STATE.REGS[Rd] = RnOP_value & OP_2 ;

       case 1 : //EOR
           NEXT_STATE.REGS[Rd] = RnOP_value ^ OP_2 ;

       case 2 : //SUB
           NEXT_STATE.REGS[Rd] = RnOP_value - OP_2 ;

       case 3 : //RSB
           NEXT_STATE.REGS[Rd] = OP_2 - RnOP_value ;

       case 4 : //ADD
           NEXT_STATE.REGS[Rd] = RnOP_value + OP_2 ;

       case 5 : //ADC
           NEXT_STATE.REGS[Rd] = RnOP_value + OP_2 + ((CURRENT_STATE.CPSR >> 29) & 1) ;

       case 6 : //SBC
           NEXT_STATE.REGS[Rd] = RnOP_value - OP_2 + ((CURRENT_STATE.CPSR >> 29) & 1) - 1 ;

       case 7 : //RSB
           NEXT_STATE.REGS[Rd] = OP_2 - RnOP_value + ((CURRENT_STATE.CPSR >> 29) & 1) - 1 ;

       case 12 : //ORR
           NEXT_STATE.REGS[Rd] = RnOP_value | OP_2  ;

       case 13 : //MOV
           NEXT_STATE.REGS[Rd] =  OP_2  ;

       case 14 : //BIC
           NEXT_STATE.REGS[Rd] = RnOP_value & ( ! OP_2)  ;

       case 15 : //MVN
           NEXT_STATE.REGS[Rd] =  ! OP_2  ;

           }
         }
        else if (((Instruction >> 20)& 1) == 1){
//==== mo'men ====== Oh, the set flage for the opcode in the if not the ones in this else if branch ===== //				
//==== mo'men ====== TST, TEQ , CMP, CMN always sets the status flages ===== //				

                switch (OpCode) {
//==== mo'men ====== Well you didn't update the V, C flags :( ===== //				

       case 8 : //TST
           NEXT_STATE.REGS[Rd] = RnOP_value & OP_2 ;
//==== mo'men ====== You shouldn't update any registers, only update the CPSR ===== //				

           if ((CURRENT_STATE.REGS[Rd] >> 31) == 1 )  //N Flag
//==== mo'men ====== Check the result of the TST, not what was saved befor ===== //				
//==== mo'men ====== you corrected this in TEQ, CMP and CMN, so good ===== //				

            { (NEXT_STATE.CPSR = CURRENT_STATE.CPSR | 0x80000000 ); } //N Flag
        if (NEXT_STATE.REGS[Rd] = 0){(NEXT_STATE.CPSR = CURRENT_STATE.CPSR |  0x40000000 ) ;} //Z Flag

       case 9 : //TEQ
           NEXT_STATE.REGS[Rd] = RnOP_value ^ OP_2 ;
           if ((CURRENT_STATE.REGS[Rd] >> 31) == 1 )  //N Flag
            { (NEXT_STATE.CPSR = CURRENT_STATE.CPSR | 0x80000000 ); } //N Flag
        if (NEXT_STATE.REGS[Rd] = 0){(NEXT_STATE.CPSR = CURRENT_STATE.CPSR |  0x40000000 ) ;} //Z Flag


       case 10 : //CMP
           NEXT_STATE.REGS[Rd] = RnOP_value - OP_2 ;
           if ((CURRENT_STATE.REGS[Rd] >> 31) == 1 )  //N Flag
            { (NEXT_STATE.CPSR = CURRENT_STATE.CPSR | 0x80000000 ); } //N Flag
        if (NEXT_STATE.REGS[Rd] = 0){(NEXT_STATE.CPSR = CURRENT_STATE.CPSR |  0x40000000 ) ;} //Z Flag


       case 11 : //CMN
           NEXT_STATE.REGS[Rd] = RnOP_value + OP_2 ;
        if ((CURRENT_STATE.REGS[Rd] >> 31) == 1 )  //N Flag
            { (NEXT_STATE.CPSR = CURRENT_STATE.CPSR | 0x80000000 ); } //N Flag
        if (NEXT_STATE.REGS[Rd] = 0){(NEXT_STATE.CPSR = CURRENT_STATE.CPSR |  0x40000000 ) ;} //Z Flag

        }
        }

	  }

	    //-------------------------------------------------------------------------------------------------------------------//

    case 1: //Single Data Transfer

    I = ((Instruction >> 25)& 1) ;
              if (I == 0)  //Offset is Imm
              {
                  Offset = ((Instruction) & 0x00000FFF )
              }
              else
                 {

                  Rm = (Instruction & 0x0000000F);
                  Offset = CURRENT_STATE.REGS[Rm] ;

                   if (((Instruction >> 4) & 1) == 0 ){

                        Shift_Amount = ((Instruction >> 7) & 31);

                   } else {

                   Shift_Amount_address = ((Instruction >> 8) & 0x0000000F);
                   Shift_Amount = CURRENT_STATE.REGS[Shift_Amount_address];
                   }

                   Latte = CURRENT_STATE.REGS[Rm];
                   switch (Shift_Type) {
                   case 0 : Offset = Latte << Shift_Amount ; break;
                   case 1 : Offset = Latte >> Shift_Amount ; break;
                   case 2 :

                       if((Latte >> 31 ) == 1) { OP_2 = (Latte >> Shift_Amount) | 0xFC000000; }
                       else { Offset = Latte >> Shift_Amount ; }
                        break;
                   case 3 : if(Shift_Amount != 0) Offset = Latte << (32 - Shift_Amount) ; //BUG slide 14
                       }
                 }
             Rn = ((Instruction >> 16) & 0x0000000F );




                 if (((Instruction >> 23) & 1) == 1 ) //U is 1 :  Base + Offset
                  {
                     Address = CURRENT_STATE[Rn] + Offset ;
                  }
                 else  //U is 0 : Base - Offset
                  {
                     Address = CURRENT_STATE[Rn] - Offset ;
                  }
    Rd = ((Instruction) >> 12 & 0x0000000F );
    if (((Instruction >> 20 ) & 1 ) == 0 )  { mem_write_32(Address,Rd) } //Store



else NEXT_STATE.REGS[Rd] = mem_read_32(Address) ;



        break;

       //-------------------------------------------------------------------------------------------------------------------//

    case 2: //Branching
//==== mo'men ====== Excellent ===== //				
			if ((( Instruction >> 24) & 1 ) == 1 ) //Branch with link
			{
				NEXT_STATE.REGS[14] = CURRENT_STATE.REGS[15]; // To keep the previous value of the PC to return to if needed
				CURRENT_STATE.REGS[14] = ( CURRENT_STATE.REGS[14] << 2); //Clearing R14 [1:0]
//==== mo'men ====== you should update the next state link register ===== //				

				int32_t Offset = Instruction & 0x00FFFFFF;

				Offset = (Offset << 2);

				if (((Offset>>25)&1)==1)
				{
					Offset = Offset | 0xFC000000;
					NEXT_STATE.REGS[15] = CURRENT_STATE.REGS[15] + Offset + 8;
				}
				else
				{
					Offset = Offset | 0x00000000;
					NEXT_STATE.REGS[15] = CURRENT_STATE.REGS[15] + Offset + 8;
				}
			}
			else                        //Branch
			{
				int32_t Offset = Instruction & 0x00FFFFFF;

				Offset = ( Offset << 2);

				if (((Offset>>25)&1)==1)
				{
					Offset = Offset | 0xFC000000;
					NEXT_STATE.REGS[15] = CURRENT_STATE.REGS[15] + Offset + 8;
				}
				else
				{
					Offset = Offset | 0x00000000;
					NEXT_STATE.REGS[15] = CURRENT_STATE.REGS[15] + Offset + 8;
				}
			}

		break;
		//-------------------------------------------------------------------------------------------------------------------//
		case 3:
//==== mo'men ====== Perfect ===== //				

			if ((Instruction & 0x000000FF)==10) //SWI
			{
				RUN_BIT = 0 ;

			}

		break;


	}
    }
	}



