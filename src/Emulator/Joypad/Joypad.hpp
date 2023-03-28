#pragma once

#include <SDL.h>
#include <array>

class Emulator;

#include <Emulator/Util/Types.hpp>
#include <Emulator/Util/Bitwise.hpp>

class Joypad {
public:
    Joypad(Emulator *emu);

    void    update(SDL_Event e);
    
    void    write_byte(u8 new_value);
    u8      read_byte();
private:
    void    key_up(SDL_Event e);
    void    key_down(SDL_Event e);

	Emulator	*m_emu;

    u8 m_column;
    std::array<u8, 2> m_rows = {0x0F, 0x0F};
};
