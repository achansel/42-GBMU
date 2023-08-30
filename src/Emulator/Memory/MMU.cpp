#include "MMU.hpp"
#include "../Emulator.hpp"

MMU::MMU(Emulator* emu)
	: m_emu(emu), m_bios_mapped(true), m_memory_map()
{
    register_range(0x0000, 0x100, this, &MMU::read_bootrom, &MMU::write_bootrom); 
    register_range(0x0100, 0x7F00, &m_emu->get_cartridge());
    register_range(0x8000, 0x2000, &m_emu->get_lcd());
    register_range(0xA000, 0x2000, &m_emu->get_cartridge(), &Cartridge::read_byte_at_ext_ram, &Cartridge::write_byte_at_ext_ram);
    register_range(0xC000, 0x3DDF, &m_emu->get_CPU());
    register_range(0xFE00, 0xA0, &m_emu->get_lcd(), &LCD::read_byte_at_oam, &LCD::write_byte_at_oam);
    register_range(0xFF00, 0x80, this, &MMU::read_ioreg, &MMU::write_ioreg);
    register_range(0xFF80, 0x80, this, &MMU::read_byte_at_hram, &MMU::write_byte_at_hram);
    register_range(0xFFFF, 1, &m_emu->get_CPU(), &CPU::read_ie, &CPU::write_ie);
}

u8 MMU::read_bootrom(u16 address)
{
    return dmg_bios[address];
}
void MMU::write_bootrom(u16 address, u8 value)
{
    (void) address;
    (void) value;
}

u8 MMU::read_ioreg(u16 address)
{
    if (address == 0xFF00)
        return m_emu->get_joypad().read_byte();
    else if (address >= 0xFF04 && address <= 0xFF07)
        return m_emu->get_timer().read_byte(address);
    else if (address == 0xFF0F)
        return m_emu->get_CPU().read_if();
    else if (address >= 0xFF40 && address < 0xFF50)
        return m_emu->get_lcd().read_byte(address);
    else if (!(address >= 0xFF10 && address < 0xFF40))
    {
        //std::cout << "GBMU: MMU: READ TO UNKNOWN/UNIMPLMENTED IO REGISTER " << std::hex << memory_location << std::endl;
    }
    return (0xFF);
}
void MMU::write_ioreg(u16 address, u8 value)
{
    if (address == 0xFF00)
        m_emu->get_joypad().write_byte(value);
    if (address == 0xFF01)
        std::cout << value;
    else if (address >= 0xFF04 && address <= 0xFF07)
        m_emu->get_timer().write_byte(address, value);
    else if (address == 0xFF0F)
        m_emu->get_CPU().write_if(value);
    else if (address >= 0xFF40 && address < 0xFF50)
        m_emu->get_lcd().write_byte(address, value);
    else if (address == 0xFF50 && m_bios_mapped)
        unmap_bootrom();
    else if (!(address >= 0xFF10 && address < 0xFF40))
    {
        //std::cout << "GBMU: MMU: WRITE TO UNKNOWN/UNIMPLMENTED IO REGISTER " << std::hex << memory_location << ", VALUE: " << +value << std::endl;
    }

}

u8 MMU::read_byte_at_hram(u16 address)
{
    return (m_hram[address & 0x7F]);
}
void MMU::write_byte_at_hram(u16 address, u8 value)
{
    m_hram[address & 0x7F] = value;
}

bool    MMU::unmap_range(u16 low, u16 size)
{
    auto it = std::find_if(m_memory_map.begin(), m_memory_map.end(), [&](const auto &e){ return (e.m_begin == low && e.m_size == size); });
    if (it != m_memory_map.end())
        m_memory_map.erase(it);
    else
        return (false);
    return (true);
}

void    MMU::unmap_bootrom()
{
    if (unmap_range(0x0000, 0x0100) & unmap_range(0x0100, 0x7F00))
        register_range(0x0000, 0x8000, &m_emu->get_cartridge());
}


u8 MMU::get_byte_at(u16 memory_location)
{
    for (auto &r : m_memory_map)
    {
        if (r.is_within(memory_location))
            return (r.m_read_func(memory_location));
    }
    return (0xFF);
}

s8 MMU::get_signed_byte_at(u16 memory_location) {
    return static_cast<s8>(get_byte_at(memory_location));
}

u16 MMU::get_word_at(u16 memory_location)
{
    return Bitwise::compose_word(get_byte_at(memory_location+1), get_byte_at(memory_location));
}

void MMU::set_byte_at(u16 memory_location, u8 value)
{
    for (auto &r : m_memory_map)
    {
        if (r.is_within(memory_location))
            r.m_write_func(memory_location, value);
    }
}

void MMU::set_word_at(u16 memory_location, u16 value)
{
    set_byte_at(memory_location, static_cast<u8>(value));
    set_byte_at(memory_location+1, static_cast<u8>(value >> 8));
}
