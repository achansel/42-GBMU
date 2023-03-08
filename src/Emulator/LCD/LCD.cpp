#include "LCD.hpp"

LCD::LCD()
{
}

void LCD::update(u8 t) 
{
    if (m_switchlcd) 
    {
        m_modeclock += t;

        switch (m_mode) {
            case Mode::LINE_SPRITES:
                if (m_modeclock >= 80) {
                    m_mode = Mode::LINE_BACKGROUND;
                    m_modeclock = 0;
                }
                break;
            case Mode::LINE_BACKGROUND:
                if (m_modeclock >= 172) {
                    m_mode = Mode::HBLANK;
                    m_modeclock = 0;
                    renderscan();
                }
                break;
            case Mode::HBLANK:
                if (m_modeclock >= 204) {
                    m_modeclock = 0;
                    m_line++;
                    if (m_line == 145) {
                        m_mode = Mode::VBLANK;
                    } 
                    else {
                        m_mode = Mode::LINE_SPRITES;
                    }

                }
                break;
            case Mode::VBLANK:
                if (m_modeclock >= 456) {
                    m_modeclock = 0;
                    m_line++;
                    if (m_line > 153) {
                        m_line = 0;
                        m_mode = Mode::LINE_SPRITES;
                        need_to_draw = true;
                    }
                }
                break;
        }
    }
}

#include <iostream>

void    *LCD::get_fb() {
    // DEBUG, RENDER TILEMAP ON SCREEN
    /*
    for (int i = 0; i < 512; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                std::cout << (unsigned short) m_tileset[i][j][k];
            }
        }
        std::cout << " ";
    }
    std::cout << "\n";
    */
    return (m_framebuffer);
}

u8 LCD::read_byte(u16 memory_loc) {
    if (memory_loc >= 0x8000 && memory_loc < 0xA000) {
        return m_video_ram[(memory_loc & 0x1FFF)];
    }
    else
    {
        switch (memory_loc)
        {
            case 0xFF40:
                return (m_switchbg  ? 0x01 : 0x00) |
                       (m_bgmap     ? 0x08 : 0x00) |
                       (m_bgtile    ? 0x10 : 0x00) |
                       (m_switchlcd ? 0x80 : 0x00);
            case 0xFF42:
                return m_scy;
            case 0xFF43:
                return m_scx;
            case 0xFF44:
                return static_cast<u8>(m_line);
            default:
                return 0;
        }
    }
}

u8 LCD::read_byte_at_oam(u8 memory_loc) {
    return m_oam[memory_loc];
}

void LCD::write_byte(u16 memory_loc, u8 value) {
    if (memory_loc < 0xA000) {
        m_video_ram[(memory_loc & 0x1FFF)] = value;
    }
    else
    {
        switch (memory_loc)
        {
            case 0xFF40:
                m_switchbg  = (value & 0x01) ? 1 : 0;
                m_bgmap     = (value & 0x08) ? 1 : 0;
                m_bgtile    = (value & 0x10) ? 1 : 0;
                m_switchlcd = (value & 0x80) ? 1 : 0;
                break;
            case 0xFF42:
                m_scy = value;
                break;
            case 0xFF43:
                m_scx = value;
                break;
            case 0xFF47:
                for (int i = 0; i < 4; i++)
                {
                    switch ((value >> (i * 2)) & 3)
                    {
                        case 0:
                            m_pal[i][0] = 255;
                            m_pal[i][1] = 255;
                            m_pal[i][2] = 255;
                            m_pal[i][3] = 255;
                            break;
                        case 1:
                            m_pal[i][0] = 192;
                            m_pal[i][1] = 192;
                            m_pal[i][2] = 192;
                            m_pal[i][3] = 255;
                            break;
                        case 2:
                            m_pal[i][0] = 96;
                            m_pal[i][1] = 96;
                            m_pal[i][2] = 96;
                            m_pal[i][3] = 255;
                            break;
                        case 3:
                            m_pal[i][0] = 0;
                            m_pal[i][1] = 0;
                            m_pal[i][2] = 0;
                            m_pal[i][3] = 255;
                            break;
                    }
                }
            break;
        }

    }
}

void LCD::write_byte_at_oam(u8 memory_loc, u8 value) {
    m_oam[memory_loc] = value;
}

void LCD::reset() {
    for (int i = 0; i < 512; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                m_tileset[i][j][k] = 0;
            }
        }
    }
}


void LCD::updatetile(u16 addr) {
    addr &= 0x17FF;

    int tile = (addr >> 4) & 511;
    int     y = (addr >> 1) & 7;

    for (int x = 0; x < 8; x++)
    {
        // get nth bit at addr and addr + 1 to determine the color in the palette
        unsigned char bitmask = 1 << (7 - x);
        m_tileset[tile][y][x] = static_cast<u8>(((m_video_ram[addr]     & bitmask) ? 1 : 0) +
                                                ((m_video_ram[addr + 1] & bitmask) ? 2 : 0));
    }

}

void LCD::renderscan()
{
    // select right background map
    int map_offset = m_bgmap ? 0x1C00 : 0x1800;

    // >> 3 -> divide by 8 to select right tile,
    // << 5 -> multiply by 32 to skip n tiles instead of n bytes
    map_offset += (((m_line + m_scy) & 255) >> 3) << 5;

    int line_offset = m_scx >> 3;
    u8            y = (m_line + m_scy) & 7;
    u8            x = m_scx & 7;

    int framebuffer_offset = (m_line - 1) * 160 * 4;

    unsigned short tile = m_video_ram[map_offset + line_offset];

    for (size_t i = 0; i < 160; i++)
    {
        // Re-map the tile pixel through the palette
        u8 colour[4];
        colour[0] = m_pal[m_tileset[tile][y][x]][0];
        colour[1] = m_pal[m_tileset[tile][y][x]][1];
        colour[2] = m_pal[m_tileset[tile][y][x]][2];
        colour[3] = m_pal[m_tileset[tile][y][x]][3];

        // Plot the pixel to canvas
        m_framebuffer[framebuffer_offset++] = colour[0];
        m_framebuffer[framebuffer_offset++] = colour[1];
        m_framebuffer[framebuffer_offset++] = colour[2];
        m_framebuffer[framebuffer_offset++] = colour[3];

        // Go to next tile
        x++;
        if (x == 8)
        {
            x = 0;
            line_offset = (line_offset + 1) & 31;
            tile = m_video_ram[map_offset + line_offset];
        }
    }
}

