#pragma once

#include <iostream>
#include <array>
#include <cstdio>

#include <Emulator/Util/Types.hpp>

class Emulator;


#define ALWAYS_INLINE inline

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
    void execute_next_instruction();
    void saveafterinstruction();

    u8 regs[10];

	u8 m_ie_reg;
	u8 m_if_reg;

	bool m_ime;
    
    FILE* savestate = nullptr;
    
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
	ALWAYS_INLINE u8	FETCH_BYTE(u16 address);
	ALWAYS_INLINE u8	GET_BYTE(u16 address);

	ALWAYS_INLINE u16	FETCH_WORD(u16 address);
	ALWAYS_INLINE u16	GET_WORD(u16 address);

	ALWAYS_INLINE void	WRITE_BYTE(u16 address, u8 byte);
	ALWAYS_INLINE void	WRITE_WORD(u16 address, u16 word);

	/* Misc */
	ALWAYS_INLINE u8	GET_TARGET_BIT(u8 base, u8 op);

	/* INSTRUCTIONS FUNCTIONS MESS */
	void NOP();
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
