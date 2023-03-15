#include <Emulator/Emulator.hpp>
#include <Emulator/Util/Types.hpp>

#include <SDL.h>

#include <iostream>
#include <thread>

SDL_Renderer    *m_renderer;
SDL_Window      *m_window;
SDL_Texture     *m_framebuffer;

int init_window()
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

    m_framebuffer = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, 160, 144);
    if (!m_framebuffer)
    {
        printf("Failed to create texture: %s\n", SDL_GetError());
        exit(1);
    }
    return (0);
}

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        std::cerr << "Please specify a rom file as an argument" << std::endl;
	    return 1;
    } 
    Emulator emu(argv[1]);
    std::thread emu_thread(&Emulator::run, &emu);

    init_window();

    while (1)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                emu.get_CPU().m_exit = true;
                goto out ;
            }
            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) emu.get_joypad().update(e);
        }
        if (emu.get_lcd().need_to_draw)
        {

            // TODO: Investigate: Maybe protect from reading a frame buffer that is being modified ?
            SDL_UpdateTexture(m_framebuffer, NULL, emu.get_lcd().get_fb(), 160 * sizeof(u32));
            SDL_RenderClear(m_renderer);
            SDL_RenderCopy(m_renderer, m_framebuffer, NULL, NULL);
            SDL_RenderPresent(m_renderer);

            emu.get_lcd().need_to_draw = false;
        }
    }

out:
    emu_thread.join();
    SDL_DestroyTexture(m_framebuffer);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    return 0;
}
