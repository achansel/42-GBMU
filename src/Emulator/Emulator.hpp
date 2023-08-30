#pragma once

#include <string>
#include <iostream>

#include <Emulator/CPU/CPU.hpp>
#include <Emulator/Memory/MMU.hpp>
#include <Emulator/Joypad/Joypad.hpp>
#include <Emulator/Timer/Timer.hpp>
#include <Emulator/LCD/LCD.hpp>
#include <Emulator/Memory/Cartridge/Cartridge.hpp>

class Emulator {
public:
    Emulator(const std::string& path_to_game);
    ~Emulator();

    void run();
    void reset();

    MMU& get_MMU();
    CPU& get_CPU();
    LCD& get_lcd();
	Timer& get_timer();
    Cartridge& get_cartridge();
    Joypad& get_joypad();
private:
    Cartridge m_cartridge;
    CPU m_cpu;
    Joypad m_joypad;
    LCD m_lcd;
	Timer m_timer;
    MMU m_mmu;
};
