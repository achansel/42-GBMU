#pragma once

#include <iostream>
#include <array>
#include <cstdio>

#include <Emulator/Util/Types.hpp>

class Emulator;


#define ALWAYS_INLINE

enum Register {RegisterB = 0, RegisterC = 1, RegisterD = 2, RegisterE = 3, RegisterH = 4, RegisterL = 5, RegisterF = 6, RegisterA = 7};
enum ComposedRegister {RegisterBC = 0x01, RegisterDE = 0x23, RegisterHL = 0x45, RegisterAF = 0x76, RegisterSP = 0x98};
enum Flag {ZeroFlag = 7, SubstractFlag = 6, HalfCarryFlag = 5, CarryFlag = 4};

class CPU {
public:
    explicit CPU(Emulator *emu);

    void tick();
    void step_lcd();

    u8 read_byte_at_working_ram(u16 position);
    void write_byte_at_working_ram(u16 position, u8 value);

	u8 read_if();
	void write_if(u8 value);

	u8 read_ie();
	void write_ie(u8 value);

    u32 m_mclock = 0, m_tclock = 0;
    bool m_exit;

    u16 PC;

	void debug_stop();
private:
	typedef void (CPU::*InstructionPtr)(void);

	void fetch_instruction();
    void execute_next_instruction();
    void saveafterinstruction();

	void fill_instruction_tables();

	std::array<InstructionPtr, 256> m_instructions_cb;
	std::array<InstructionPtr, 256> m_instructions;

    u8 m_regs[10];
	u8 m_ie_reg;
	u8 m_if_reg;
	bool m_ime;
    

    FILE* savestate = nullptr;
    u8 m_opcode;

    Emulator* m_emu;
	std::array<u8, 8192> m_working_ram{ 0 };

	/* MESS STARTING FROM HERE */
	ALWAYS_INLINE u8	GET_REG(u8 reg);
	ALWAYS_INLINE void	SET_REG(u8 reg, u8 value);
	ALWAYS_INLINE void	INC_REG(u8 reg);
	ALWAYS_INLINE void	DEC_REG(u8 reg);

	ALWAYS_INLINE u8	GET_REG_BIT(u8 reg, u8 bit);
	ALWAYS_INLINE void	SET_REG_BIT(u8 reg, u8 bit, u8 value);

	ALWAYS_INLINE u16	GET_COMPOSED_REG(u8 reg);
	ALWAYS_INLINE void	SET_COMPOSED_REG(u8 reg, u16 value);
	ALWAYS_INLINE void	INC_COMPOSED_REG(u8 reg);
	ALWAYS_INLINE void	DEC_COMPOSED_REG(u8 reg);

	ALWAYS_INLINE void	SET_FLAG(u8 flag, u8 value);
	ALWAYS_INLINE u8	GET_FLAG(u8 flag);

	/* Memory instructions */
	ALWAYS_INLINE u8	FETCH_BYTE();
	ALWAYS_INLINE u8	GET_BYTE(u16 address);

	ALWAYS_INLINE u16	FETCH_WORD();
	ALWAYS_INLINE u16	GET_WORD(u16 address);

	ALWAYS_INLINE void	WRITE_BYTE(u16 address, u8 byte);
	ALWAYS_INLINE void	WRITE_WORD(u16 address, u16 word);

	/* Misc */
	ALWAYS_INLINE u8	GET_TARGET_BIT(u8 base, u8 op);

	ALWAYS_INLINE void 	MOV_ADDR_8(u16 address, u8 value);
	ALWAYS_INLINE void 	MOV_ADDR_16(u16 address, u16 value);
	ALWAYS_INLINE void 	MOV_REG_8(u8 reg, u8 value);
	ALWAYS_INLINE void 	MOV_REG_16(u8 reg, u16 value);
	ALWAYS_INLINE void 	MOV_ADDR_REG8(u16 address, u8 reg);
	ALWAYS_INLINE void 	MOV_ADDR_REG16(u16 address, u8 reg);
	ALWAYS_INLINE void 	MOV_REG8_REG8(u8 dst, u8 src);
	ALWAYS_INLINE void 	MOV_REG16_REG16(u8 dst, u8 src);
	ALWAYS_INLINE void 	MOV_REG8_IMM8(u8 dst);
	ALWAYS_INLINE void 	MOV_REG16_IMM16(u8 dst);
	ALWAYS_INLINE void 	MOV_REG8_ADDR(u8 dst, u16 address);
	ALWAYS_INLINE void 	POP_REG16(u8 dst);
	ALWAYS_INLINE void 	PUSH_REG16(u8 src);


	/* Micro instructions */
	template<int SIGN, int CARRY>
	ALWAYS_INLINE void ADD_A_8(u8 operand)	
	{
		u16 res = GET_FLAG(CarryFlag) * CARRY + operand * SIGN + GET_REG(RegisterA);
		SET_FLAG(ZeroFlag, static_cast<u8>(res) == 0);
		SET_FLAG(SubstractFlag, SIGN == -1);
		SET_FLAG(HalfCarryFlag, (res >> 4) & 1);
		SET_FLAG(CarryFlag, (res >> 8) & 1);
		SET_REG(RegisterA, static_cast<u8>(res));
	}
	ALWAYS_INLINE void ADD_REG16_REG16(u8 dst, u8 src)
	{
		m_tclock += 4;

		u32 res = GET_COMPOSED_REG(dst) + GET_COMPOSED_REG(src);
		SET_FLAG(SubstractFlag, 0);
		SET_FLAG(HalfCarryFlag, (res >> 8) & 1);
		SET_FLAG(CarryFlag, (res >> 16) & 1);
		SET_COMPOSED_REG(dst, static_cast<u16>(res));
	}
	ALWAYS_INLINE void ADC(u8 operand)			{ ADD_A_8<1, 1>(operand); }
	ALWAYS_INLINE void SBC(u8 operand)			{ ADD_A_8<-1, 1>(operand); }
	ALWAYS_INLINE void ADD(u8 operand)			{ ADD_A_8<1, 0>(operand); }
	ALWAYS_INLINE void SUB(u8 operand)			{ ADD_A_8<-1, 0>(operand); }
	ALWAYS_INLINE void DEC_REG16(u8 reg) 		{ m_tclock += 4; DEC_COMPOSED_REG(reg); }
	ALWAYS_INLINE void INC_REG16(u8 reg) 		{ m_tclock += 4; INC_COMPOSED_REG(reg); }
	ALWAYS_INLINE void DEC_REG8(u8 reg)  		{ DEC_REG(reg); SET_FLAG(ZeroFlag, GET_REG(reg) == 0); SET_FLAG(SubstractFlag, 1); SET_FLAG(HalfCarryFlag, (GET_REG(reg) >> 4) & 1); }
	ALWAYS_INLINE void INC_REG8(u8 reg) 		{ INC_REG(reg); SET_FLAG(ZeroFlag, GET_REG(reg) == 0); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, (GET_REG(reg) >> 4) & 1); }
	ALWAYS_INLINE void DEC_ADDR(u16 address)	{ u8 b = GET_BYTE(address) - 1; SET_FLAG(ZeroFlag, b == 0); SET_FLAG(SubstractFlag, 1); SET_FLAG(HalfCarryFlag, (b >> 4) & 1); WRITE_BYTE(address, b); }
	ALWAYS_INLINE void INC_ADDR(u16 address)	{ u8 b = GET_BYTE(address) + 1; SET_FLAG(ZeroFlag, b == 0); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, (b >> 4) & 1); WRITE_BYTE(address, b); }

	ALWAYS_INLINE void AND(u8 operand)			{ u8 a = GET_REG(RegisterA) & operand; SET_REG(RegisterA, a); SET_FLAG(ZeroFlag, a == 0); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, 1); SET_FLAG(CarryFlag, 0); }
	ALWAYS_INLINE void XOR(u8 operand)			{ u8 a = GET_REG(RegisterA) ^ operand; SET_REG(RegisterA, a); SET_FLAG(ZeroFlag, a == 0); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, 0); SET_FLAG(CarryFlag, 0); }
	ALWAYS_INLINE void OR(u8 operand)			{ u8 a = GET_REG(RegisterA) | operand; SET_REG(RegisterA, a); SET_FLAG(ZeroFlag, a == 0); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, 0); SET_FLAG(CarryFlag, 0); }
	ALWAYS_INLINE void CP(u8 operand)
	{
		u8 a = GET_REG(RegisterA);
		SET_FLAG(ZeroFlag, a == operand);
		SET_FLAG(SubstractFlag, 1);
		SET_FLAG(HalfCarryFlag, ((a - operand) & 0xF) > (a & 0xF));
		SET_FLAG(CarryFlag, a < operand);
	}
	ALWAYS_INLINE void _JUMP(u16 address)	{ PC = address;}
	ALWAYS_INLINE void _RET()				{ u16 sp = GET_COMPOSED_REG(RegisterSP); PC = GET_WORD(sp); SET_COMPOSED_REG(RegisterSP, sp + 2); }
	ALWAYS_INLINE void _CALL(u16 address)	{ u16 sp = GET_COMPOSED_REG(RegisterSP) - 2; WRITE_WORD(sp, PC); SET_COMPOSED_REG(RegisterSP, sp); _JUMP(address); }
	u8 RLC(u8 operand)
	{
		u8 a = operand;
		a = (a << 1) | (a >> 7);

		SET_FLAG(ZeroFlag, a == 0);
		SET_FLAG(SubstractFlag, 0);
		SET_FLAG(HalfCarryFlag, 0);
		SET_FLAG(CarryFlag, (a >> 0) & 1);

		return (a);
	}

	u8 RRC(u8 operand)
	{
		u8 a = operand;
		a = (a >> 1) | (a << 7);

		SET_FLAG(ZeroFlag, a == 0);
		SET_FLAG(SubstractFlag, 0);
		SET_FLAG(HalfCarryFlag, 0);
		SET_FLAG(CarryFlag, (a >> 7) & 1); //maybe wrong swap & 1 and & 0x80

		return (a);
	}

	u8 RL(u8 operand)
	{
		u8 a = operand;
		a = (a << 1) | (GET_FLAG(CarryFlag) << 0);

		SET_FLAG(ZeroFlag, a == 0);
		SET_FLAG(SubstractFlag, 0);
		SET_FLAG(HalfCarryFlag, 0);
		SET_FLAG(CarryFlag, (operand >> 7) & 1); //maybe wrong swap & 1 and & 0x80

		return (a);
	}

	u8 RR(u8 operand)
	{
		u8 a = operand;
		a = (a >> 1) | (GET_FLAG(CarryFlag) << 7);

		SET_FLAG(ZeroFlag, a == 0);
		SET_FLAG(SubstractFlag, 0);
		SET_FLAG(HalfCarryFlag, 0);
		SET_FLAG(CarryFlag, operand & 1); //maybe wrong swap & 1 and & 0x80

		return (a);
	}

	u8 SRA(u8 operand)
	{
		u8 a = operand;
		a = (a >> 1) | (a & 0x80);

		SET_FLAG(ZeroFlag, a == 0);
		SET_FLAG(SubstractFlag, 0);
		SET_FLAG(HalfCarryFlag, 0);
		SET_FLAG(CarryFlag, operand & 0x01);

		return a;
	}

	u8 SLA(u8 operand)
	{
		u8 a = operand;
		a = ((a << 1) & ~0x80) | (a & 0x80);

		SET_FLAG(ZeroFlag, a == 0);
		SET_FLAG(SubstractFlag, 0);
		SET_FLAG(HalfCarryFlag, 0);
		SET_FLAG(CarryFlag, (operand >> 7) & 0x01);

		return a;
	}

	u8 SWAP(u8 operand)
	{
		u8 a = operand;
		a = (a >> 4) | ((a & 0xf) << 4);

		SET_FLAG(ZeroFlag, a == 0);
		SET_FLAG(SubstractFlag, 0);
		SET_FLAG(HalfCarryFlag, 0);
		SET_FLAG(CarryFlag, 0);

		return a;
	}


	u8 SRL(u8 operand)
	{
		u8 a = operand;
		a <<= 1;

		SET_FLAG(ZeroFlag, a == 0);
		SET_FLAG(SubstractFlag, 0);
		SET_FLAG(HalfCarryFlag, 0);
		SET_FLAG(CarryFlag, (operand >> 7) & 0x01);

		return a;
	}
	void	BIT_N_8(u8 bit, u8 val) { SET_FLAG(ZeroFlag, !((val >> bit) & 1)); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, 1); }
	u8		RES_N_8(u8 bit, u8 val) { return (val & ~(1 << bit)); }
	u8		SET_N_8(u8 bit, u8 val) { return (val | (1 << bit)); }


	/* INSTRUCTIONS FUNCTIONS MESS */
	void UNDEFINED() { std::cout << "GBMU: FATAL: UNDEFINED OPCODE; SYSTEM HALTED" << std::endl; debug_stop(); }
	void UNDEFINED_CB() { std::cout << "GBMU: FATAL: UNDEFINED OPCODE CB; SYSTEM HALTED" << std::endl; debug_stop(); }
	void NOP();
	void DI();
	void EI();
	void CALL_IMM16();
	void CALL_NC_IMM16();
	void CALL_NZ_IMM16();
	void CALL_C_IMM16();
	void CALL_Z_IMM16();
	void RST_00();
	void RST_08();
	void RST_10();
	void RST_18();
	void RST_20();
	void RST_28();
	void RST_30();
	void RST_38();
	void LD_BC_d16();
	void JUMP_IMM16();
	void LD_DE_d16();
	void JUMP_NC_IMM16();
	void LD_HL_d16();
	void JUMP_NZ_IMM16();
	void LD_SP_d16();
	void JUMP_C_IMM16();
	void JUMP_Z_IMM16();
	void LD_ADDR_BC_A();
	void JUMP_HL();
	void LD_ADDR_DE_A();
	void LD_ADDR_HLI_A();
	void JR_IMM8();
	void LD_ADDR_HLD_A();
	void JR_NC_IMM8();
	void JR_NZ_IMM8();
	void LD_B_d8();
	void JR_C_IMM8();
	void LD_D_d8();
	void JR_Z_IMM8();
	void LD_H_d8();
	void LD_ADDR_HL_d8();
	void RET();
	void LD_ADDR_d16_SP(); 
	void RETI();
	void RET_NC();
	void LD_A_ADDR_BC();
	void RET_NZ();
	void LD_A_ADDR_DE();
	void RET_C();
	void LD_A_ADDR_HLI();
	void RET_Z();
	void LD_A_ADDR_HLD();
	void LD_C_d8();
	void LD_E_d8();
	void LD_L_d8();
	void LD_A_d8();
	void LD_A_A();
	void LD_B_A();
	void LD_C_A();
	void DEC_A();
	void LD_D_A();
	void DEC_B();
	void LD_E_A();
	void DEC_C();
	void LD_H_A();
	void DEC_D();
	void LD_L_A();
	void DEC_E();
	void LD_ADDR_HL_A();
	void DEC_H();
	void DEC_L();
	void LD_A_B();
	void DEC_ADDR_HL();
	void LD_B_B();
	void LD_C_B();
	void DEC_BC();
	void LD_D_B();
	void DEC_DE();
	void LD_E_B();
	void DEC_HL();
	void LD_H_B();
	void DEC_SP();
	void LD_L_B();
	void LD_ADDR_HL_B();
	void INC_A();
	void INC_B();
	void LD_A_C();
	void INC_C();
	void LD_B_C();
	void INC_D();
	void LD_C_C();
	void INC_E();
	void LD_D_C();
	void INC_H();
	void LD_E_C();
	void INC_L();
	void LD_H_C();
	void INC_ADDR_HL();
	void LD_L_C();
	void LD_ADDR_HL_C();
	void INC_BC();
	void INC_DE();
	void LD_A_D();
	void INC_HL();
	void LD_B_D();
	void INC_SP();
	void LD_C_D();
	void LD_D_D();
	void LD_E_D();
	void LD_H_D();
	void LD_L_D();
	void ADD_A_A();
	void LD_ADDR_HL_D();
	void ADD_A_B();
	void ADD_A_C();
	void LD_A_E();
	void ADD_A_D();
	void LD_B_E();
	void ADD_A_E();
	void LD_C_E();
	void ADD_A_H();
	void LD_D_E();
	void ADD_A_L();
	void LD_E_E();
	void ADD_A_ADDR_HL();
	void LD_H_E();
	void ADD_A_IMM8();
	void LD_L_E();
	void LD_ADDR_HL_E();
	void ADC_A_A();
	void ADC_A_B();
	void LD_A_H();
	void ADC_A_C();
	void LD_B_H();
	void ADC_A_D();
	void LD_C_H();
	void ADC_A_E();
	void LD_D_H();
	void ADC_A_H();
	void LD_E_H();
	void ADC_A_L();
	void LD_H_H();
	void ADC_A_ADDR_HL();
	void LD_L_H();
	void ADC_A_IMM8();
	void LD_ADDR_HL_H();
	void LD_A_L();
	void SUB_A_A();
	void LD_B_L();
	void SUB_A_B();
	void LD_C_L();
	void SUB_A_C();
	void LD_D_L();
	void SUB_A_D();
	void LD_E_L();
	void SUB_A_E();
	void LD_H_L();
	void SUB_A_H();
	void LD_L_L();
	void SUB_A_L();
	void LD_ADDR_HL_L();
	void SUB_A_ADDR_HL();
	void SUB_A_IMM8();
	void LD_A_ADDR_HL();
	void LD_B_ADDR_HL();
	void SBC_A_A();
	void LD_C_ADDR_HL();
	void SBC_A_B();
	void LD_D_ADDR_HL();
	void SBC_A_C();
	void LD_E_ADDR_HL();
	void SBC_A_D();
	void LD_H_ADDR_HL();
	void SBC_A_E();
	void LD_L_ADDR_HL();
	void SBC_A_H();
	void SBC_A_L();
	void LDH_IMM8_A();
	void SBC_A_ADDR_HL();
	void LDH_A_IMM8();
	void SBC_A_IMM8();
	void ADD_HL_BC();
	void LD_ADDR_C_A();
	void ADD_HL_DE();
	void LD_A_ADDR_C();
	void ADD_HL_HL();
	void ADD_HL_SP();
	void LD_HL_SP_X_IMM8();
	void ADD_SP_IMM8();
	void RLC_A();
	void RLC_B();
	void RLC_C();
	void RLC_D();
	void RLC_E();
	void RLC_H();
	void RLC_L();
	void RLC_ADDR_HL();
	void LD_SP_HL();
	void RRC_A();
	void RRC_B();
	void LD_ADDR_IMM16_A();
	void RRC_C();
	void LD_A_ADDR_IMM16();
	void RRC_D();
	void AND_A_A();
	void RRC_E();
	void AND_A_B();
	void POP_BC();
	void RRC_H();
	void AND_A_C();
	void POP_DE();
	void RRC_L();
	void AND_A_D();
	void POP_HL();
	void RRC_ADDR_HL();
	void AND_A_E();
	void POP_AF();
	void AND_A_H();
	void PUSH_BC();
	void RL_A();
	void AND_A_L();
	void PUSH_DE();
	void RL_B();
	void AND_A_ADDR_HL();
	void PUSH_HL();
	void RL_C();
	void AND_A_IMM8();
	void PUSH_AF();
	void RL_D();
	void RL_E();
	void OR_A_A();
	void RL_H();
	void OR_A_B();
	void RL_L();
	void OR_A_C();
	void RL_ADDR_HL();
	void OR_A_D();
	void OR_A_E();
	void OR_A_H();
	void RR_A();
	void OR_A_L();
	void RR_B();
	void OR_A_ADDR_HL();
	void RR_C();
	void OR_A_IMM8();
	void RR_D();
	void RR_E();
	void XOR_A_A();
	void RR_H();
	void XOR_A_B();
	void RR_L();
	void XOR_A_C();
	void RR_ADDR_HL();
	void XOR_A_D();
	void XOR_A_E();
	void SLA_A();
	void XOR_A_H();
	void SLA_B();
	void XOR_A_L();
	void SLA_C();
	void XOR_A_ADDR_HL();
	void SLA_D();
	void XOR_A_IMM8();
	void SLA_E();
	void SLA_H();
	void CP_A_A();
	void SLA_L();
	void CP_A_B();
	void SLA_ADDR_HL();
	void CP_A_C();
	void CP_A_D();
	void SRA_A();
	void CP_A_E();
	void SRA_B();
	void CP_A_H();
	void SRA_C();
	void CP_A_L();
	void SRA_D();
	void CP_A_ADDR_HL();
	void SRA_E();
	void CP_A_IMM8();
	void SRA_H();
	void SRA_L();
	void SRA_ADDR_HL();
	void DAA();
	void SRL_A();
	void SRL_B();
	void SRL_C();
	void SRL_D();
	void SRL_E();
	void SRL_H();
	void SRL_L();
	void SRL_ADDR_HL();
	void SWAP_A();
	void SWAP_B();
	void SWAP_C();
	void SWAP_D();
	void SWAP_E();
	void SWAP_H();
	void SWAP_L();
	void SWAP_ADDR_HL();
	void BIT_N_A();
	void BIT_N_B();
	void BIT_N_C();
	void BIT_N_D();
	void BIT_N_E();
	void BIT_N_H();
	void BIT_N_L();
	void BIT_N_ADDR_HL();
	void CPL();
	void RES_N_A();
	void RES_N_B();
	void RES_N_C();
	void RES_N_D();
	void RES_N_E();
	void RES_N_H();
	void RES_N_L();
	void RES_N_ADDR_HL();
	void CCF();
	void SET_N_A();
	void SET_N_B();
	void SET_N_C();
	void SET_N_D();
	void SET_N_E();
	void SET_N_H();
	void SET_N_L();
	void SET_N_ADDR_HL();
	void SCF();
};
