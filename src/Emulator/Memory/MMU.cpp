#include "MMU.hpp"
#include "../Emulator.hpp"

#include "Emulator/Memory/MemoryController/ROMOnly.hpp"

MMU::MMU(Emulator* emu)
{ 
    m_emu = emu;
}

void MMU::set_memory_rule(int memrule)
{
    if (static_cast<MemoryRule>(memrule) == MemoryRule::ROMONLY)
    {
        mem_controller = std::make_unique<ROMOnly>(m_emu);
    }
}

u8 MMU::get_byte_at(u16 memory_location)
{
    //if (memory_location >= 0x100 && memory_location < 0xff00)
    //    std::cout << "Requested byte read at: " << std::hex << memory_location << std::endl;
    if (memory_location < 0x100)
    {
        return dmg_bios[memory_location];
    }
    u8 byte = mem_controller->get_byte_at(memory_location);
    //if (memory_location >= 0x100 && memory_location < 0xff00)
    //    std::cout << "And the byte is : " << std::hex << (unsigned int) byte << std::endl;
    return (byte);
}

s8 MMU::get_signed_byte_at(u16 memory_location) {
    return static_cast<s8>(get_byte_at(memory_location));
}

u16 MMU::get_word_at(u16 memory_location)
{
    return Bitwise::compose_bytes(get_byte_at(memory_location+1), get_byte_at(memory_location));
}

void MMU::set_byte_at(u16 memory_location, u8 value) {
    mem_controller->set_byte_at(memory_location, value);
}

void MMU::set_word_at(u16 memory_location, u16 value)
{
    set_byte_at(memory_location, static_cast<u8>(value));
    set_byte_at(memory_location+1, static_cast<u8>(value >> 8));
}
