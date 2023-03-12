#include <Emulator/CPU/CPU.hpp>
#include <Emulator/CPU/Instructions/InstrCommon.hpp>

/*
	MICRO-INSTRUCTIONS (if its called this way)
*/
ALWAYS_INLINE void CPU::_JUMP(u16 address)	{ m_tclock += 4; PC = address; }
ALWAYS_INLINE void CPU::_RET()				{ m_tclock += 4; u16 sp = GET_COMPOSED_REG(RegisterSP); PC = GET_WORD(sp); SET_COMPOSED_REG(RegisterSP, sp + 2); }
ALWAYS_INLINE void CPU::_CALL(u16 address)	{ u16 sp = GET_COMPOSED_REG(RegisterSP) - 2; WRITE_WORD(sp, PC); SET_COMPOSED_REG(RegisterSP, sp); _JUMP(address); }

/*
	ALL THE CONTROL INSTRUCTIONS
*/
void CPU::CALL_IMM16()			{ PC++; m_tclock += 4; u16 a = FETCH_WORD(PC); _CALL(a); }
void CPU::CALL_NC_IMM16()		{ PC++; m_tclock += 4; u16 a = FETCH_WORD(PC); if (!GET_FLAG(CarryFlag)) _CALL(a); }
void CPU::CALL_NZ_IMM16()		{ PC++; m_tclock += 4; u16 a = FETCH_WORD(PC); if (!GET_FLAG(ZeroFlag)) _CALL(a); }
void CPU::CALL_C_IMM16()		{ PC++; m_tclock += 4; u16 a = FETCH_WORD(PC); if (GET_FLAG(CarryFlag)) _CALL(a); }
void CPU::CALL_Z_IMM16()		{ PC++; m_tclock += 4; u16 a = FETCH_WORD(PC); if (GET_FLAG(ZeroFlag)) _CALL(a); }

void CPU::RST_00()				{ PC++; m_tclock += 4; _CALL(0x0000); }
void CPU::RST_08()				{ PC++; m_tclock += 4; _CALL(0x0008); }
void CPU::RST_10()				{ PC++; m_tclock += 4; _CALL(0x0010); }
void CPU::RST_18()				{ PC++; m_tclock += 4; _CALL(0x0018); }
void CPU::RST_20()				{ PC++; m_tclock += 4; _CALL(0x0020); }
void CPU::RST_28()				{ PC++; m_tclock += 4; _CALL(0x0028); }
void CPU::RST_30()				{ PC++; m_tclock += 4; _CALL(0x0030); }
void CPU::RST_38()				{ PC++; m_tclock += 4; _CALL(0x0038); }

void CPU::JUMP_IMM16()			{ PC++; m_tclock += 4; u16 a = FETCH_WORD(PC); _JUMP(a); }
void CPU::JUMP_NC_IMM16()		{ PC++; m_tclock += 4; u16 a = FETCH_WORD(PC); if (!GET_FLAG(CarryFlag)) _JUMP(a); }
void CPU::JUMP_NZ_IMM16()		{ PC++; m_tclock += 4; u16 a = FETCH_WORD(PC); if (!GET_FLAG(ZeroFlag)) _JUMP(a); }
void CPU::JUMP_C_IMM16()		{ PC++; m_tclock += 4; u16 a = FETCH_WORD(PC); if (GET_FLAG(CarryFlag)) _JUMP(a); }
void CPU::JUMP_Z_IMM16()		{ PC++; m_tclock += 4; u16 a = FETCH_WORD(PC); if (GET_FLAG(ZeroFlag)) _JUMP(a); }
void CPU::JUMP_HL()				{ _JUMP(GET_COMPOSED_REG(RegisterHL)); }

void CPU::JR_IMM8()				{ PC++; m_tclock += 4; s8 a = static_cast<s8>(FETCH_BYTE(PC)); _JUMP(PC + a); }
void CPU::JR_NC_IMM8()			{ PC++; m_tclock += 4; s8 a = static_cast<s8>(FETCH_BYTE(PC)); if (!GET_FLAG(CarryFlag)) _JUMP(PC + a); }
void CPU::JR_NZ_IMM8()			{ PC++; m_tclock += 4; s8 a = static_cast<s8>(FETCH_BYTE(PC)); if (!GET_FLAG(ZeroFlag)) _JUMP(PC + a); }
void CPU::JR_C_IMM8()			{ PC++; m_tclock += 4; s8 a = static_cast<s8>(FETCH_BYTE(PC)); if (GET_FLAG(CarryFlag)) _JUMP(PC + a); }
void CPU::JR_Z_IMM8()			{ PC++; m_tclock += 4; s8 a = static_cast<s8>(FETCH_BYTE(PC)); if (GET_FLAG(ZeroFlag)) _JUMP(PC + a); }

void CPU::RET()					{ m_tclock += 4; _RET(); }
void CPU::RETI()				{ m_tclock += 4; _RET(); m_ime = true; }
void CPU::RET_NC()				{ m_tclock += 8; if (!GET_FLAG(CarryFlag)) _RET(); }
void CPU::RET_NZ()				{ m_tclock += 8; if (!GET_FLAG(ZeroFlag)) _RET(); }
void CPU::RET_C()				{ m_tclock += 8; if (GET_FLAG(CarryFlag)) _RET(); }
void CPU::RET_Z()				{ m_tclock += 8; if (GET_FLAG(ZeroFlag)) _RET(); }
