#include "LCD.hpp"
#include <Emulator/Emulator.hpp>

LCD::LCD(Emulator *emu)
	: m_emu(emu)
{
}

void LCD::update(u8 t) 
{
    if (m_lcdon) 
    {
		if (m_dma_cycles_left)
		{
			u16 src = (m_dma << 8) + m_dma_offset;
			u8 	nbytes = (t / 4);

			//std::cout << "GBMU: LCD: DOING DMA TRANSFER TO OAM; bytes transfered: " << std::hex << +m_dma_offset << ", bytes soon to be transfered on this iter: " << +nbytes << ", at address: " << src << std::endl; 
			for (u8 c = 0; c < nbytes && m_dma_cycles_left; c++, m_dma_cycles_left -= 4)
				write_byte_at_oam(m_dma_offset++, m_emu->get_MMU().get_byte_at(src + c));
		}

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
                    if (m_line == 143) {
		                need_to_draw = true;
                        m_mode = Mode::VBLANK;
						m_emu->get_CPU().request_interrupt(CPU::Interrupt::VBLANK);
                    } 
                    else {
						// Here to request the stat interrupt
						if (m_stat_int_sources & 0x80 && m_line == m_lyc)
							m_emu->get_CPU().request_interrupt(CPU::Interrupt::STAT);
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
						// Here to request the stat interrupt
						if (m_stat_int_sources & 0x80 && m_line == m_lyc)
							m_emu->get_CPU().request_interrupt(CPU::Interrupt::STAT);
                        m_mode = Mode::LINE_SPRITES;
                    }
                }
                break;
        }

		/* STAT interrupt for OAM, VBLANK AND HBLANK */
		if (m_mode != LINE_BACKGROUND && (static_cast<u8>(1 << (3 + m_mode)) & m_stat_int_sources))
			m_emu->get_CPU().request_interrupt(CPU::Interrupt::STAT);
    }
}

#include <iostream>

void    *LCD::get_fb() {
    // DEBUG, RENDER TILEMAP ON SCREEN
    /*for (int i = 0; i < 384; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                std::cout << (unsigned short) m_tileset[i][j][k];
            }
        }
        std::cout << " ";
    }
    std::cout << "\n\n\n";*/
    return (m_framebuffer);
}

u8 LCD::read_byte(u16 memory_loc) {
    if (memory_loc >= 0x8000 && memory_loc < 0xA000) {
        return m_video_ram[(memory_loc & 0x1FFF)];
    }
    else
    {
		u8 d;
        switch (memory_loc)
        {
            case 0xFF40:
                return (m_bgwinon	? 0x01 : 0x00) |
					   (m_spriteon	? 0x02 : 0x00) |
					   (m_spritesz	? 0x04 : 0x00) |
                       (m_bgmap     ? 0x08 : 0x00) |
                       (m_bgwintile	? 0x10 : 0x00) |
					   (m_windowon	? 0x20 : 0x00) |
					   (m_windowmap	? 0x40 : 0x00) |
                       (m_lcdon		? 0x80 : 0x00);
			case 0xFF41:
				d = 	((m_mode)		   			<< 0) |
						((m_lyc == m_line) 		<< 2) |
						((m_stat_int_sources) 	<< 3);
				std::cout << "GBMU: LCD: READ FROM FF41 WITH RETURNED VALUE " << std::hex << +d << std::endl;
				return (d);
            case 0xFF42:
                return m_scy;
            case 0xFF43:
                return m_scx;
            case 0xFF44:
                return static_cast<u8>(m_line);
			case 0xFF45:
				std::cout << "GBMU: LCD: READ FROM LYC " << std::hex << +m_lyc << std::endl;
				return m_lyc;
			case 0xFF46:
				return m_dma;
			case 0xFF4A:
				return m_wy;
			case 0xFF4B:
				return m_wx;
            default:
                return 0;
        }
    }
}

u8 LCD::read_byte_at_oam(u8 memory_loc) {
    Sprite &s = m_sprites[memory_loc >> 2];

	//std::cout << "GBMU: LCD: READING SPRITE " << +(memory_loc >> 2) << " in OAM" << std::endl;
	switch (memory_loc & 2)
	{
		case 0:
			return (s.y);
		case 1:
			return (s.x);
		case 2:
			return (s.tile_index);
		case 3:
			return	(s.background	<< 7) |
					(s.y_flip		<< 6) |
					(s.x_flip		<< 5) |
					(s.palette		<< 4);
		// Not possibly reachable
		default:
			return (1);
	}
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
                m_bgwinon 	= (value)		& 1;
				m_spriteon	= (value >> 1)	& 1;
				m_spritesz	= (value >> 2)	& 1;
                m_bgmap     = (value >> 3)	& 1;
                m_bgwintile = (value >> 4)	& 1;
				m_windowon	= (value >> 5)	& 1;
				m_windowmap = (value >> 6)	& 1;
                m_lcdon		= (value >> 7)	& 1;
                break;
			case 0xFF41:
				std::cout << "Write to ff41 with value " << std::hex << +value << std::endl;
				m_stat_int_sources = (value >> 3) & 0xF;
				break;
            case 0xFF42:
                m_scy = value;
                break;
            case 0xFF43:
                m_scx = value;
                break;
			case 0xFF45:
				std::cout << "GBMU: LCD: WRITE TO LYC " << std::hex << +m_lyc << std::endl;
				m_lyc = value;
				break ;
			case 0xFF46:
				m_dma = value;
				init_dma_transfer();
				break ;
            case 0xFF47:
                for (int i = 0; i < 4; i++)
                {
                    switch ((value >> (i * 2)) & 3)
                    {
                        case 0:
                            m_pal[i] = 0xffe7cdff;
                            break;
                        case 1:
                            m_pal[i] = 0xe4a39fff;
                            break;
                        case 2:
                            m_pal[i] = 0x629098ff;
                            break;
                        case 3:
                            m_pal[i] = 0x4c3457ff;
                            break;
                    }
                }
				break;
			case 0xFF48:
                for (int i = 1; i < 4; i++)
                {
                    switch ((value >> (i * 2)) & 3)
                    {
                        case 0:
                            m_obj_pal0[i] = 0xffe7cdff;
                            break;
                        case 1:
                            m_obj_pal0[i] = 0xe4a39fff;
                            break;
                        case 2:
                            m_obj_pal0[i] = 0x629098ff;
                            break;
                        case 3:
                            m_obj_pal0[i] = 0x4c3457ff;
                            break;
                    }
                }
				break;
			case 0xFF49:
			    for (int i = 1; i < 4; i++)
                {
                    switch ((value >> (i * 2)) & 3)
                    {
                        case 0:
                            m_obj_pal1[i] = 0xffe7cdff;
                            break;
                        case 1:
                            m_obj_pal1[i] = 0xe4a39fff;
                            break;
                        case 2:
                            m_obj_pal1[i] = 0x629098ff;
                            break;
                        case 3:
                            m_obj_pal1[i] = 0x4c3457ff;
                            break;
                    }
                }
				break;
			case 0xFF4A:
				m_wy = value;
				break ;
			case 0xFF4B:
				m_wx = value;
				break ;
        }

    }
}

void LCD::write_byte_at_oam(u8 memory_loc, u8 value) {
	Sprite &s = m_sprites[memory_loc >> 2];

	//std::cout << "GBMU: LCD: WRITING SPRITE " << std::dec << +(memory_loc >> 2) << " in OAM" << std::endl;
	switch (memory_loc & 2)
	{
		case 0:
			s.y = value;
			break;
		case 1:
			s.x = value;
			break;
		case 2:
			s.tile_index = value;
			break;
		case 3:
			s.background =	(value >> 7) & 1;
			s.y_flip =		(value >> 6) & 1;
			s.x_flip =		(value >> 5) & 1;
			s.palette =		(value >> 4) & 1;
			break;
	}
}

void LCD::reset() {
    for (int i = 0; i < 384; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                m_tileset[i][j][k] = 0;
            }
        }
    }
}


void LCD::updatetile(u16 addr) {
    addr &= 0x1FFE;

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
	// TODO: Draw sprites with the m_sprites array

    // select right background map
    int map_offset = m_bgmap ? 0x1C00 : 0x1800;

    // >> 3 -> divide by 8 to select right tile,
    // << 5 -> multiply by 32 to skip n tiles instead of n bytes
    map_offset += (((m_line + m_scy) & 255) >> 3) << 5;

    int line_offset = m_scx >> 3;
    u8            y = (m_line + m_scy) & 7;
    u8            x = m_scx & 7;

    int framebuffer_offset = (m_line - 1) * 160;

    unsigned short tile = m_video_ram[map_offset + line_offset];

    for (size_t i = 0; i < 160; i++)
    {
        // Re-map the tile pixel through the palette and put it to screen
        m_framebuffer[framebuffer_offset++] = m_pal[m_tileset[tile][y][x]];

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

