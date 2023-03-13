#include <Emulator/CPU/CPU.hpp>


/*
	ACTUAL INSTRUCTIONS
*/
/*
void	CPU::RLC_A() 		{ SET_REG(RegisterA, RLC(GET_REG(RegisterA))); }
void	CPU::RLC_B() 		{ SET_REG(RegisterB, RLC(GET_REG(RegisterB))); }
void	CPU::RLC_C() 		{ SET_REG(RegisterC, RLC(GET_REG(RegisterC))); }
void	CPU::RLC_D() 		{ SET_REG(RegisterD, RLC(GET_REG(RegisterD))); }
void	CPU::RLC_E() 		{ SET_REG(RegisterE, RLC(GET_REG(RegisterE))); }
void	CPU::RLC_H() 		{ SET_REG(RegisterH, RLC(GET_REG(RegisterH))); }
void	CPU::RLC_L() 		{ SET_REG(RegisterL, RLC(GET_REG(RegisterL))); }
void	CPU::RLC_ADDR_HL()	{ u16 HL = GET_COMPOSED_REG(RegisterHL); WRITE_BYTE(HL, RLC(GET_BYTE(HL))); }

void	CPU::RRC_A() 		{ SET_REG(RegisterA, RRC(GET_REG(RegisterA))); }
void	CPU::RRC_B() 		{ SET_REG(RegisterB, RRC(GET_REG(RegisterB))); }
void	CPU::RRC_C() 		{ SET_REG(RegisterC, RRC(GET_REG(RegisterC))); }
void	CPU::RRC_D() 		{ SET_REG(RegisterD, RRC(GET_REG(RegisterD))); }
void	CPU::RRC_E() 		{ SET_REG(RegisterE, RRC(GET_REG(RegisterE))); }
void	CPU::RRC_H() 		{ SET_REG(RegisterH, RRC(GET_REG(RegisterH))); }
void	CPU::RRC_L() 		{ SET_REG(RegisterL, RRC(GET_REG(RegisterL))); }
void	CPU::RRC_ADDR_HL()	{ u16 HL = GET_COMPOSED_REG(RegisterHL); WRITE_BYTE(HL, RRC(GET_BYTE(HL))); }

void	CPU::RL_A() 		{ SET_REG(RegisterA, RL(GET_REG(RegisterA))); }
void	CPU::RL_B() 		{ SET_REG(RegisterB, RL(GET_REG(RegisterB))); }
void	CPU::RL_C() 		{ SET_REG(RegisterC, RL(GET_REG(RegisterC))); }
void	CPU::RL_D() 		{ SET_REG(RegisterD, RL(GET_REG(RegisterD))); }
void	CPU::RL_E() 		{ SET_REG(RegisterE, RL(GET_REG(RegisterE))); }
void	CPU::RL_H() 		{ SET_REG(RegisterH, RL(GET_REG(RegisterH))); }
void	CPU::RL_L() 		{ SET_REG(RegisterL, RL(GET_REG(RegisterL))); }
void	CPU::RL_ADDR_HL()	{ u16 HL = GET_COMPOSED_REG(RegisterHL); WRITE_BYTE(HL, RL(GET_BYTE(HL))); }


void	CPU::RR_A() 		{ SET_REG(RegisterA, RR(GET_REG(RegisterA))); }
void	CPU::RR_B() 		{ SET_REG(RegisterB, RR(GET_REG(RegisterB))); }
void	CPU::RR_C() 		{ SET_REG(RegisterC, RR(GET_REG(RegisterC))); }
void	CPU::RR_D() 		{ SET_REG(RegisterD, RR(GET_REG(RegisterD))); }
void	CPU::RR_E() 		{ SET_REG(RegisterE, RR(GET_REG(RegisterE))); }
void	CPU::RR_H() 		{ SET_REG(RegisterH, RR(GET_REG(RegisterH))); }
void	CPU::RR_L() 		{ SET_REG(RegisterL, RR(GET_REG(RegisterL))); }
void	CPU::RR_ADDR_HL()	{ u16 HL = GET_COMPOSED_REG(RegisterHL); WRITE_BYTE(HL, RR(GET_BYTE(HL))); }

void	CPU::SLA_A() 		{ SET_REG(RegisterA, SLA(GET_REG(RegisterA))); }
void	CPU::SLA_B() 		{ SET_REG(RegisterB, SLA(GET_REG(RegisterB))); }
void	CPU::SLA_C() 		{ SET_REG(RegisterC, SLA(GET_REG(RegisterC))); }
void	CPU::SLA_D() 		{ SET_REG(RegisterD, SLA(GET_REG(RegisterD))); }
void	CPU::SLA_E() 		{ SET_REG(RegisterE, SLA(GET_REG(RegisterE))); }
void	CPU::SLA_H() 		{ SET_REG(RegisterH, SLA(GET_REG(RegisterH))); }
void	CPU::SLA_L() 		{ SET_REG(RegisterL, SLA(GET_REG(RegisterL))); }
void	CPU::SLA_ADDR_HL()	{ u16 HL = GET_COMPOSED_REG(RegisterHL); WRITE_BYTE(HL, SLA(GET_BYTE(HL))); }

void	CPU::SRA_A() 		{ SET_REG(RegisterA, SRA(GET_REG(RegisterA))); }
void	CPU::SRA_B() 		{ SET_REG(RegisterB, SRA(GET_REG(RegisterB))); }
void	CPU::SRA_C() 		{ SET_REG(RegisterC, SRA(GET_REG(RegisterC))); }
void	CPU::SRA_D() 		{ SET_REG(RegisterD, SRA(GET_REG(RegisterD))); }
void	CPU::SRA_E() 		{ SET_REG(RegisterE, SRA(GET_REG(RegisterE))); }
void	CPU::SRA_H() 		{ SET_REG(RegisterH, SRA(GET_REG(RegisterH))); }
void	CPU::SRA_L() 		{ SET_REG(RegisterL, SRA(GET_REG(RegisterL))); }
void	CPU::SRA_ADDR_HL()	{ u16 HL = GET_COMPOSED_REG(RegisterHL); WRITE_BYTE(HL, SRA(GET_BYTE(HL))); }

void	CPU::SRL_A() 		{ SET_REG(RegisterA, SRL(GET_REG(RegisterA))); }
void	CPU::SRL_B() 		{ SET_REG(RegisterB, SRL(GET_REG(RegisterB))); }
void	CPU::SRL_C() 		{ SET_REG(RegisterC, SRL(GET_REG(RegisterC))); }
void	CPU::SRL_D() 		{ SET_REG(RegisterD, SRL(GET_REG(RegisterD))); }
void	CPU::SRL_E() 		{ SET_REG(RegisterE, SRL(GET_REG(RegisterE))); }
void	CPU::SRL_H() 		{ SET_REG(RegisterH, SRL(GET_REG(RegisterH))); }
void	CPU::SRL_L() 		{ SET_REG(RegisterL, SRL(GET_REG(RegisterL))); }
void	CPU::SRL_ADDR_HL()	{ u16 HL = GET_COMPOSED_REG(RegisterHL); WRITE_BYTE(HL, SRL(GET_BYTE(HL))); }

void	CPU::SWAP_A() 		{ SET_REG(RegisterA, SWAP(GET_REG(RegisterA))); }
void	CPU::SWAP_B() 		{ SET_REG(RegisterB, SWAP(GET_REG(RegisterB))); }
void	CPU::SWAP_C() 		{ SET_REG(RegisterC, SWAP(GET_REG(RegisterC))); }
void	CPU::SWAP_D() 		{ SET_REG(RegisterD, SWAP(GET_REG(RegisterD))); }
void	CPU::SWAP_E() 		{ SET_REG(RegisterE, SWAP(GET_REG(RegisterE))); }
void	CPU::SWAP_H() 		{ SET_REG(RegisterH, SWAP(GET_REG(RegisterH))); }
void	CPU::SWAP_L() 		{ SET_REG(RegisterL, SWAP(GET_REG(RegisterL))); }
void	CPU::SWAP_ADDR_HL()	{ u16 HL = GET_COMPOSED_REG(RegisterHL); WRITE_BYTE(HL, SWAP(GET_BYTE(HL))); }

void	CPU::BIT_N_A()		{ BIT_N_8(GET_TARGET_BIT(0x40, m_opcode), GET_REG(RegisterA)); }
void	CPU::BIT_N_B()		{ BIT_N_8(GET_TARGET_BIT(0x40, m_opcode), GET_REG(RegisterB)); }
void	CPU::BIT_N_C()		{ BIT_N_8(GET_TARGET_BIT(0x40, m_opcode), GET_REG(RegisterC)); }
void	CPU::BIT_N_D()		{ BIT_N_8(GET_TARGET_BIT(0x40, m_opcode), GET_REG(RegisterD)); }
void	CPU::BIT_N_E()		{ BIT_N_8(GET_TARGET_BIT(0x40, m_opcode), GET_REG(RegisterE)); }
void	CPU::BIT_N_H()		{ BIT_N_8(GET_TARGET_BIT(0x40, m_opcode), GET_REG(RegisterH)); }
void	CPU::BIT_N_L()		{ BIT_N_8(GET_TARGET_BIT(0x40, m_opcode), GET_REG(RegisterL)); }
void	CPU::BIT_N_ADDR_HL(){ BIT_N_8(GET_TARGET_BIT(0x40, m_opcode), GET_BYTE(GET_COMPOSED_REG(RegisterHL))); }

void	CPU::RES_N_A()		{ SET_REG(RegisterA, RES_N_8(GET_TARGET_BIT(0x80, m_opcode), GET_REG(RegisterA))); }
void	CPU::RES_N_B()		{ SET_REG(RegisterB, RES_N_8(GET_TARGET_BIT(0x80, m_opcode), GET_REG(RegisterB))); }
void	CPU::RES_N_C()		{ SET_REG(RegisterC, RES_N_8(GET_TARGET_BIT(0x80, m_opcode), GET_REG(RegisterC))); }
void	CPU::RES_N_D()		{ SET_REG(RegisterD, RES_N_8(GET_TARGET_BIT(0x80, m_opcode), GET_REG(RegisterD))); }
void	CPU::RES_N_E()		{ SET_REG(RegisterE, RES_N_8(GET_TARGET_BIT(0x80, m_opcode), GET_REG(RegisterE))); }
void	CPU::RES_N_H()		{ SET_REG(RegisterH, RES_N_8(GET_TARGET_BIT(0x80, m_opcode), GET_REG(RegisterH))); }
void	CPU::RES_N_L()		{ SET_REG(RegisterL, RES_N_8(GET_TARGET_BIT(0x80, m_opcode), GET_REG(RegisterL))); }
void	CPU::RES_N_ADDR_HL(){ u16 HL = GET_COMPOSED_REG(RegisterHL); WRITE_BYTE(HL, RES_N_8(GET_TARGET_BIT(0x80, m_opcode), GET_BYTE(HL))); }

void	CPU::SET_N_A()		{ SET_REG(RegisterA, SET_N_8(GET_TARGET_BIT(0xC0, m_opcode), GET_REG(RegisterA))); }
void	CPU::SET_N_B()		{ SET_REG(RegisterB, SET_N_8(GET_TARGET_BIT(0xC0, m_opcode), GET_REG(RegisterB))); }
void	CPU::SET_N_C()		{ SET_REG(RegisterC, SET_N_8(GET_TARGET_BIT(0xC0, m_opcode), GET_REG(RegisterC))); }
void	CPU::SET_N_D()		{ SET_REG(RegisterD, SET_N_8(GET_TARGET_BIT(0xC0, m_opcode), GET_REG(RegisterD))); }
void	CPU::SET_N_E()		{ SET_REG(RegisterE, SET_N_8(GET_TARGET_BIT(0xC0, m_opcode), GET_REG(RegisterE))); }
void	CPU::SET_N_H()		{ SET_REG(RegisterH, SET_N_8(GET_TARGET_BIT(0xC0, m_opcode), GET_REG(RegisterH))); }
void	CPU::SET_N_L()		{ SET_REG(RegisterL, SET_N_8(GET_TARGET_BIT(0xC0, m_opcode), GET_REG(RegisterL))); }
void	CPU::SET_N_ADDR_HL(){ u16 HL = GET_COMPOSED_REG(RegisterHL); WRITE_BYTE(HL, SET_N_8(GET_TARGET_BIT(0xC0, m_opcode), GET_BYTE(HL))); }
*/