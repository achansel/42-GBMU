/*
 * TODO: Fix the timings
 */

#ifndef GAMEBOYPROJECT_EMULATOR_HPP
#define GAMEBOYPROJECT_EMULATOR_HPP

#include <string>


#include "CPU/CPU.hpp"
#include "Memory/MMU.hpp"
#include "Joypad/Joypad.hpp"
#include "Timer/Timer.hpp"
#include "LCD/LCD.hpp"
#include "Memory/Cartridge/Cartridge.hpp"

class Emulator {
public:
    Emulator(const std::string& path_to_game);
    ~Emulator();

    void run();
    void reset();

    void draw_frame();

    MMU& get_MMU();
    CPU& get_CPU();
    LCD& get_lcd();
	Timer& get_timer();
    Cartridge& get_cartridge();
    Joypad& get_joypad();
private:
    Cartridge m_cartridge;
    MMU m_mmu;
    CPU m_cpu;
    Joypad m_joypad;
    LCD m_lcd;
	Timer m_timer;
};


#endif //GAMEBOYCOLORPROJECT_EMULATOR_HPP
