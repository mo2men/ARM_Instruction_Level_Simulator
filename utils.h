/*
 * utils.h
 *
 *  Created on: ? —»Ì⁄ «·√Ê·° ???? Â‹
 *      Author: WIN8
 */

#ifndef UTILS_H_
#define UTILS_H_
#define SET_BIT(Reg,BitNum) (Reg)|=(1<<(BitNum))
#define CLR_BIT(Reg,BitNum) (Reg)&=~(1<<(BitNum))
#define TGL_BIT(Reg,BitNum) (Reg)^=(1<<(BitNum))
#define GET_BIT(Reg,BitNum) ((Reg)>>(BitNum))&1
#define SET_HIGH_NIBBLE(Reg) (Reg)|=(0x0f<<4)
#define CLR_HIGH_NIBBLE(Reg) (Reg)&=(0x0f)
#define Set_LOW_Nibble(Reg) (Reg)|=(0x0f)
#define CLR_LOW_Nibble(Reg) (Reg)&=(0x0f<<4)
#define GET_HIGH_NIBBLE(Reg) ((Reg)>>4)&0x0f
#define GET_LOW_NIBBLE(Reg) (Reg)&0x0f
#define ASSIN_HIGH_NIBBLE(Reg,Val) ((Reg)&0x0f)|(((Val)&0x0f)<<4)
#define ASSIN_LOW_NIBBLE(Reg,Val) ((Reg)&0xf0)|((Val)&0x0f)
#endif /* UTILS_H_ */
