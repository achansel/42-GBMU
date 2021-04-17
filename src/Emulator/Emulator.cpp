#include "Emulator.hpp"
#include "LCD/LCD.hpp"


Emulator::Emulator(const std::string& path_to_game):
    m_cartridge(path_to_game),
    m_mmu(this),
    m_cpu(this),
    m_joypad(),
    render_window(sf::VideoMode(480, 432), "Gameboy Emulator"),
    m_lcd()
{
}

void Emulator::run() {
    m_mmu.set_memory_rule(m_cartridge.get_cart_type());
    while (render_window.isOpen() && !m_cpu.m_exit)
    {
        sf::Event e;
        while (render_window.pollEvent(e))
        {
            if (e.type == sf::Event::EventType::Closed) render_window.close();
            m_joypad.update(e);
        }
        m_cpu.tick();
        draw_frame();
        render_window.clear(sf::Color::Magenta);
    }
    //m_mmu.dumpmem();
}

void Emulator::draw_frame()
{
    if (m_lcd.need_to_draw)
    {
        sf::Image image = m_lcd.draw();
        sf::Texture tex;
        tex.loadFromImage(image, sf::IntRect(0, 0, 160, 144));
        sf::Sprite sprite;
        sprite.setTexture(tex);
        sprite.setScale(sf::Vector2f(3.0f, 3.0f));

        render_window.draw(sprite);
        render_window.display();

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
