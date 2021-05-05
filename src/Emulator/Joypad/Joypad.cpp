#include <iostream>
#include "Joypad.hpp"

void Joypad::update(sf::Event e)
{
    if (e.type == sf::Event::EventType::KeyPressed) key_down(e);
    if (e.type == sf::Event::EventType::KeyReleased)  key_up(e);
}

void Joypad::write_byte(u8 value)
{
    m_column = static_cast<u8>(value & 0x30);
}

u8 Joypad::read_byte()
{
    switch (m_column)
    {
        case 0x10: return m_rows[0];
        case 0x20: return m_rows[1];
        default:   return 0;
    }
}

void Joypad::key_down(sf::Event e)
{
    switch (e.key.code)
    {
        case sf::Keyboard::S:
            m_rows[0] &= 0xE; // A gameboy key
            break;
        case sf::Keyboard::X:
            m_rows[0] &= 0xD; // B gameboy key
            break;
        case sf::Keyboard::Space:
            m_rows[0] &= 0xB; // SELECT gameboy key
            break;
        case sf::Keyboard::Return:
            m_rows[0] &= 0x7; // ENTER gameboy key
            break;
        case sf::Keyboard::Right:
            m_rows[1] &= 0xE;
            break;
        case sf::Keyboard::Left:
            m_rows[1] &= 0xD;
            break;
        case sf::Keyboard::Up:
            m_rows[1] &= 0xB;
            break;
        case sf::Keyboard::Down:
            m_rows[1] &= 0x7;
            break;
        default:
            break;
    }
}

void Joypad::key_up(sf::Event e)
{
    switch (e.key.code) {
        case sf::Keyboard::S:
            m_rows[0] |= 0x1; // A gameboy key
            break;
        case sf::Keyboard::X:
            m_rows[0] |= 0x2; // B gameboy key
            break;
        case sf::Keyboard::Space:
            m_rows[0] |= 0x4; // SELECT gameboy key
            break;
        case sf::Keyboard::Return:
            m_rows[0] |= 0x8; // ENTER gameboy key
            break;
        case sf::Keyboard::Right:
            m_rows[1] |= 0x1;
            break;
        case sf::Keyboard::Left:
            m_rows[1] |= 0x2;
            break;
        case sf::Keyboard::Up:
            m_rows[1] |= 0x4;
            break;
        case sf::Keyboard::Down:
            m_rows[1] |= 0x8;
            break;
        default:
            break;
    }
}