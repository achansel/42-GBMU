#include "LCD.hpp"
#include <Emulator/Emulator.hpp>

#include <algorithm>

LCD::LCD(Emulator *emu)
	: m_emu(emu),
	m_windowon(false),
	m_window_line(0),
	m_should_display_window(false)
{
}

void LCD::perform_dma(u8 t)
{
    u16 src = (m_dma << 8) + m_dma_offset;
    u8 	nbytes = (t / 4);

    for (u8 c = 0; c < nbytes && m_dma_cycles_left; c++, m_dma_cycles_left -= 4)
        write_byte_at_oam(m_dma_offset++, m_emu->get_MMU().get_byte_at(src + c));
}

// TODO: Fix interrupts and maybe timings so that TLOZ Links Awakening works
void LCD::request_interrupts()
{
    if (m_mode == Mode::VBLANK)
		m_emu->get_CPU().request_interrupt(CPU::Interrupt::VBLANK);
	if ((m_stat_int_sources & 0x8) && m_line == m_lyc)
		m_emu->get_CPU().request_interrupt(CPU::Interrupt::STAT);
	if (m_mode != LINE_BACKGROUND
		&& (static_cast<u8>(1 << m_mode) & m_stat_int_sources))
		m_emu->get_CPU().request_interrupt(CPU::Interrupt::STAT);
}

void LCD::update(u8 t) 
{
    if (m_lcdon) 
    {
		if (m_dma_cycles_left)
            perform_dma(t);

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
					request_interrupts();
                }
                break;
            case Mode::HBLANK:
                if (m_modeclock >= 204) {
                    m_modeclock = 0;
                    m_line++;
                    if (m_line == 144) {
		                need_to_draw = true;
                        m_mode = Mode::VBLANK;
						request_interrupts();
                    } 
                    else {
                        m_mode = Mode::LINE_SPRITES;
                        request_interrupts();
						if (m_windowon && m_wy == m_line)
							m_should_display_window = true;
                    }

                }
                break;
            case Mode::VBLANK:
                if (m_modeclock >= 456) {
                    m_modeclock = 0;
                    m_line++;
                    if (m_line > 153) {
						m_window_line = 0;
                        m_line = 0;
                        m_mode = Mode::LINE_SPRITES;
                        request_interrupts();
						m_should_display_window = m_windowon && (m_wy == m_line);
                    }
                }
                break;
        }
    }
}

void    *LCD::get_fb() {
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
				d = 	((m_mode)		   		<< 0) |
						((m_lyc == m_line) 		<< 2) |
						((m_stat_int_sources) 	<< 3);
				return (d);
            case 0xFF42:
                return m_scy;
            case 0xFF43:
                return m_scx;
            case 0xFF44:
                return static_cast<u8>(m_line);
			case 0xFF45:
				return m_lyc;
			case 0xFF46:
				return m_dma;
			case 0xFF47:
			case 0xFF48:
			case 0xFF49:
				return (m_raw_pal[memory_loc - 0xFF47]);
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

	switch (memory_loc & 3)
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
				m_stat_int_sources = (value >> 3) & 0xF;
				break;
            case 0xFF42:
                m_scy = value;
                break;
            case 0xFF43:
                m_scx = value;
                break;
			case 0xFF45:
				m_lyc = value;
				break ;
			case 0xFF46:
				m_dma = value;
				init_dma_transfer();
				break ;
            case 0xFF47:
				m_raw_pal[0] = value;
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
				m_raw_pal[1] = value;
                for (int i = 0; i < 4; i++)
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
				m_raw_pal[2] = value;
			    for (int i = 0; i < 4; i++)
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

	switch (memory_loc & 3)
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

    int	tile    = (addr >> 4) & 0x1FF;
    int		y   = (addr >> 1) & 7;


    for (int x = 0; x < 8; x++)
    {
        unsigned char bitmask = 1 << (7 - x);
        m_tileset[tile][y][x] = static_cast<u8>(((m_video_ram[addr]     & bitmask) ? 1 : 0) +
                                                ((m_video_ram[addr + 1] & bitmask) ? 2 : 0));
    }
}

void LCD::renderscan()
{
	//std::cout << "GBMU: DEBUG: " << "window_on " << m_windowon << ", wx: " << std::hex << +m_wx << ", wy: " << +m_wy <<std::endl;

	// TODO: REFACTOR AND FIX SPRITES
	// Select sprites
	std::vector<std::reference_wrapper<Sprite>>				selection;
	size_t													selection_idx;

	if (m_spriteon)
	{
		selection_idx = 0;
		selection.reserve(10);
		
		// Fetch sprites that need to be drawn
		for (Sprite &s : m_sprites)
		{
			// Sprite could be on line
			if (m_line < s.y && m_line >= s.y - 16)
			{
				// Is sprite on this scanline according to its size
				if (m_spritesz || (m_line < s.y - 8))
				{
					selection.push_back(s);
					if (selection.size() == 10)
						break ;
				}
			}
		}
		std::stable_sort(selection.begin(), selection.end(), [](const std::reference_wrapper<Sprite> &a, const std::reference_wrapper<Sprite> &b) { return (a.get().x < b.get().x); });
	}


    /* Select right tile map for the tile we are about to draw
		if there is a window where we draw */
    int map_offset		= m_bgmap ? 0x1C00 : 0x1800;

    map_offset += (((m_line + m_scy) & 255) >> 3) << 5;
	if (m_should_display_window && m_wx == 0 + 7)
		map_offset = (m_windowmap ? 0x1C00 : 0x1800) + ((m_window_line >> 3) << 5);

	/* The code that picks the right tile to draw, 
		if we are drawing background, select tile according to position on screen and scroll regs
		otherwise simply select the tile by the position relative to the window */
    int line_offset = m_scx >> 3;
    u8            y = (m_line + m_scy) & 7;
    u8            x = m_scx & 7;

	if (m_should_display_window && m_wx == 0 + 7)
	{
		line_offset = 0;
		y = m_window_line & 7;
		x = 0;
	}

    u32 *framebuffer_ptr = m_framebuffer + (m_line * 160);

	/*	Then finally get the right tile index from the tile map
		Here, the io register bgwintile tells us which tile set to use. */
    u16 tile = m_video_ram[map_offset + line_offset];
    if (!m_bgwintile)
		tile = static_cast<u16>(256 + static_cast<s8>(tile & 0xFF));

	/* For each pixel of the scanline */
    for (size_t i = 0; i < 160; i++)
    {
        // Re-map the tile pixel through the palette and put it to screen
        *framebuffer_ptr = m_pal[m_tileset[tile][y][x]];

		// Sprite appears here
		bool already_drawn = false;
		for (auto s = selection.begin() + selection_idx ; !selection.empty() && s != selection.end() && s->get().x - 8 <= (int)(i) && s->get().x > (int)(i) ; ++s)
		{
			// Get tile and x and y coordinate inside of tile
			u8	tile_x		= i			- (s->get().x - 8);
			u8	tile_y		= m_line	- (s->get().y - 16);

			// Flipping (maybe does not work on y axis)
			if (s->get().x_flip)
				tile_x = 7 - tile_x;
			if (s->get().y_flip)
				tile_y = (m_spritesz ? 15 : 7) - tile_y;
			u8 sprite_tile = s->get().tile_index;

			// 2 tiles tall sprites
			if (m_spritesz)
			{
				sprite_tile = (sprite_tile & 0xFE) + (tile_y >= 8);
				tile_y %= 8;
			}

			// Check if we have a visible pixel
			u8 col = m_tileset[sprite_tile][tile_y][tile_x];
			if (col && !already_drawn && (!s->get().background || (s->get().background && !m_tileset[tile][y][x])))
			{
				// Get color mapped through palette and show it
				if (s->get().palette)
					*framebuffer_ptr = m_obj_pal1[col];
				else
					*framebuffer_ptr = m_obj_pal0[col];
				already_drawn = true;
			}

			// Go to next sprite if this x marks the end of it
			if (i + 1 == s->get().x)
				selection_idx++;
		}
		framebuffer_ptr++;

        // Go to next tile
        x++;
        if (x == 8)
        {
            x = 0;
            line_offset = (line_offset + 1) & 31;
            tile = m_video_ram[map_offset + line_offset];
			//TODO: WINDOW ENABLING MID FRAME when m_wx > 7

		    if (!m_bgwintile)
				tile = static_cast<u16>(256 + static_cast<s8>(tile & 0xFF));
        }
    }
	if (m_should_display_window)
		m_window_line++;
}

