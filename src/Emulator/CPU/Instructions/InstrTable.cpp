#include <Emulator/CPU/CPU.hpp>
#include <algorithm>

void CPU::fill_instruction_tables()
{
	#define i	m_instructions
	#define ie	m_instructions_cb

	i.fill(&CPU::UNDEFINED);
	ie.fill(&CPU::UNDEFINED_CB);

	i[0x00] = &CPU::NOP;
	i[0x01] = &CPU::LD_BC_d16;
	i[0x04] = &CPU::INC_B;
	i[0x05] = &CPU::DEC_B;
	i[0x06] = &CPU::LD_B_d8;
	i[0x0B] = &CPU::DEC_BC;
	i[0x0C] = &CPU::INC_C;
	i[0x0D] = &CPU::DEC_C;
	i[0x0E] = &CPU::LD_C_d8;
	i[0x11] = &CPU::LD_DE_d16;
	i[0x12] = &CPU::LD_ADDR_DE_A;
	i[0x13] = &CPU::INC_DE;
	i[0x15] = &CPU::DEC_D;
	i[0x16] = &CPU::LD_D_d8;
	i[0x17] = &CPU::RL_A;
	i[0x18] = &CPU::JR_IMM8;
	i[0x19] = &CPU::ADD_HL_DE;
	i[0x1A] = &CPU::LD_A_ADDR_DE;
	i[0x1C] = &CPU::INC_E;
	i[0x1D] = &CPU::DEC_E;
	i[0x1E] = &CPU::LD_E_d8;
	i[0x20] = &CPU::JR_NZ_IMM8;
	i[0x21] = &CPU::LD_HL_d16;
	i[0x22] = &CPU::LD_ADDR_HLI_A;
	i[0x23] = &CPU::INC_HL;
	i[0x24] = &CPU::INC_H;
	i[0x28] = &CPU::JR_Z_IMM8;
	i[0x2A] = &CPU::LD_A_ADDR_HLI;
	i[0x2C] = &CPU::INC_L;
	i[0x2E] = &CPU::LD_L_d8;
	i[0x2F] = &CPU::CPL;
	i[0x31] = &CPU::LD_SP_d16;
	i[0x32] = &CPU::LD_ADDR_HLD_A;
	i[0x35] = &CPU::DEC_ADDR_HL;
	i[0x36] = &CPU::LD_ADDR_HL_d8;
	i[0x3D] = &CPU::DEC_A;
	i[0x3E] = &CPU::LD_A_d8;
	i[0x47] = &CPU::LD_B_A;
	i[0x4F] = &CPU::LD_C_A;
	i[0x56] = &CPU::LD_D_ADDR_HL;
	i[0x57] = &CPU::LD_D_A;
	i[0x5E] = &CPU::LD_E_ADDR_HL;
	i[0x5F] = &CPU::LD_E_A;
	i[0x67] = &CPU::LD_H_A;
	i[0x77] = &CPU::LD_ADDR_HL_A;
	i[0x78] = &CPU::LD_A_B;
	i[0x79] = &CPU::LD_A_C;
	i[0x7B] = &CPU::LD_A_E;
	i[0x7C] = &CPU::LD_A_H;
	i[0x7D] = &CPU::LD_A_L;
	i[0x7E] = &CPU::LD_A_ADDR_HL;
	i[0x86] = &CPU::ADD_A_ADDR_HL;
	i[0x87] = &CPU::ADD_A_A;
	i[0x90] = &CPU::SUB_A_B;
	i[0xA1] = &CPU::AND_A_C;
	i[0xA7] = &CPU::AND_A_A;
	i[0xA9] = &CPU::XOR_A_C;
	i[0xAF] = &CPU::XOR_A_A;
	i[0xB0] = &CPU::OR_A_B;
	i[0xB1] = &CPU::OR_A_C;
	i[0xBE] = &CPU::CP_A_ADDR_HL;
	i[0xC1] = &CPU::POP_BC;
	i[0xC3] = &CPU::JUMP_IMM16;
	i[0xC5] = &CPU::PUSH_BC;
	i[0xC8] = &CPU::RET_Z;
	i[0xC9] = &CPU::RET;
	i[0xCA] = &CPU::JUMP_Z_IMM16;
	i[0xCD] = &CPU::CALL_IMM16;
	i[0xD1] = &CPU::POP_DE;
	i[0xD5] = &CPU::PUSH_DE;
	i[0xE0] = &CPU::LDH_IMM8_A;
	i[0xE1] = &CPU::POP_HL;
	i[0xE2] = &CPU::LD_ADDR_C_A;
	i[0xE5] = &CPU::PUSH_HL;
	i[0xE6] = &CPU::AND_A_IMM8;
	i[0xE9] = &CPU::JUMP_HL;
	i[0xEA] = &CPU::LD_ADDR_IMM16_A;
	i[0xEF] = &CPU::RST_28;
	i[0xF0] = &CPU::LDH_A_IMM8;
	i[0xF1] = &CPU::POP_AF;
	i[0xF3] = &CPU::DI;
	i[0xF5] = &CPU::PUSH_AF;
	i[0xFA] = &CPU::LD_A_ADDR_IMM16;
	i[0xFB] = &CPU::EI;
	i[0xFE] = &CPU::CP_A_IMM8;

	ie[0x00] = &CPU::NOP;
	ie[0x11] = &CPU::RL_C;
	ie[0x37] = &CPU::SWAP_A;
	ie[0x7C] = &CPU::BIT_N_H;
	ie[0x87] = &CPU::RES_N_A;
}