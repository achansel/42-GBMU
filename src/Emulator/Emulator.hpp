/*
 * TODO: Fix the timings
 */

#ifndef GAMEBOYPROJECT_EMULATOR_HPP
#define GAMEBOYPROJECT_EMULATOR_HPP

#include <string>

#include <SFML/Graphics.hpp>

#include "CPU/CPU.hpp"
#include "Memory/MMU.hpp"
#include "Joypad/Joypad.hpp"
#include "LCD/LCD.hpp"
#include "Memory/Cartridge/Cartridge.hpp"

class Emulator {
public:
    Emulator(const std::string& path_to_game);

    void run();
    void reset();

    void draw_frame();

    MMU& get_MMU();
    CPU& get_CPU();
    LCD& get_lcd();
    Cartridge& get_cartridge();
    Joypad& get_joypad();
private:
    Cartridge m_cartridge;
    MMU m_mmu;
    CPU m_cpu;
    Joypad m_joypad;
    sf::RenderWindow render_window;
    LCD m_lcd;
};


#endif //GAMEBOYCOLORPROJECT_EMULATOR_HPP
