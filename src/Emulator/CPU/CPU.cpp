#include <Emulator/CPU/CPU.hpp>
#include <Emulator/Emulator.hpp>

CPU::CPU(Emulator* emu)
{
    m_emu = emu;
    m_regs[7] = 1;

	fill_instruction_tables();
}

void CPU::saveafterinstruction()
{
    if (savestate == nullptr)
        savestate = fopen("states.bin", "wb");
    fwrite((void*)m_regs, sizeof(u8), 10, savestate);
    fwrite((void*)&PC, sizeof(u16), 1, savestate);
}

void CPU::tick()
{
    execute_next_instruction();
    //saveafterinstruction();
    step_lcd();
}

void CPU::debug_stop()
{
	#ifndef NDEBUG
		std::cout << "GBMU: CPU FATAL:" << std::endl;
		u16 SP = GET_COMPOSED_REG(RegisterSP);
		printf("\tREGS: A: 0x%02X, B:0x%02X, C:0x%02X, D:0x%02X, E:0x%02X, H:0x%02X, L:0x%02X, F:0x%02X\n\t\tSP:0x%04X, PC:0x%04X\n", GET_REG(RegisterA), GET_REG(RegisterB), GET_REG(RegisterC), GET_REG(RegisterD), GET_REG(RegisterE), GET_REG(RegisterH), GET_REG(RegisterL), GET_REG(RegisterF), SP, PC);
		printf("\tHALTED OPCODE (could be wrong lol) (NOT XCUTED): %02X\n", m_opcode);
		std::cout << "\tat " << std::hex << SP << ": " << m_emu->get_MMU().get_word_at(SP) << std::endl;
		std::cout << "\tat " << std::hex << SP + 2 << ": " << m_emu->get_MMU().get_word_at(SP + 2) << std::endl;
		std::cout << "\tat " << std::hex << SP + 4 << ": " << m_emu->get_MMU().get_word_at(SP + 4) << std::endl;
		std::cout << "\tat " << std::hex << SP + 6 << ": " << m_emu->get_MMU().get_word_at(SP + 6) << std::endl;
		std::cout << "\tat " << std::hex << SP + 8 << ": " << m_emu->get_MMU().get_word_at(SP + 8) << std::endl;
		std::cout << "\tat " << std::hex << SP + 10 << ": " << m_emu->get_MMU().get_word_at(SP + 10) << std::endl;
		std::cout << "\tat " << std::hex << SP + 12 << ": " << m_emu->get_MMU().get_word_at(SP + 12) << std::endl;

		exit(1);
	#endif

}

inline void CPU::step_lcd()
{
    m_emu->get_lcd().update(m_tclock);
}

inline void CPU::fetch_instruction()
{
    m_opcode = m_emu->get_MMU().get_byte_at(PC);
	PC++; m_tclock += 4;
}

inline void CPU::execute_next_instruction()
{
	fetch_instruction();

    // Extended opcode table
    if (m_opcode == 0xCB)
    {
        fetch_instruction();
		auto operation = m_instructions_cb[m_opcode];
		(this->*operation)();
    }
    // Standard opcode table
    else
    {
		auto operation = m_instructions[m_opcode];
		(this->*operation)();
    }
}

u8 CPU::read_byte_at_working_ram(u16 position) {
    return m_working_ram[position];
}


void CPU::write_byte_at_working_ram(u16 position, u8 value) {
    m_working_ram[position] = value;
}
