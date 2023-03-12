/*
 * TODO: Add the memory banking system
 */

#include <iostream>
#include "Cartridge.hpp"

// TODO: Fix folder opening
Cartridge::Cartridge(const std::string& path_to_game) {
    std::ifstream ifs(path_to_game, std::ios::binary | std::ios::ate);
    if (ifs.fail() || !ifs.good())
    {
        std::cerr << "Error opening file '" << path_to_game << "'" << std::endl;
        exit(1);
    }

	/* File loading stuff */
    m_rom_size = ifs.tellg();

    m_rom = (u8*) malloc(m_rom_size);
    ifs.seekg(0, std::ios::beg);
    ifs.read((char*) m_rom, m_rom_size);

	/* Cartridge headers reading */
	m_title = std::string(m_rom + 0x134, m_rom + 0x143);
	m_manufacturer_code = std::string(m_rom + 0x13F, m_rom + 0x142);
	m_cgb_flag = m_rom[0x143];
	m_cartridge_type = static_cast<CartridgeType>(m_rom[0x147]);
	m_destination_code = m_rom[0x14A];
	m_mask_rom_version_number = m_rom[0x14C];
	//TODO: m_ram_size = m_rom[149];

	std::cout << "GBMU: ROM LOADED:\n";
	std::cout << "\ttitle: " << m_title << " (manufacturer_code: " << (m_manufacturer_code[0] != '\0' ? m_manufacturer_code : "NONE") << ") version " << std::hex << static_cast<unsigned int>(m_mask_rom_version_number) << "\n";
	std::cout << "\ttype: " << std::hex << static_cast<unsigned int>(m_cartridge_type) << " \tcgb_flag: " << static_cast<unsigned int>(m_cgb_flag) << "\n";
	std::cout << "\tdestination: " << std::hex << static_cast<unsigned int>(m_destination_code) << "\n";
	if (!Cartridge::is_supported_mbc(m_cartridge_type))
	{
		std::cout << "GBMU: FATAL: UNSUPPORTED CART TYPE 0X" << std::hex << static_cast<unsigned int>(m_cartridge_type) << "\n";
		exit(1);
	}
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
    //std::cout << "Cartridge::read_byte -> U16: " << std::hex << memory_loc << std::endl;
    return memory_loc >= m_rom_size ? 0 : m_rom[memory_loc];
}

u8 Cartridge::read_byte_at_ext_ram(u16 memory_loc)
{
	if (m_cartridge_type != CartridgeType::ROM_ONLY)
		return m_ram[memory_loc];
	return (0xFF);
}

void Cartridge::write_byte(u16 memory_loc, u8 value) {
    std::cout << "\nGBMU Cartridge write: memory Location: " << std::hex << memory_loc << " value:" << +value << std::endl;
    //m_rom[memory_loc] = value;
}

void Cartridge::write_byte_at_ext_ram(u16 memory_loc, u8 value) 
{
	if (m_cartridge_type != CartridgeType::ROM_ONLY)
    	m_ram[memory_loc] = value;
}

/* Statics */
bool Cartridge::is_supported_mbc(CartridgeType type)
{
	return (type == CartridgeType::ROM_ONLY
		||	type == CartridgeType::MBC1);
}
