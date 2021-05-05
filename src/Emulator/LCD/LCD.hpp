#ifndef GAMEBOYPROJECT_LCD_HPP
#define GAMEBOYPROJECT_LCD_HPP

#include <array>
#include <cstdio>

#include <SFML/Graphics.hpp>
#include "../Util/Types.hpp"

enum Mode
{
    HBLANK = 0, VBLANK = 1, LINE_SPRITES = 2, LINE_BACKGROUND = 3
};

class LCD {
public:
    LCD();

    void update(u8 t);
    sf::Image& draw();

    u8 read_byte(u16 memory_loc);
    u8 read_byte_at_oam(u8 memory_loc);

    void write_byte(u16 memory_loc, u8 value);
    void write_byte_at_oam(u8 memory_loc, u8 value);

    void reset();
    void updatetile(u16 addr);
    void renderscan();

    bool need_to_draw = false;
private:
    sf::Image to_render;


	std::array<u8, 8192> m_video_ram{0};
	std::array<u8, 160> m_oam{0};

    u8 m_framebuffer[144*160*4];
    int m_modeclock, m_line = 1;
    Mode m_mode = Mode::LINE_BACKGROUND;

    bool m_bgmap, m_bgtile, m_switchbg, m_switchlcd;
    u8 m_scx, m_scy;
    u8 m_pal[4][4];

    u8 m_tileset[384][8][8];

};


#endif //GAMEBOYPROJECT_LCD_HPP
