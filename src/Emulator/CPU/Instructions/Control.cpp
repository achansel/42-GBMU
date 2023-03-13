#include <Emulator/CPU/CPU.hpp>

/*
	ALL THE CONTROL INSTRUCTIONS
*/
void CPU::CALL_IMM16()			{ m_tclock += 4; u16 a = FETCH_WORD(); _CALL(a); }
void CPU::CALL_NC_IMM16()		{ m_tclock += 4; u16 a = FETCH_WORD(); if (!GET_FLAG(CarryFlag)) _CALL(a); }
void CPU::CALL_NZ_IMM16()		{ m_tclock += 4; u16 a = FETCH_WORD(); if (!GET_FLAG(ZeroFlag)) _CALL(a); }
void CPU::CALL_C_IMM16()		{ m_tclock += 4; u16 a = FETCH_WORD(); if (GET_FLAG(CarryFlag)) _CALL(a); }
void CPU::CALL_Z_IMM16()		{ m_tclock += 4; u16 a = FETCH_WORD(); if (GET_FLAG(ZeroFlag)) _CALL(a); }

void CPU::RST_00()				{ m_tclock += 4; _CALL(0x0000); }
void CPU::RST_08()				{ m_tclock += 4; _CALL(0x0008); }
void CPU::RST_10()				{ m_tclock += 4; _CALL(0x0010); }
void CPU::RST_18()				{ m_tclock += 4; _CALL(0x0018); }
void CPU::RST_20()				{ m_tclock += 4; _CALL(0x0020); }
void CPU::RST_28()				{ m_tclock += 4; _CALL(0x0028); }
void CPU::RST_30()				{ m_tclock += 4; _CALL(0x0030); }
void CPU::RST_38()				{ m_tclock += 4; _CALL(0x0038); }

void CPU::JUMP_IMM16()			{ m_tclock += 4; u16 a = FETCH_WORD(); _JUMP(a); }
void CPU::JUMP_NC_IMM16()		{ m_tclock += 4; u16 a = FETCH_WORD(); if (!GET_FLAG(CarryFlag)) _JUMP(a); }
void CPU::JUMP_NZ_IMM16()		{ m_tclock += 4; u16 a = FETCH_WORD(); if (!GET_FLAG(ZeroFlag)) _JUMP(a); }
void CPU::JUMP_C_IMM16()		{ m_tclock += 4; u16 a = FETCH_WORD(); if (GET_FLAG(CarryFlag)) _JUMP(a); }
void CPU::JUMP_Z_IMM16()		{ m_tclock += 4; u16 a = FETCH_WORD(); if (GET_FLAG(ZeroFlag)) _JUMP(a); }
void CPU::JUMP_HL()				{ _JUMP(GET_COMPOSED_REG(RegisterHL)); }

void CPU::JR_IMM8()				{ m_tclock += 4; s8 a = static_cast<s8>(FETCH_BYTE()); _JUMP(PC + a); }
void CPU::JR_NC_IMM8()			{ m_tclock += 4; s8 a = static_cast<s8>(FETCH_BYTE()); if (!GET_FLAG(CarryFlag)) _JUMP(PC + a); }
void CPU::JR_NZ_IMM8()			{ m_tclock += 4; s8 a = static_cast<s8>(FETCH_BYTE()); if (!GET_FLAG(ZeroFlag)) _JUMP(PC + a); }
void CPU::JR_C_IMM8()			{ m_tclock += 4; s8 a = static_cast<s8>(FETCH_BYTE()); if (GET_FLAG(CarryFlag)) _JUMP(PC + a); }
void CPU::JR_Z_IMM8()			{ m_tclock += 4; s8 a = static_cast<s8>(FETCH_BYTE()); if (GET_FLAG(ZeroFlag)) _JUMP(PC + a); }

void CPU::RET()					{ m_tclock += 4; _RET(); }
void CPU::RETI()				{ m_tclock += 4; _RET(); m_ime = true; }
void CPU::RET_NC()				{ m_tclock += 8; if (!GET_FLAG(CarryFlag)) _RET(); }
void CPU::RET_NZ()				{ m_tclock += 8; if (!GET_FLAG(ZeroFlag)) _RET(); }
void CPU::RET_C()				{ m_tclock += 8; if (GET_FLAG(CarryFlag)) _RET(); }
void CPU::RET_Z()				{ m_tclock += 8; if (GET_FLAG(ZeroFlag)) _RET(); }
