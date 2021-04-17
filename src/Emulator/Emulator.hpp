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
    sf::RenderWindow render_window;

    CPU m_cpu;
    MMU m_mmu;
    Joypad m_joypad;
    LCD m_lcd;
    Cartridge m_cartridge;
};


#endif //GAMEBOYCOLORPROJECT_EMULATOR_HPP
