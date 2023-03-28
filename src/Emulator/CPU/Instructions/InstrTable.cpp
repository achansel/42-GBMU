#include <Emulator/CPU/CPU.hpp>

#include <algorithm>

void CPU::fill_instructions_table()
{
	#define i	m_instructions

	i.fill(&CPU::UNDEFINED);
	
	i[0x00] = &CPU::NOP;
	i[0x10] = &CPU::STOP;
	i[0xF3] = &CPU::DI;
	i[0xFB] = &CPU::EI;
	i[0x76] = &CPU::HALT;

	/* No use of PREFIXED_OP_REG here because the instructions, when executed from the non-prefixed opcodes, set the Z flag to 0 */
	i[0x07] = &CPU::RLCA;
	i[0x17] = &CPU::RLA;
	i[0x0F] = &CPU::RRCA;
	i[0x1F] = &CPU::RRA;

	i[0xC7] = &CPU::RST<0x00>;
	i[0xCF] = &CPU::RST<0x08>;
	i[0xD7] = &CPU::RST<0x10>;
	i[0xDF] = &CPU::RST<0x18>;
	i[0xE7] = &CPU::RST<0x20>;
	i[0xEF] = &CPU::RST<0x28>;
	i[0xF7] = &CPU::RST<0x30>;
	i[0xFF] = &CPU::RST<0x38>;

	i[0xC2] = &CPU::JUMP_IMM16<Cond::NZ>;
	i[0xC3] = &CPU::JUMP_IMM16;
	i[0xD2] = &CPU::JUMP_IMM16<Cond::NC>;
	i[0xCA] = &CPU::JUMP_IMM16<Cond::Z>;
	i[0xDA] = &CPU::JUMP_IMM16<Cond::C>;
	i[0xE9] = &CPU::JUMP_HL;

	i[0x20] = &CPU::JR_IMM8<Cond::NZ>;
	i[0x30] = &CPU::JR_IMM8<Cond::NC>;
	i[0x28] = &CPU::JR_IMM8<Cond::Z>;
	i[0x38] = &CPU::JR_IMM8<Cond::C>;
	i[0x18] = &CPU::JR_IMM8;

	i[0xC4] = &CPU::CALL_IMM16<Cond::NZ>;
	i[0xD4] = &CPU::CALL_IMM16<Cond::NC>;
	i[0xCC] = &CPU::CALL_IMM16<Cond::Z>;
	i[0xDC] = &CPU::CALL_IMM16<Cond::C>;
	i[0xCD] = &CPU::CALL_IMM16;

	i[0xC0] = &CPU::RET<Cond::NZ>;
	i[0xD0] = &CPU::RET<Cond::NC>;
	i[0xC8] = &CPU::RET<Cond::Z>;
	i[0xD8] = &CPU::RET<Cond::C>;
	i[0xC9] = &CPU::RET;
	i[0xD9] = &CPU::RETI;



	i[0x01] = &CPU::MOV_REG_IMM<RegisterBC>;
	i[0x11] = &CPU::MOV_REG_IMM<RegisterDE>;
	i[0x21] = &CPU::MOV_REG_IMM<RegisterHL>;
	i[0x31] = &CPU::MOV_REG_IMM<RegisterSP>;

	i[0x02] = &CPU::MOV_ADDR_REG_REG<RegisterBC, RegisterA>;
	i[0x12] = &CPU::MOV_ADDR_REG_REG<RegisterDE, RegisterA>;
	i[0x22] = &CPU::LD_ADDR_HLI_A;
	i[0x32] = &CPU::LD_ADDR_HLD_A;

	i[0x06] = &CPU::MOV_REG_IMM<RegisterB>;
	i[0x16] = &CPU::MOV_REG_IMM<RegisterD>;
	i[0x26] = &CPU::MOV_REG_IMM<RegisterH>;
	i[0x36] = &CPU::LD_ADDR_HL_IMM8;

	i[0x08] = &CPU::LD_ADDR_IMM16_SP;

	i[0x0A] = &CPU::MOV_REG_ADDR_REG<RegisterA, RegisterBC>;
	i[0x1A] = &CPU::MOV_REG_ADDR_REG<RegisterA, RegisterDE>;
	i[0x2A] = &CPU::LD_A_ADDR_HLI;
	i[0x3A] = &CPU::LD_A_ADDR_HLD;

	i[0x0E] = &CPU::MOV_REG_IMM<RegisterC>;
	i[0x1E] = &CPU::MOV_REG_IMM<RegisterE>;
	i[0x2E] = &CPU::MOV_REG_IMM<RegisterL>;
	i[0x3E] = &CPU::MOV_REG_IMM<RegisterA>;

	i[0x40] = &CPU::MOV_REG_REG<RegisterB, RegisterB>;
	i[0x41] = &CPU::MOV_REG_REG<RegisterB, RegisterC>;
	i[0x42] = &CPU::MOV_REG_REG<RegisterB, RegisterD>;
	i[0x43] = &CPU::MOV_REG_REG<RegisterB, RegisterE>;
	i[0x44] = &CPU::MOV_REG_REG<RegisterB, RegisterH>;
	i[0x45] = &CPU::MOV_REG_REG<RegisterB, RegisterL>;
	i[0x46] = &CPU::MOV_REG_ADDR_REG<RegisterB, RegisterHL>;
	i[0x47] = &CPU::MOV_REG_REG<RegisterB, RegisterA>;

	i[0x48] = &CPU::MOV_REG_REG<RegisterC, RegisterB>;
	i[0x49] = &CPU::MOV_REG_REG<RegisterC, RegisterC>;
	i[0x4A] = &CPU::MOV_REG_REG<RegisterC, RegisterD>;
	i[0x4B] = &CPU::MOV_REG_REG<RegisterC, RegisterE>;
	i[0x4C] = &CPU::MOV_REG_REG<RegisterC, RegisterH>;
	i[0x4D] = &CPU::MOV_REG_REG<RegisterC, RegisterL>;
	i[0x4E] = &CPU::MOV_REG_ADDR_REG<RegisterC, RegisterHL>;
	i[0x4F] = &CPU::MOV_REG_REG<RegisterC, RegisterA>;

	i[0x50] = &CPU::MOV_REG_REG<RegisterD, RegisterB>;
	i[0x51] = &CPU::MOV_REG_REG<RegisterD, RegisterC>;
	i[0x52] = &CPU::MOV_REG_REG<RegisterD, RegisterD>;
	i[0x53] = &CPU::MOV_REG_REG<RegisterD, RegisterE>;
	i[0x54] = &CPU::MOV_REG_REG<RegisterD, RegisterH>;
	i[0x55] = &CPU::MOV_REG_REG<RegisterD, RegisterL>;
	i[0x56] = &CPU::MOV_REG_ADDR_REG<RegisterD, RegisterHL>;
	i[0x57] = &CPU::MOV_REG_REG<RegisterD, RegisterA>;

	i[0x58] = &CPU::MOV_REG_REG<RegisterE, RegisterB>;
	i[0x59] = &CPU::MOV_REG_REG<RegisterE, RegisterC>;
	i[0x5A] = &CPU::MOV_REG_REG<RegisterE, RegisterD>;
	i[0x5B] = &CPU::MOV_REG_REG<RegisterE, RegisterE>;
	i[0x5C] = &CPU::MOV_REG_REG<RegisterE, RegisterH>;
	i[0x5D] = &CPU::MOV_REG_REG<RegisterE, RegisterL>;
	i[0x5E] = &CPU::MOV_REG_ADDR_REG<RegisterE, RegisterHL>;
	i[0x5F] = &CPU::MOV_REG_REG<RegisterE, RegisterA>;

	i[0x60] = &CPU::MOV_REG_REG<RegisterH, RegisterB>;
	i[0x61] = &CPU::MOV_REG_REG<RegisterH, RegisterC>;
	i[0x62] = &CPU::MOV_REG_REG<RegisterH, RegisterD>;
	i[0x63] = &CPU::MOV_REG_REG<RegisterH, RegisterE>;
	i[0x64] = &CPU::MOV_REG_REG<RegisterH, RegisterH>;
	i[0x65] = &CPU::MOV_REG_REG<RegisterH, RegisterL>;
	i[0x66] = &CPU::MOV_REG_ADDR_REG<RegisterH, RegisterHL>;
	i[0x67] = &CPU::MOV_REG_REG<RegisterH, RegisterA>;

	i[0x68] = &CPU::MOV_REG_REG<RegisterL, RegisterB>;
	i[0x69] = &CPU::MOV_REG_REG<RegisterL, RegisterC>;
	i[0x6A] = &CPU::MOV_REG_REG<RegisterL, RegisterD>;
	i[0x6B] = &CPU::MOV_REG_REG<RegisterL, RegisterE>;
	i[0x6C] = &CPU::MOV_REG_REG<RegisterL, RegisterH>;
	i[0x6D] = &CPU::MOV_REG_REG<RegisterL, RegisterL>;
	i[0x6E] = &CPU::MOV_REG_ADDR_REG<RegisterL, RegisterHL>;
	i[0x6F] = &CPU::MOV_REG_REG<RegisterL, RegisterA>;

	i[0x70] = &CPU::MOV_ADDR_REG_REG<RegisterHL, RegisterB>;
	i[0x71] = &CPU::MOV_ADDR_REG_REG<RegisterHL, RegisterC>;
	i[0x72] = &CPU::MOV_ADDR_REG_REG<RegisterHL, RegisterD>;
	i[0x73] = &CPU::MOV_ADDR_REG_REG<RegisterHL, RegisterE>;
	i[0x74] = &CPU::MOV_ADDR_REG_REG<RegisterHL, RegisterH>;
	i[0x75] = &CPU::MOV_ADDR_REG_REG<RegisterHL, RegisterL>;
	i[0x77] = &CPU::MOV_ADDR_REG_REG<RegisterHL, RegisterA>;

	i[0x78] = &CPU::MOV_REG_REG<RegisterA, RegisterB>;
	i[0x79] = &CPU::MOV_REG_REG<RegisterA, RegisterC>;
	i[0x7A] = &CPU::MOV_REG_REG<RegisterA, RegisterD>;
	i[0x7B] = &CPU::MOV_REG_REG<RegisterA, RegisterE>;
	i[0x7C] = &CPU::MOV_REG_REG<RegisterA, RegisterH>;
	i[0x7D] = &CPU::MOV_REG_REG<RegisterA, RegisterL>;
	i[0x7E] = &CPU::MOV_REG_ADDR_REG<RegisterA, RegisterHL>;
	i[0x7F] = &CPU::MOV_REG_REG<RegisterA, RegisterA>;

	i[0xE0] = &CPU::LDH_IMM8_A;
	i[0xF0] = &CPU::LDH_A_IMM8;

	i[0xE2] = &CPU::MOV_ADDR_REG_REG<RegisterC, RegisterA>;
	i[0xF2] = &CPU::MOV_REG_ADDR_REG<RegisterA, RegisterC>;

	i[0xF8] = &CPU::LD_HL_SP_X_IMM8;
	i[0xF9] = &CPU::LD_SP_HL;

	i[0xEA] = &CPU::LD_ADDR_IMM16_A;
	i[0xFA] = &CPU::LD_A_ADDR_IMM16;

	i[0xC1] = &CPU::POP<RegisterBC>;
	i[0xD1] = &CPU::POP<RegisterDE>;
	i[0xE1] = &CPU::POP<RegisterHL>;
	i[0xF1] = &CPU::POP<RegisterAF>;

	i[0xC5] = &CPU::PUSH<RegisterBC>;
	i[0xD5] = &CPU::PUSH<RegisterDE>;
	i[0xE5] = &CPU::PUSH<RegisterHL>;
	i[0xF5] = &CPU::PUSH<RegisterAF>;

	i[0x27] = &CPU::DAA;
	i[0x2F] = &CPU::CPL;
	i[0x37] = &CPU::SCF;
	i[0x3F] = &CPU::CCF;

	i[0x03] = &CPU::INC_REG<RegisterBC>;
	i[0x13] = &CPU::INC_REG<RegisterDE>;
	i[0x23] = &CPU::INC_REG<RegisterHL>;
	i[0x33] = &CPU::INC_REG<RegisterSP>;

	i[0x04] = &CPU::INC_REG<RegisterB>;
	i[0x14] = &CPU::INC_REG<RegisterD>;
	i[0x24] = &CPU::INC_REG<RegisterH>;
	i[0x34] = &CPU::INC_ADDR_HL;

	i[0x05] = &CPU::DEC_REG<RegisterB>;
	i[0x15] = &CPU::DEC_REG<RegisterD>;
	i[0x25] = &CPU::DEC_REG<RegisterH>;
	i[0x35] = &CPU::DEC_ADDR_HL;

	i[0x09] = &CPU::ADD_REG16_REG16<RegisterHL, RegisterBC>;
	i[0x19] = &CPU::ADD_REG16_REG16<RegisterHL, RegisterDE>;
	i[0x29] = &CPU::ADD_REG16_REG16<RegisterHL, RegisterHL>;
	i[0x39] = &CPU::ADD_REG16_REG16<RegisterHL, RegisterSP>;

	i[0x0B] = &CPU::DEC_REG<RegisterBC>;
	i[0x1B] = &CPU::DEC_REG<RegisterDE>;
	i[0x2B] = &CPU::DEC_REG<RegisterHL>;
	i[0x3B] = &CPU::DEC_REG<RegisterSP>;

	i[0x0C] = &CPU::INC_REG<RegisterC>;
	i[0x1C] = &CPU::INC_REG<RegisterE>;
	i[0x2C] = &CPU::INC_REG<RegisterL>;
	i[0x3C] = &CPU::INC_REG<RegisterA>;

	i[0x0D] = &CPU::DEC_REG<RegisterC>;
	i[0x1D] = &CPU::DEC_REG<RegisterE>;
	i[0x2D] = &CPU::DEC_REG<RegisterL>;
	i[0x3D] = &CPU::DEC_REG<RegisterA>;

	i[0x80] = &CPU::ALU_OP_REG<ALUOP::ADD, RegisterB>;
	i[0x81] = &CPU::ALU_OP_REG<ALUOP::ADD, RegisterC>;
	i[0x82] = &CPU::ALU_OP_REG<ALUOP::ADD, RegisterD>;
	i[0x83] = &CPU::ALU_OP_REG<ALUOP::ADD, RegisterE>;
	i[0x84] = &CPU::ALU_OP_REG<ALUOP::ADD, RegisterH>;
	i[0x85] = &CPU::ALU_OP_REG<ALUOP::ADD, RegisterL>;
	i[0x86] = &CPU::ALU_OP_ADDR_HL<ALUOP::ADD>;
	i[0x87] = &CPU::ALU_OP_REG<ALUOP::ADD, RegisterA>;
	i[0xC6] = &CPU::ALU_OP_IMM8<ALUOP::ADD>;

	i[0x88] = &CPU::ALU_OP_REG<ALUOP::ADC, RegisterB>;
	i[0x89] = &CPU::ALU_OP_REG<ALUOP::ADC, RegisterC>;
	i[0x8A] = &CPU::ALU_OP_REG<ALUOP::ADC, RegisterD>;
	i[0x8B] = &CPU::ALU_OP_REG<ALUOP::ADC, RegisterE>;
	i[0x8C] = &CPU::ALU_OP_REG<ALUOP::ADC, RegisterH>;
	i[0x8D] = &CPU::ALU_OP_REG<ALUOP::ADC, RegisterL>;
	i[0x8E] = &CPU::ALU_OP_ADDR_HL<ALUOP::ADC>;
	i[0x8F] = &CPU::ALU_OP_REG<ALUOP::ADC, RegisterA>;
	i[0xCE] = &CPU::ALU_OP_IMM8<ALUOP::ADC>;

	i[0x90] = &CPU::ALU_OP_REG<ALUOP::SUB, RegisterB>;
	i[0x91] = &CPU::ALU_OP_REG<ALUOP::SUB, RegisterC>;
	i[0x92] = &CPU::ALU_OP_REG<ALUOP::SUB, RegisterD>;
	i[0x93] = &CPU::ALU_OP_REG<ALUOP::SUB, RegisterE>;
	i[0x94] = &CPU::ALU_OP_REG<ALUOP::SUB, RegisterH>;
	i[0x95] = &CPU::ALU_OP_REG<ALUOP::SUB, RegisterL>;
	i[0x96] = &CPU::ALU_OP_ADDR_HL<ALUOP::SUB>;
	i[0x97] = &CPU::ALU_OP_REG<ALUOP::SUB, RegisterA>;
	i[0xD6] = &CPU::ALU_OP_IMM8<ALUOP::SUB>;

	i[0x98] = &CPU::ALU_OP_REG<ALUOP::SBC, RegisterB>;
	i[0x99] = &CPU::ALU_OP_REG<ALUOP::SBC, RegisterC>;
	i[0x9A] = &CPU::ALU_OP_REG<ALUOP::SBC, RegisterD>;
	i[0x9B] = &CPU::ALU_OP_REG<ALUOP::SBC, RegisterE>;
	i[0x9C] = &CPU::ALU_OP_REG<ALUOP::SBC, RegisterH>;
	i[0x9D] = &CPU::ALU_OP_REG<ALUOP::SBC, RegisterL>;
	i[0x9E] = &CPU::ALU_OP_ADDR_HL<ALUOP::SBC>;
	i[0x9F] = &CPU::ALU_OP_REG<ALUOP::SBC, RegisterA>;
	i[0xDE] = &CPU::ALU_OP_IMM8<ALUOP::SBC>;

	i[0xA0] = &CPU::ALU_OP_REG<ALUOP::AND, RegisterB>;
	i[0xA1] = &CPU::ALU_OP_REG<ALUOP::AND, RegisterC>;
	i[0xA2] = &CPU::ALU_OP_REG<ALUOP::AND, RegisterD>;
	i[0xA3] = &CPU::ALU_OP_REG<ALUOP::AND, RegisterE>;
	i[0xA4] = &CPU::ALU_OP_REG<ALUOP::AND, RegisterH>;
	i[0xA5] = &CPU::ALU_OP_REG<ALUOP::AND, RegisterL>;
	i[0xA6] = &CPU::ALU_OP_ADDR_HL<ALUOP::AND>;
	i[0xA7] = &CPU::ALU_OP_REG<ALUOP::AND, RegisterA>;
	i[0xE6] = &CPU::ALU_OP_IMM8<ALUOP::AND>;

	i[0xA8] = &CPU::ALU_OP_REG<ALUOP::XOR, RegisterB>;
	i[0xA9] = &CPU::ALU_OP_REG<ALUOP::XOR, RegisterC>;
	i[0xAA] = &CPU::ALU_OP_REG<ALUOP::XOR, RegisterD>;
	i[0xAB] = &CPU::ALU_OP_REG<ALUOP::XOR, RegisterE>;
	i[0xAC] = &CPU::ALU_OP_REG<ALUOP::XOR, RegisterH>;
	i[0xAD] = &CPU::ALU_OP_REG<ALUOP::XOR, RegisterL>;
	i[0xAE] = &CPU::ALU_OP_ADDR_HL<ALUOP::XOR>;
	i[0xAF] = &CPU::ALU_OP_REG<ALUOP::XOR, RegisterA>;
	i[0xEE] = &CPU::ALU_OP_IMM8<ALUOP::XOR>;

	i[0xB0] = &CPU::ALU_OP_REG<ALUOP::OR, RegisterB>;
	i[0xB1] = &CPU::ALU_OP_REG<ALUOP::OR, RegisterC>;
	i[0xB2] = &CPU::ALU_OP_REG<ALUOP::OR, RegisterD>;
	i[0xB3] = &CPU::ALU_OP_REG<ALUOP::OR, RegisterE>;
	i[0xB4] = &CPU::ALU_OP_REG<ALUOP::OR, RegisterH>;
	i[0xB5] = &CPU::ALU_OP_REG<ALUOP::OR, RegisterL>;
	i[0xB6] = &CPU::ALU_OP_ADDR_HL<ALUOP::OR>;
	i[0xB7] = &CPU::ALU_OP_REG<ALUOP::OR, RegisterA>;
	i[0xF6] = &CPU::ALU_OP_IMM8<ALUOP::OR>;

	i[0xB8] = &CPU::ALU_OP_REG<ALUOP::CP, RegisterB>;
	i[0xB9] = &CPU::ALU_OP_REG<ALUOP::CP, RegisterC>;
	i[0xBA] = &CPU::ALU_OP_REG<ALUOP::CP, RegisterD>;
	i[0xBB] = &CPU::ALU_OP_REG<ALUOP::CP, RegisterE>;
	i[0xBC] = &CPU::ALU_OP_REG<ALUOP::CP, RegisterH>;
	i[0xBD] = &CPU::ALU_OP_REG<ALUOP::CP, RegisterL>;
	i[0xBE] = &CPU::ALU_OP_ADDR_HL<ALUOP::CP>;
	i[0xBF] = &CPU::ALU_OP_REG<ALUOP::CP, RegisterA>;
	i[0xFE] = &CPU::ALU_OP_IMM8<ALUOP::CP>;

	i[0xE8] = &CPU::ADD_SP_IMM8;

	#undef i
}

void CPU::fill_instructions_table_cb()
{
	#define ie	m_instructions_cb

	ie.fill(&CPU::UNDEFINED_CB);

	ie[0x00] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RLC, RegisterB>;
	ie[0x01] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RLC, RegisterC>;
	ie[0x02] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RLC, RegisterD>;
	ie[0x03] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RLC, RegisterE>;
	ie[0x04] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RLC, RegisterH>;
	ie[0x05] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RLC, RegisterL>;
	ie[0x06] = &CPU::PREFIXED_OP_ADDR_HL<PREFIXEDOP::RLC>;
	ie[0x07] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RLC, RegisterA>;

	ie[0x08] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RRC, RegisterB>;
	ie[0x09] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RRC, RegisterC>;
	ie[0x0A] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RRC, RegisterD>;
	ie[0x0B] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RRC, RegisterE>;
	ie[0x0C] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RRC, RegisterH>;
	ie[0x0D] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RRC, RegisterL>;
	ie[0x0E] = &CPU::PREFIXED_OP_ADDR_HL<PREFIXEDOP::RRC>;
	ie[0x0F] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RRC, RegisterA>;

	ie[0x10] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RL, RegisterB>;
	ie[0x11] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RL, RegisterC>;
	ie[0x12] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RL, RegisterD>;
	ie[0x13] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RL, RegisterE>;
	ie[0x14] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RL, RegisterH>;
	ie[0x15] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RL, RegisterL>;
	ie[0x16] = &CPU::PREFIXED_OP_ADDR_HL<PREFIXEDOP::RL>;
	ie[0x17] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RL, RegisterA>;

	ie[0x18] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RR, RegisterB>;
	ie[0x19] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RR, RegisterC>;
	ie[0x1A] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RR, RegisterD>;
	ie[0x1B] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RR, RegisterE>;
	ie[0x1C] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RR, RegisterH>;
	ie[0x1D] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RR, RegisterL>;
	ie[0x1E] = &CPU::PREFIXED_OP_ADDR_HL<PREFIXEDOP::RR>;
	ie[0x1F] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::RR, RegisterA>;

	ie[0x20] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SLA, RegisterB>;
	ie[0x21] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SLA, RegisterC>;
	ie[0x22] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SLA, RegisterD>;
	ie[0x23] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SLA, RegisterE>;
	ie[0x24] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SLA, RegisterH>;
	ie[0x25] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SLA, RegisterL>;
	ie[0x26] = &CPU::PREFIXED_OP_ADDR_HL<PREFIXEDOP::SLA>;
	ie[0x27] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SLA, RegisterA>;

	ie[0x28] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SRA, RegisterB>;
	ie[0x29] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SRA, RegisterC>;
	ie[0x2A] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SRA, RegisterD>;
	ie[0x2B] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SRA, RegisterE>;
	ie[0x2C] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SRA, RegisterH>;
	ie[0x2D] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SRA, RegisterL>;
	ie[0x2E] = &CPU::PREFIXED_OP_ADDR_HL<PREFIXEDOP::SRA>;
	ie[0x2F] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SRA, RegisterA>;

	ie[0x30] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SWAP, RegisterB>;
	ie[0x31] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SWAP, RegisterC>;
	ie[0x32] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SWAP, RegisterD>;
	ie[0x33] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SWAP, RegisterE>;
	ie[0x34] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SWAP, RegisterH>;
	ie[0x35] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SWAP, RegisterL>;
	ie[0x36] = &CPU::PREFIXED_OP_ADDR_HL<PREFIXEDOP::SWAP>;
	ie[0x37] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SWAP, RegisterA>;

	ie[0x38] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SRL, RegisterB>;
	ie[0x39] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SRL, RegisterC>;
	ie[0x3A] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SRL, RegisterD>;
	ie[0x3B] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SRL, RegisterE>;
	ie[0x3C] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SRL, RegisterH>;
	ie[0x3D] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SRL, RegisterL>;
	ie[0x3E] = &CPU::PREFIXED_OP_ADDR_HL<PREFIXEDOP::SRL>;
	ie[0x3F] = &CPU::PREFIXED_OP_REG<PREFIXEDOP::SRL, RegisterA>;

	/* See comment in Prefixed.inl, above their implementation */
	/* Maybe use a compile time for loop to fill them, allowing to specify the opcode in order to perform compile time register and bit deduction */
	std::fill(ie.begin() + 0x40, ie.begin() + 0x80, &CPU::PREFIXED_OP_DEDUCE<PREFIXEDOP::BIT>);
	std::fill(ie.begin() + 0x80, ie.begin() + 0xC0, &CPU::PREFIXED_OP_DEDUCE<PREFIXEDOP::RES>);
	std::fill(ie.begin() + 0xC0, ie.end(), 			&CPU::PREFIXED_OP_DEDUCE<PREFIXEDOP::SET>);

	#undef ie
}