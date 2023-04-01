#pragma once

#include <array>
#include <iostream>

#include <Emulator/Util/Types.hpp>
#include <Emulator/Memory/IMMIO.hpp>

class Emulator;

enum Mode
{
    HBLANK = 0, VBLANK = 1, LINE_SPRITES = 2, LINE_BACKGROUND = 3
};

class LCD : public IMMIO {
public:
    LCD(Emulator *emu);

    void update(u8 t);
    void *get_fb();

    u8 read_byte(u16 memory_loc);
    u8 read_byte_at_oam(u16 memory_loc);

    void write_byte(u16 memory_loc, u8 value);
    void write_byte_at_oam(u16 memory_loc, u8 value);

    void reset();
    void updatetile(u16 addr);

    bool need_to_draw = false;
private:
	Emulator *m_emu;

	void request_interrupts(bool newline);
    void renderscan();

	/* TODO: Implement blocking of whole address space but HRAM during DMA Transfer */
	void perform_dma(u8 tclock);
	void init_dma_transfer();

	std::array<u8, 8192> m_video_ram{0};

	struct Sprite {
		Sprite() = default;

		u8		x, y;
		u8		tile_index;

		bool	background;
		bool	x_flip, y_flip;
		u8		palette;
	};
	std::array<Sprite, 40>	m_sprites;

    u32 m_framebuffer[144*160];

	int m_modeclock = 0;
    int m_line = 1;
    Mode m_mode = Mode::LINE_BACKGROUND;

    bool m_bgmap, m_bgwintile, m_bgwinon, m_lcdon;
	bool m_spriteon, m_spritesz;
	bool m_windowon, m_windowmap;

	u8		m_wx, m_wy;
	u8		m_window_line;
    u8		m_scx, m_scy;
	bool	m_should_display_window;

	u8 m_lyc;
	u8 m_stat_int_sources;

	u8	m_dma;
	u16 m_dma_cycles_left = 0;
	u8	m_dma_offset;


	// https://lospec.com/palette-list
	// good pals
	
	//TODO: IMRPOVE PALETTE HANDLING
	u8 m_raw_pal[3];
    u32 m_pal[4];
	u32 m_obj_pal0[4];
	u32 m_obj_pal1[4];

    u8 m_tileset[384][8][8];
};
