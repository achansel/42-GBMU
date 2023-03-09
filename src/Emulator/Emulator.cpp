#include "Emulator.hpp"
#include "LCD/LCD.hpp"


Emulator::Emulator(const std::string& path_to_game):
    m_cartridge(path_to_game),
    m_mmu(this),
    m_cpu(this),
    m_joypad(),
    m_lcd()
{
    int rendererFlags;
	rendererFlags = SDL_RENDERER_ACCELERATED;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	m_window = SDL_CreateWindow("GBMU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 160 * 3, 144 * 3, 0);
	if (!m_window)
	{
		printf("Failed to open %d x %d window: %s\n", 160*3, 144*3, SDL_GetError());
		exit(1);
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, rendererFlags);
	if (!m_renderer)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
	}

    m_framebuffer = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, 160, 144);
    if (!m_framebuffer)
    {
        printf("Failed to create texture: %s\n", SDL_GetError());
        exit(1);

    }
}

Emulator::~Emulator()
{
	std::cout << "~Emulator(void) was called\n\n";
    SDL_DestroyTexture(m_framebuffer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Emulator::run() {
    m_lcd.reset();
    while (!m_cpu.m_exit)
    {
        m_cpu.tick();
        draw_frame();
    }
    //m_mmu.dumpmem();
}

void Emulator::draw_frame()
{
    if (m_lcd.need_to_draw)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) return ;
            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) m_joypad.update(e);
        }

        SDL_UpdateTexture(m_framebuffer, NULL, m_lcd.get_fb(), 160 * sizeof(uint32_t));

        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, m_framebuffer, NULL, NULL);
        SDL_RenderPresent(m_renderer);

        m_lcd.need_to_draw = false;
        m_cpu.tclock = 0;
    }
}

void Emulator::reset() {
}

MMU &Emulator::get_MMU() {
    return m_mmu;
}

CPU &Emulator::get_CPU() {
    return m_cpu;
}

LCD &Emulator::get_lcd() {
    return m_lcd;
}

Cartridge &Emulator::get_cartridge() {
    return m_cartridge;
}

Joypad &Emulator::get_joypad()
{
    return m_joypad;
}
