#include <Emulator/CPU/CPU.hpp>
#include <Emulator/CPU/InstrCommon.hpp>

/*
	MICRO-INSTRUCTIONS (if its called this way)
*/
ALWAYS_INLINE void CPU::MOV_ADDR_8(u16 address, u8 value)	{ m_tclock += 4; PC++; this->WRITE_BYTE(address, value); }
ALWAYS_INLINE void CPU::MOV_ADDR_16(u16 address, u16 value)	{ m_tclock += 4; PC++; this->WRITE_WORD(address, value); }
ALWAYS_INLINE void CPU::MOV_REG_8(u8 reg, u8 value)			{ m_tclock += 4; PC++; this->SET_REG(reg, value); }
ALWAYS_INLINE void CPU::MOV_REG_16(u8 reg, u16 value)		{ m_tclock += 4; PC++; this->SET_COMPOSED_REG(reg, value); }

ALWAYS_INLINE void CPU::MOV_ADDR_REG8(u16 address, u8 reg)  { MOV_ADDR_8(address, this->GET_REG(reg)); }
ALWAYS_INLINE void CPU::MOV_ADDR_REG16(u16 address, u8 reg)	{ MOV_ADDR_16(address, this->GET_COMPOSED_REG(reg)); }
ALWAYS_INLINE void CPU::MOV_REG8_REG8(u8 dst, u8 src)		{ MOV_REG_8(dst, this->GET_REG(src)); }
ALWAYS_INLINE void CPU::MOV_REG16_REG16(u8 dst, u8 src)		{ MOV_REG_16(dst, this->GET_COMPOSED_REG(src));}

ALWAYS_INLINE void CPU::MOV_REG8_IMM8(u8 dst)				{ MOV_REG_8(dst, this->FETCH_BYTE(PC + 1)); }
ALWAYS_INLINE void CPU::MOV_REG16_IMM16(u8 dst)				{ MOV_REG_16(dst, this->FETCH_WORD(PC + 1)); }

ALWAYS_INLINE void CPU::MOV_REG8_ADDR(u8 dst, u16 address)	{ MOV_REG_8(dst, this->GET_BYTE(address)); }

ALWAYS_INLINE void CPU::POP_REG16(u8 dst)					{ u16 sp = GET_COMPOSED_REG(RegisterSP); MOV_REG_16(dst, this->GET_WORD(sp)); sp += 2; SET_COMPOSED_REG(RegisterSP, sp); }
ALWAYS_INLINE void CPU::PUSH_REG16(u8 src)					{ u16 sp = GET_COMPOSED_REG(RegisterSP) - 2; SET_COMPOSED_REG(RegisterSP, sp); m_tclock += 4; MOV_ADDR_REG16(sp, GET_COMPOSED_REG(src)); }

/*
	ALL THE LD INSTRUCTIONS
*/
void CPU::LD_BC_d16()		{ MOV_REG16_IMM16(RegisterBC); }
void CPU::LD_DE_d16()		{ MOV_REG16_IMM16(RegisterDE); }
void CPU::LD_HL_d16()		{ MOV_REG16_IMM16(RegisterHL); }
void CPU::LD_SP_d16()		{ MOV_REG16_IMM16(RegisterSP); }

void CPU::LD_ADDR_BC_A()	{ MOV_ADDR_REG8(GET_COMPOSED_REG(RegisterBC), RegisterA); }
void CPU::LD_ADDR_DE_A()	{ MOV_ADDR_REG8(GET_COMPOSED_REG(RegisterDE), RegisterA); }
void CPU::LD_ADDR_HLI_A()	{ MOV_ADDR_REG8(GET_COMPOSED_REG(RegisterHL), RegisterA); INC_COMPOSED_REG(RegisterHL); }
void CPU::LD_ADDR_HLD_A()	{ MOV_ADDR_REG8(GET_COMPOSED_REG(RegisterHL), RegisterA); DEC_COMPOSED_REG(RegisterHL); }

void CPU::LD_B_d8()			{ MOV_REG8_IMM8(RegisterB); }
void CPU::LD_D_d8()			{ MOV_REG8_IMM8(RegisterD); }
void CPU::LD_H_d8()			{ MOV_REG8_IMM8(RegisterH); }
void CPU::LD_ADDR_HL_d8()	{ MOV_ADDR_8(GET_COMPOSED_REG(RegisterHL), FETCH_BYTE(PC + 1)); }

void CPU::LD_ADDR_d16_SP()  { MOV_ADDR_16(FETCH_WORD(PC + 1), GET_COMPOSED_REG(RegisterSP)); }

void CPU::LD_A_ADDR_BC()	{ MOV_REG8_ADDR(RegisterA, GET_COMPOSED_REG(RegisterBC)); }
void CPU::LD_A_ADDR_DE()	{ MOV_REG8_ADDR(RegisterA, GET_COMPOSED_REG(RegisterDE)); }
void CPU::LD_A_ADDR_HLI()	{ MOV_REG8_ADDR(RegisterA, GET_COMPOSED_REG(RegisterHL)); INC_COMPOSED_REG(RegisterHL); }
void CPU::LD_A_ADDR_HLD()	{ MOV_REG8_ADDR(RegisterA, GET_COMPOSED_REG(RegisterHL)); DEC_COMPOSED_REG(RegisterHL); }

void CPU::LD_C_d8()			{ MOV_REG8_IMM8(RegisterC); }
void CPU::LD_E_d8()			{ MOV_REG8_IMM8(RegisterE); }
void CPU::LD_L_d8()			{ MOV_REG8_IMM8(RegisterL); }
void CPU::LD_A_d8()			{ MOV_REG8_IMM8(RegisterA); }

void CPU::LD_A_A()			{ MOV_REG8_REG8(RegisterA, RegisterA); }
void CPU::LD_B_A()			{ MOV_REG8_REG8(RegisterB, RegisterA); }
void CPU::LD_C_A()			{ MOV_REG8_REG8(RegisterC, RegisterA); }
void CPU::LD_D_A()			{ MOV_REG8_REG8(RegisterD, RegisterA); }
void CPU::LD_E_A()			{ MOV_REG8_REG8(RegisterE, RegisterA); }
void CPU::LD_H_A()			{ MOV_REG8_REG8(RegisterH, RegisterA); }
void CPU::LD_L_A()			{ MOV_REG8_REG8(RegisterL, RegisterA); }
void CPU::LD_ADDR_HL_A()	{ MOV_ADDR_REG8(GET_COMPOSED_REG(RegisterHL), RegisterA); }

void CPU::LD_A_B()			{ MOV_REG8_REG8(RegisterA, RegisterB); }
void CPU::LD_B_B()			{ MOV_REG8_REG8(RegisterB, RegisterB); }
void CPU::LD_C_B()			{ MOV_REG8_REG8(RegisterC, RegisterB); }
void CPU::LD_D_B()			{ MOV_REG8_REG8(RegisterD, RegisterB); }
void CPU::LD_E_B()			{ MOV_REG8_REG8(RegisterE, RegisterB); }
void CPU::LD_H_B()			{ MOV_REG8_REG8(RegisterH, RegisterB); }
void CPU::LD_L_B()			{ MOV_REG8_REG8(RegisterL, RegisterB); }
void CPU::LD_ADDR_HL_B()	{ MOV_ADDR_REG8(GET_COMPOSED_REG(RegisterHL), RegisterB); }

void CPU::LD_A_C()			{ MOV_REG8_REG8(RegisterA, RegisterC); }
void CPU::LD_B_C()			{ MOV_REG8_REG8(RegisterB, RegisterC); }
void CPU::LD_C_C()			{ MOV_REG8_REG8(RegisterC, RegisterC); }
void CPU::LD_D_C()			{ MOV_REG8_REG8(RegisterD, RegisterC); }
void CPU::LD_E_C()			{ MOV_REG8_REG8(RegisterE, RegisterC); }
void CPU::LD_H_C()			{ MOV_REG8_REG8(RegisterH, RegisterC); }
void CPU::LD_L_C()			{ MOV_REG8_REG8(RegisterL, RegisterC); }
void CPU::LD_ADDR_HL_C()	{ MOV_ADDR_REG8(GET_COMPOSED_REG(RegisterHL), RegisterC); }

void CPU::LD_A_D()			{ MOV_REG8_REG8(RegisterA, RegisterD); }
void CPU::LD_B_D()			{ MOV_REG8_REG8(RegisterB, RegisterD); }
void CPU::LD_C_D()			{ MOV_REG8_REG8(RegisterC, RegisterD); }
void CPU::LD_D_D()			{ MOV_REG8_REG8(RegisterD, RegisterD); }
void CPU::LD_E_D()			{ MOV_REG8_REG8(RegisterE, RegisterD); }
void CPU::LD_H_D()			{ MOV_REG8_REG8(RegisterH, RegisterD); }
void CPU::LD_L_D()			{ MOV_REG8_REG8(RegisterL, RegisterD); }
void CPU::LD_ADDR_HL_D()	{ MOV_ADDR_REG8(GET_COMPOSED_REG(RegisterHL), RegisterD); }

void CPU::LD_A_E()			{ MOV_REG8_REG8(RegisterA, RegisterE); }
void CPU::LD_B_E()			{ MOV_REG8_REG8(RegisterB, RegisterE); }
void CPU::LD_C_E()			{ MOV_REG8_REG8(RegisterC, RegisterE); }
void CPU::LD_D_E()			{ MOV_REG8_REG8(RegisterD, RegisterE); }
void CPU::LD_E_E()			{ MOV_REG8_REG8(RegisterE, RegisterE); }
void CPU::LD_H_E()			{ MOV_REG8_REG8(RegisterH, RegisterE); }
void CPU::LD_L_E()			{ MOV_REG8_REG8(RegisterL, RegisterE); }
void CPU::LD_ADDR_HL_E()	{ MOV_ADDR_REG8(GET_COMPOSED_REG(RegisterHL), RegisterE); }

void CPU::LD_A_H()			{ MOV_REG8_REG8(RegisterA, RegisterH); }
void CPU::LD_B_H()			{ MOV_REG8_REG8(RegisterB, RegisterH); }
void CPU::LD_C_H()			{ MOV_REG8_REG8(RegisterC, RegisterH); }
void CPU::LD_D_H()			{ MOV_REG8_REG8(RegisterD, RegisterH); }
void CPU::LD_E_H()			{ MOV_REG8_REG8(RegisterE, RegisterH); }
void CPU::LD_H_H()			{ MOV_REG8_REG8(RegisterH, RegisterH); }
void CPU::LD_L_H()			{ MOV_REG8_REG8(RegisterL, RegisterH); }
void CPU::LD_ADDR_HL_H()	{ MOV_ADDR_REG8(GET_COMPOSED_REG(RegisterHL), RegisterH); }

void CPU::LD_A_L()			{ MOV_REG8_REG8(RegisterA, RegisterL); }
void CPU::LD_B_L()			{ MOV_REG8_REG8(RegisterB, RegisterL); }
void CPU::LD_C_L()			{ MOV_REG8_REG8(RegisterC, RegisterL); }
void CPU::LD_D_L()			{ MOV_REG8_REG8(RegisterD, RegisterL); }
void CPU::LD_E_L()			{ MOV_REG8_REG8(RegisterE, RegisterL); }
void CPU::LD_H_L()			{ MOV_REG8_REG8(RegisterH, RegisterL); }
void CPU::LD_L_L()			{ MOV_REG8_REG8(RegisterL, RegisterL); }
void CPU::LD_ADDR_HL_L()	{ MOV_ADDR_REG8(GET_COMPOSED_REG(RegisterHL), RegisterL); }

void CPU::LD_A_ADDR_HL()	{ MOV_REG8_ADDR(RegisterA, GET_COMPOSED_REG(RegisterHL)); }
void CPU::LD_B_ADDR_HL()	{ MOV_REG8_ADDR(RegisterB, GET_COMPOSED_REG(RegisterHL)); }
void CPU::LD_C_ADDR_HL()	{ MOV_REG8_ADDR(RegisterC, GET_COMPOSED_REG(RegisterHL)); }
void CPU::LD_D_ADDR_HL()	{ MOV_REG8_ADDR(RegisterD, GET_COMPOSED_REG(RegisterHL)); }
void CPU::LD_E_ADDR_HL()	{ MOV_REG8_ADDR(RegisterE, GET_COMPOSED_REG(RegisterHL)); }
void CPU::LD_H_ADDR_HL()	{ MOV_REG8_ADDR(RegisterH, GET_COMPOSED_REG(RegisterHL)); }
void CPU::LD_L_ADDR_HL()	{ MOV_REG8_ADDR(RegisterL, GET_COMPOSED_REG(RegisterHL)); }

void CPU::LDH_IMM8_A()		{ MOV_ADDR_REG8(0xFF00 + FETCH_BYTE(PC + 1), RegisterA); }
void CPU::LDH_A_IMM8()		{ MOV_REG8_ADDR(RegisterA, 0xFF00 + FETCH_BYTE(PC + 1)); }

void CPU::LD_ADDR_C_A()		{ MOV_ADDR_REG8(0xFF00 + GET_REG(RegisterC), RegisterA); }
void CPU::LD_A_ADDR_C()		{ MOV_REG8_ADDR(RegisterA, 0xFF00 + GET_REG(RegisterC)); }

void CPU::LD_HL_SP_X_IMM8()
{
	m_tclock += 4;

	u32 n = GET_COMPOSED_REG(RegisterSP) + static_cast<s8>(FETCH_BYTE(PC + 1));
	SET_FLAG(ZeroFlag, 0);
	SET_FLAG(SubstractFlag, 0);
    SET_FLAG(HalfCarryFlag, !!(n & 0x100));
    SET_FLAG(CarryFlag,		!!(n & 0x10000));

	MOV_REG_16(RegisterHL, static_cast<u16>(n));
}
void CPU::LD_SP_HL()		{ MOV_REG16_REG16(RegisterSP, RegisterHL); m_tclock += 4; }	

void CPU::LD_ADDR_IMM16_A()	{ MOV_ADDR_REG8(FETCH_WORD(PC + 1), RegisterA); }
void CPU::LD_A_ADDR_IMM16()	{ MOV_REG8_ADDR(RegisterA, FETCH_WORD(PC + 1)); }

void CPU::POP_BC()			{ POP_REG16(RegisterBC); }
void CPU::POP_DE()			{ POP_REG16(RegisterDE); }
void CPU::POP_HL()			{ POP_REG16(RegisterHL); }
void CPU::POP_AF()			{ POP_REG16(RegisterAF); }
void CPU::PUSH_BC()			{ PUSH_REG16(RegisterBC); }
void CPU::PUSH_DE()			{ PUSH_REG16(RegisterDE); }
void CPU::PUSH_HL()			{ PUSH_REG16(RegisterHL); }
void CPU::PUSH_AF()			{ PUSH_REG16(RegisterAF); }