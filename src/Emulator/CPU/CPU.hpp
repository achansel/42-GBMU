#pragma once

#include <iostream>
#include <array>
#include <cstdio>

#include <Emulator/Memory/IMMIO.hpp>

#include <Emulator/Util/Types.hpp>

class Emulator;

#define ALWAYS_INLINE

enum Register {RegisterB = 0, RegisterC = 1, RegisterD = 2, RegisterE = 3, RegisterH = 4, RegisterL = 5, RegisterF = 6, RegisterA = 7};
enum ComposedRegister {RegisterBC = 0x01, RegisterDE = 0x23, RegisterHL = 0x45, RegisterAF = 0x76, RegisterSP = 0x98};
enum Flag {ZeroFlag = 7, SubstractFlag = 6, HalfCarryFlag = 5, CarryFlag = 4};

class CPU : public IMMIO {
public:
	enum Interrupt {
		VBLANK	= 1 << 0,
		STAT	= 1 << 1,
		TIMER	= 1 << 2,
		SERIAL	= 1 << 3,
		JOYPAD	= 1 << 4
	};

    explicit CPU(Emulator *emu);
	~CPU() {}

    void tick();

    u8 read_byte(u16 position);
    void write_byte(u16 position, u8 value);

	u8 read_if();
	void write_if(u8 val);

	void check_and_service_interrupts();
	void request_interrupt(Interrupt i);

	u8 read_ie(u16 address);
	void write_ie(u16 address, u8 value);

    u32 m_mclock = 0, m_tclock = 0;
    bool m_exit;

    u16 PC;

	void debug_stop();

	//TODO: STOPPING OF THE CPU
	bool is_stopped() { return (false); }
private:
	typedef void (CPU::*InstructionPtr)(void);

	void fetch_instruction();
    void execute_next_instruction();
    void saveafterinstruction();

	void fill_instructions_table();
	void fill_instructions_table_cb();

	void service_interrupt(Interrupt i);

	std::array<InstructionPtr, 256> m_instructions_cb;
	std::array<InstructionPtr, 256> m_instructions;

    u8 m_regs[10];
	u8 m_ie_reg;
	u8 m_if_reg;
	bool m_ime;

	bool m_halted;
    
    u8 m_opcode;

    Emulator* m_emu;
	std::array<u8, 8192> m_working_ram{ 0 };

	/* MESS STARTING FROM HERE */
	ALWAYS_INLINE u8	GET_REG(Register reg);
	ALWAYS_INLINE void	SET_REG(Register reg, u8 value);
	ALWAYS_INLINE void	INC_REG(Register reg);
	ALWAYS_INLINE void	DEC_REG(Register reg);

	ALWAYS_INLINE u8	GET_REG_BIT(Register reg, u8 bit);
	ALWAYS_INLINE void	SET_REG_BIT(Register reg, u8 bit, u8 value);

	ALWAYS_INLINE u16	GET_COMPOSED_REG(ComposedRegister reg);
	ALWAYS_INLINE void	SET_COMPOSED_REG(ComposedRegister reg, u16 value);
	ALWAYS_INLINE void	INC_COMPOSED_REG(ComposedRegister reg);
	ALWAYS_INLINE void	DEC_COMPOSED_REG(ComposedRegister reg);

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
	#include "Instructions/System.inl"
	#include "Instructions/Prefixed.inl"

	void UNDEFINED() { std::cout << "GBMU: FATAL: UNDEFINED OPCODE; SYSTEM HALTED" << std::endl; debug_stop(); }
	void UNDEFINED_CB() { std::cout << "GBMU: FATAL: UNDEFINED OPCODE CB; SYSTEM HALTED" << std::endl; debug_stop(); }
};
