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

	#include "Instructions/Control.inl"
	#include "Instructions/LD.inl"
	#include "Instructions/ALU.inl"


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
};
