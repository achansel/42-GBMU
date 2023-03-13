/*
	ALL THE CONTROL INSTRUCTIONS
*/
enum class Cond {
	NONE, NC, NZ, C, Z
};

template<Cond c>
ALWAYS_INLINE bool CHECK_COND()
{
	if constexpr (c == Cond::NONE)
		return (true);
	else if constexpr (c == Cond::NC)
		return (!GET_FLAG(CarryFlag));
	else if constexpr (c == Cond::NZ)
		return (!GET_FLAG(ZeroFlag));
	else if constexpr (c == Cond::C)
		return (GET_FLAG(CarryFlag));
	else
		return (GET_FLAG(ZeroFlag));
}

ALWAYS_INLINE void _JUMP(u16 address)	{ PC = address; m_tclock += 4; }
ALWAYS_INLINE void _RET()				{ u16 sp = GET_COMPOSED_REG(RegisterSP); PC = GET_WORD(sp); SET_COMPOSED_REG(RegisterSP, sp + 2); }
ALWAYS_INLINE void _CALL(u16 address)	{ u16 sp = GET_COMPOSED_REG(RegisterSP) - 2; WRITE_WORD(sp, PC); SET_COMPOSED_REG(RegisterSP, sp); _JUMP(address); }

template<Cond cond = Cond::NONE>
void CALL_IMM16()			{ u16 a = FETCH_WORD(); if (CHECK_COND<cond>()) _CALL(a); }

template<u16 address>
void RST()					{ _CALL(address); }

/* TODO: FIX TIMINGS */
template<Cond cond = Cond::NONE>
void JUMP_IMM16()			{ u16 a = FETCH_WORD(); if (CHECK_COND<cond>()) _JUMP(a); }
void JUMP_HL()				{ _JUMP(GET_COMPOSED_REG(RegisterHL)); }

template<Cond cond = Cond::NONE>
void JR_IMM8()				{ s8 a = static_cast<s8>(FETCH_BYTE()); if (CHECK_COND<cond>()) _JUMP(PC + a); }

template<Cond cond = Cond::NONE>
void RET()					{ m_tclock += 4; if constexpr (cond != Cond::NONE) m_tclock += 4; if (CHECK_COND<cond>()) _RET(); }
void RETI()					{ m_tclock += 4; _RET(); m_ime = true; }

