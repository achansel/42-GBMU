#ifndef GAMEBOYPROJECT_JOYPAD_HPP
#define GAMEBOYPROJECT_JOYPAD_HPP

#include <array>

#include <SFML/Window/Event.hpp>
#include "../Util/Types.hpp"
#include "../Util/Bitwise.hpp"

class Joypad {
public:
    Joypad() = default;
    void update(sf::Event e);
    void write_byte(u8 new_value);
    u8 read_byte();
private:
    void key_up(sf::Event e);
    void key_down(sf::Event e);

    u8 m_column;
    std::array<u8, 2> m_rows = {0x0F, 0x0F};


};


#endif //GAMEBOYPROJECT_JOYPAD_HPP
