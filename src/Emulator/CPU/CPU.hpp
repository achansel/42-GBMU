#pragma once

#include <iostream>
#include <array>
#include <cstdio>

#include "Emulator/Util/Types.hpp"

class Emulator;

class CPU {
public:
    explicit CPU(Emulator *emu);

    void tick();
    void step_lcd();

    u8 read_byte_at_working_ram(u16 position);
    void write_byte_at_working_ram(u16 position, u8 value);

    u32 mclock = 0, tclock = 0;
    bool m_exit;

    u16 SP, PC;
private:
    void execute_next_instruction();
    void saveafterinstruction();

    u8 regs[8];
    
    FILE* savestate = nullptr;
    
    Emulator* m_emu;
	std::array<u8, 8192> m_working_ram{ 0 };

};
