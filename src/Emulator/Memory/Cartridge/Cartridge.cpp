/*
 * TODO: Add the memory banking system
 */

#include <iostream>
#include "Cartridge.hpp"

Cartridge::Cartridge(const std::string& path_to_game) {
    std::ifstream ifs(path_to_game, std::ios::binary | std::ios::ate);
    if (ifs.fail())
    {
        std::cerr << "Error opening file '" << path_to_game << "'" << std::endl;
        exit(1);
    }
    std::ifstream::pos_type rom_size = ifs.tellg();

    m_rom = (u8*) malloc(rom_size);

    ifs.seekg(0, std::ios::beg);
    ifs.read((char*) m_rom, rom_size);
}

Cartridge::~Cartridge()
{
    free(m_rom);
}

int Cartridge::get_cart_type()
{
    return m_rom[0x147];
}

u8 Cartridge::read_byte(u16 memory_loc) {
    return memory_loc >= rom_size ? 0: m_rom[memory_loc];
}

u8 Cartridge::read_byte_at_ext_ram(u16 memory_loc) {
    return m_external_ram[memory_loc];
}

void Cartridge::write_byte(u16 memory_loc, u8 value) {
    std::cout << "Memory Location: " << std::hex << memory_loc << " Value:" << +value << std::endl;
    m_rom[memory_loc] = value;
}

void Cartridge::write_byte_at_ext_ram(u16 memory_loc, u8 value) {
    m_external_ram[memory_loc] = value;
}
