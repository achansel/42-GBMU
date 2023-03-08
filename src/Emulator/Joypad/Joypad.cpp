#include <iostream>
#include "Joypad.hpp"

void Joypad::update(SDL_Event e)
{
    if (e.type == SDL_KEYDOWN)  key_down(e);
    if (e.type == SDL_KEYUP)    key_up(e);
}

void Joypad::write_byte(u8 value)
{
    m_column = static_cast<u8>(value & 0x30);
}

// TODO: FIX A BUG??
u8 Joypad::read_byte()
{
    switch (m_column)
    {
        case 0x10: return m_rows[0];
        case 0x20: return m_rows[1];
        default:   return 0;
    }
}

void Joypad::key_down(SDL_Event e)
{
    switch (e.key.keysym.sym)
    {
        case SDLK_s:
            m_rows[0] &= 0xE; // A gameboy key
            break;
        case SDLK_x:
            m_rows[0] &= 0xD; // B gameboy key
            break;
        case SDLK_SPACE:
            m_rows[0] &= 0xB; // SELECT gameboy key
            break;
        case SDLK_RETURN:
            m_rows[0] &= 0x7; // ENTER gameboy key
            break;
        case SDLK_RIGHT:
            m_rows[1] &= 0xE;
            break;
        case SDLK_LEFT:
            m_rows[1] &= 0xD;
            break;
        case SDLK_UP:
            m_rows[1] &= 0xB;
            break;
        case SDLK_DOWN:
            m_rows[1] &= 0x7;
            break;
        default:
            break;
    }
}

void Joypad::key_up(SDL_Event e)
{
    switch (e.key.keysym.sym) {
        case SDLK_s:
            m_rows[0] |= 0x1; // A gameboy key
            break;
        case SDLK_x:
            m_rows[0] |= 0x2; // B gameboy key
            break;
        case SDLK_SPACE:
            m_rows[0] |= 0x4; // SELECT gameboy key
            break;
        case SDLK_RETURN:
            m_rows[0] |= 0x8; // ENTER gameboy key
            break;
        case SDLK_RIGHT:
            m_rows[1] |= 0x1;
            break;
        case SDLK_LEFT:
            m_rows[1] |= 0x2;
            break;
        case SDLK_UP:
            m_rows[1] |= 0x4;
            break;
        case SDLK_DOWN:
            m_rows[1] |= 0x8;
            break;
        default:
            break;
    }
}