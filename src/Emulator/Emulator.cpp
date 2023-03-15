#include "Emulator.hpp"
#include "LCD/LCD.hpp"


Emulator::Emulator(const std::string& path_to_game):
    m_cartridge(path_to_game),
    m_mmu(this),
    m_cpu(this),
    m_joypad(this),
    m_lcd(this),
	m_timer(this)
{
}

Emulator::~Emulator()
{
	std::cout << "~Emulator(void) was called\n\n";
}

void Emulator::run() {
    m_lcd.reset();
    while (!m_cpu.m_exit)
        m_cpu.tick();
    //m_mmu.dumpmem();
}

void Emulator::draw_frame()
{

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

Timer &Emulator::get_timer() {
    return m_timer;
}

Cartridge &Emulator::get_cartridge() {
    return m_cartridge;
}

Joypad &Emulator::get_joypad()
{
    return m_joypad;
}
