ALWAYS_INLINE void MOV_ADDR_8(u16 address, u8 value)	{ this->WRITE_BYTE(address, value); }
ALWAYS_INLINE void MOV_ADDR_16(u16 address, u16 value)	{ this->WRITE_WORD(address, value); }
ALWAYS_INLINE void MOV_REG_8(Register reg, u8 value)			{ this->SET_REG(reg, value); }
ALWAYS_INLINE void MOV_REG_16(ComposedRegister reg, u16 value)		{ this->SET_COMPOSED_REG(reg, value); }

template<u16 a, Register reg>
ALWAYS_INLINE void MOV_ADDR_REG() 	 					{ MOV_ADDR_8(a, this->GET_REG(reg)); }

template<u16 a, ComposedRegister reg>
ALWAYS_INLINE void MOV_ADDR_REG()						{ MOV_ADDR_16(a, this->GET_COMPOSED_REG(reg)); }

template<Register dst, Register src>
ALWAYS_INLINE void MOV_REG_REG()						{ MOV_REG_8(dst, this->GET_REG(src)); }
ALWAYS_INLINE void MOV_REG16_REG16(ComposedRegister dst, ComposedRegister src)		{ MOV_REG_16(dst, this->GET_COMPOSED_REG(src));}

template<Register dst>
ALWAYS_INLINE void MOV_REG_IMM()						{ MOV_REG_8(dst, this->FETCH_BYTE()); }
template<ComposedRegister dst>
ALWAYS_INLINE void MOV_REG_IMM()						{ MOV_REG_16(dst, this->FETCH_WORD()); }

ALWAYS_INLINE void MOV_REG8_ADDR(Register dst, u16 address)	{ MOV_REG_8(dst, this->GET_BYTE(address)); }

template<ComposedRegister dst, Register src>
ALWAYS_INLINE void MOV_ADDR_REG_REG() 					{ MOV_ADDR_8(GET_COMPOSED_REG(dst), GET_REG(src)); }
template<Register dst, Register src>
ALWAYS_INLINE void MOV_ADDR_REG_REG() 					{ MOV_ADDR_8(0xFF00 + GET_REG(dst), GET_REG(src)); }

template<Register dst, ComposedRegister src>
ALWAYS_INLINE void MOV_REG_ADDR_REG() 					{ MOV_REG_8(dst, this->GET_BYTE(GET_COMPOSED_REG(src))); }
template<Register dst, Register src>
ALWAYS_INLINE void MOV_REG_ADDR_REG() 					{ MOV_REG_8(dst, this->GET_BYTE(0xFF00 + GET_REG(src))); }

template<ComposedRegister dst>
ALWAYS_INLINE void POP()
{
	u16 sp = GET_COMPOSED_REG(RegisterSP);
	u16 w = this->GET_WORD(sp);

	if constexpr (dst == RegisterAF)
		w &= 0xFFF0;
	MOV_REG_16(dst, w);
	sp += 2;
	SET_COMPOSED_REG(RegisterSP, sp);
}

template<ComposedRegister src>
ALWAYS_INLINE void PUSH()								{ u16 sp = GET_COMPOSED_REG(RegisterSP) - 2; SET_COMPOSED_REG(RegisterSP, sp); m_tclock += 4; MOV_ADDR_16(sp, this->GET_COMPOSED_REG(src)); }

/*
	ALL THE LD INSTRUCTIONS
*/
void LD_ADDR_HLI_A()	{ MOV_ADDR_8(GET_COMPOSED_REG(RegisterHL), GET_REG(RegisterA)); INC_COMPOSED_REG(RegisterHL); }
void LD_ADDR_HLD_A()	{ MOV_ADDR_8(GET_COMPOSED_REG(RegisterHL), GET_REG(RegisterA)); DEC_COMPOSED_REG(RegisterHL); }
void LD_ADDR_HL_IMM8()	{ MOV_ADDR_8(GET_COMPOSED_REG(RegisterHL), FETCH_BYTE()); }
void LD_ADDR_IMM16_SP()	{ MOV_ADDR_16(FETCH_WORD(), GET_COMPOSED_REG(RegisterSP)); }
void LD_A_ADDR_HLI()	{ MOV_REG_ADDR_REG<RegisterA, RegisterHL>(); INC_COMPOSED_REG(RegisterHL); }
void LD_A_ADDR_HLD()	{ MOV_REG_ADDR_REG<RegisterA, RegisterHL>(); DEC_COMPOSED_REG(RegisterHL); }
void LD_HL_SP_X_IMM8()
{
	m_tclock += 4;

	u32 n = GET_COMPOSED_REG(RegisterSP) + static_cast<s8>(FETCH_BYTE());
	SET_FLAG(ZeroFlag, 0);
	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, !!(n & 0x10));
	SET_FLAG(CarryFlag,		!!(n & 0x100));

	MOV_REG_16(RegisterHL, static_cast<u16>(n));
}
void LD_SP_HL()			{ MOV_REG16_REG16(RegisterSP, RegisterHL); m_tclock += 4; }	

void LDH_IMM8_A()		{ MOV_ADDR_8(0xFF00 + FETCH_BYTE(), GET_REG(RegisterA)); }
void LDH_A_IMM8()		{ MOV_REG8_ADDR(RegisterA, 0xFF00 + FETCH_BYTE()); }

void LD_ADDR_IMM16_A()	{ MOV_ADDR_8(FETCH_WORD(), GET_REG(RegisterA)); }
void LD_A_ADDR_IMM16()	{ MOV_REG8_ADDR(RegisterA, FETCH_WORD()); }