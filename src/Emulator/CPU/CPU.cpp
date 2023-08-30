#include <Emulator/CPU/CPU.hpp>
#include <Emulator/Emulator.hpp>

#include <iomanip>

CPU::CPU(Emulator* emu)
{
    m_emu = emu;
	m_halted = false;

	PC=0;
	m_exit=false;

	fill_instructions_table();
	fill_instructions_table_cb();
}

void CPU::saveafterinstruction()
{
}

void CPU::tick()
{
	while (m_emu->get_lcd().need_to_draw && !m_exit)
		;
    execute_next_instruction();
    //saveafterinstruction();
    m_emu->get_lcd().update(m_tclock);
	m_emu->get_timer().update(m_tclock);
	m_tclock = 0;
}

void CPU::debug_stop()
{
	#ifndef NDEBUG
		u16 SP = GET_COMPOSED_REG(RegisterSP);
		
		std::cout << "GBMU: CPU FATAL:" << std::endl;
		std::cout << "\tHALTED OPCODE: " << std::hex << +m_opcode << std::endl;
		std::cout << "\tREGS: " << std::hex
			<< "A: 0x" << std::setfill('0') << std::setw(2) << +GET_REG(RegisterA) << ", "
			<< "B: 0x" << std::setfill('0') << std::setw(2) << +GET_REG(RegisterB) << ", "
			<< "C: 0x" << std::setfill('0') << std::setw(2) << +GET_REG(RegisterC) << ", "
			<< "D: 0x" << std::setfill('0') << std::setw(2) << +GET_REG(RegisterD) << ", "
			<< "E: 0x" << std::setfill('0') << std::setw(2) << +GET_REG(RegisterE) << ", "
			<< "H: 0x" << std::setfill('0') << std::setw(2) << +GET_REG(RegisterH) << ", "
			<< "L: 0x" << std::setfill('0') << std::setw(2) << +GET_REG(RegisterL) << ", "
			<< "F: 0x" << std::setfill('0') << std::setw(2) << +GET_REG(RegisterF) << "\n\t"
			<< "SP: 0x" << std::setfill('0') << std::setw(4) << SP << ", "
			<< "PC: 0x" << std::setfill('0') << std::setw(4) << PC << std::endl;
		std::cout << "      STACK: " << std::endl;
		for (size_t i = 0; i < 16 && SP + i < 0x10000; i += 2 )
			std::cout << "\tat " << std::hex << SP + i << " (SP + " << i << "): " << std::setfill('0') << std::setw(4) << m_emu->get_MMU().get_word_at(SP + i) << std::endl;
		m_emu->get_cartridge().debug();

		exit(1);
	#else
		std::cout << "GBMU: FATAL: CPU ERROR ENCOUNTERED" << std::endl;
		exit(1);
	#endif

}

inline void CPU::fetch_instruction()
{
    m_opcode = m_emu->get_MMU().get_byte_at(PC);
	PC++; m_tclock += 4;
}

inline void CPU::execute_next_instruction()
{
	/* Check and handle interrupts if the previous instruction was not EI or RETI */
	if (m_opcode != 0xFB && m_opcode != 0xD9)
		check_and_service_interrupts();
	
	/* If the CPU is halted here, that means no interrupts were handled, add 4 to t_clock to allow the screen and the timer to step */
	if (m_halted)
	{
		m_tclock += 4;
		return ;
	}

	/* Execute the next instruction */
	fetch_instruction();
    if (m_opcode == 0xCB)
    {
        fetch_instruction();
		auto operation = m_instructions_cb[m_opcode];
		(this->*operation)();
    }
    else
    {
		auto operation = m_instructions[m_opcode];
		(this->*operation)();
    }
}


//TODO: HALT BUG
void CPU::check_and_service_interrupts()
{
	/* No interrupts to handle ? -> return */
	u8 masked = m_ie_reg & m_if_reg;
	if (!masked)
		return ;

	/* Unhalt the CPU */
	if (m_halted)
		m_halted = false;

	// Handle the interrupt, while taking the priority into account
	if (m_ime)
	{
		// Maybe use for loop
		if (masked & Interrupt::VBLANK)
			service_interrupt(Interrupt::VBLANK);
		else if (masked & Interrupt::STAT)
			service_interrupt(Interrupt::STAT);
		else if (masked & Interrupt::TIMER)
			service_interrupt(Interrupt::TIMER);
		else if (masked & Interrupt::SERIAL)
			service_interrupt(Interrupt::SERIAL);
		else if (masked & Interrupt::JOYPAD)
			service_interrupt(Interrupt::JOYPAD);
	}
}

void CPU::service_interrupt(Interrupt i)
{
	// NOPS
	NOP();
	NOP();
	
	// PUSH ADDRESS OF CURRENT PLACE
	u16 sp = GET_COMPOSED_REG(RegisterSP) - 2;
	SET_COMPOSED_REG(RegisterSP, sp);
	MOV_ADDR_16(sp, PC);

	// DISABLE INTERRUPTS, REMOVE FLAG AND JUMP TO HANDLER
	m_ime = false;
	m_if_reg &= ~i;
	if (i == Interrupt::VBLANK)
		_JUMP(0x40);
	else if (i == Interrupt::STAT)
		_JUMP(0x48);
	else if (i == Interrupt::TIMER)
		_JUMP(0x50);
	else if (i == Interrupt::SERIAL)
		_JUMP(0x58);
	else if (i == Interrupt::JOYPAD)
		_JUMP(0x60);
}

u8 CPU::read_if() { return (m_if_reg); }
void CPU::write_if(u8 value) { m_if_reg = value; }
void CPU::request_interrupt(Interrupt i) { m_if_reg |= static_cast<u8>(i); }

u8 CPU::read_ie(u16 address) { (void) address; return (m_ie_reg); }
void CPU::write_ie(u16 address, u8 value) { (void) address; m_ie_reg = value; }


u8 CPU::read_byte(u16 position) {
    return m_working_ram[position & 0x1FFF];
}


void CPU::write_byte(u16 position, u8 value) {
    m_working_ram[position & 0x1FFF] = value;
}
