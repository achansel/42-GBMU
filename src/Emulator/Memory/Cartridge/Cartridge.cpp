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
    u32	rom_size = ifs.tellg();
    u8	*rom = (u8*) malloc(rom_size);

    ifs.seekg(0, std::ios::beg);
    ifs.read((char*) rom, rom_size);

	/* Cartridge headers reading */
	m_title = std::string(rom + 0x134, rom + 0x143);
	m_manufacturer_code = std::string(rom + 0x13F, rom + 0x142);
	m_cgb_flag = rom[0x143];
	CartridgeType cartridge_type = static_cast<CartridgeType>(rom[0x147]);
	m_destination_code = rom[0x14A];
	m_mask_rom_version_number = rom[0x14C];
	
	u32 sz[] = {0, 0, 8*1024, 32*1024, 128*1024, 64*1024};
	//std::cout << std::dec << sz[rom[0x149]] << ":" << static_cast<unsigned int>(rom[0x149]) << std::endl;
	//exit(1);
	u32 ram_size = sz[rom[0x149]];

	std::cout << "GBMU: ROM LOADED:\n";
	std::cout << "\ttitle: " << m_title << " (manufacturer_code: " << (m_manufacturer_code[0] != '\0' ? m_manufacturer_code : "NONE") << ") version " << std::hex << static_cast<unsigned int>(m_mask_rom_version_number) << "\n";
	std::cout << "\ttype: " << std::hex << static_cast<unsigned int>(cartridge_type) << " \tcgb_flag: " << static_cast<unsigned int>(m_cgb_flag) << "\n";
	std::cout << "\tdestination: " << std::hex << static_cast<unsigned int>(m_destination_code) << "\n";
	if (!Cartridge::is_supported_mbc(cartridge_type))
	{
		std::cout << "GBMU: FATAL: UNSUPPORTED CART TYPE 0X" << std::hex << static_cast<unsigned int>(cartridge_type) << "\n";
		exit(1);
	}

	if (cartridge_type == CartridgeType::ROM_ONLY)
		m_mapper = AMapper::make<ROMOnly>(path_to_game, rom, rom_size, ram_size);
	else if (cartridge_type >= CartridgeType::MBC5_RAM && cartridge_type <= CartridgeType::MBC5_RUMBLE_RAM_BATTERY)
		m_mapper = AMapper::make<MBC5>(path_to_game, rom, rom_size, ram_size, cartridge_type == CartridgeType::MBC5_RAM_BATTERY || cartridge_type == CartridgeType::MBC5_RUMBLE_RAM_BATTERY, false);
	else if (cartridge_type >= CartridgeType::MBC1 && cartridge_type <= CartridgeType::MBC1_RAM_BATTERY)
		m_mapper = AMapper::make<MBC1>(path_to_game, rom, rom_size, ram_size, cartridge_type == CartridgeType::MBC1_RAM_BATTERY);
}

Cartridge::~Cartridge()
{
}

void Cartridge::debug() {
	m_mapper->debug();
}

u8 Cartridge::read_byte(u16 memory_loc) {
    //std::cout << "Cartridge::read_byte -> U16: " << std::hex << memory_loc << std::endl;
    return m_mapper->read_rom(memory_loc);
}

u8 Cartridge::read_byte_at_ext_ram(u16 memory_loc)
{
    return m_mapper->read_ram(memory_loc);
}

void Cartridge::write_byte(u16 memory_loc, u8 value) {
    m_mapper->write_rom(memory_loc, value);
}

void Cartridge::write_byte_at_ext_ram(u16 memory_loc, u8 value) 
{
    m_mapper->write_ram(memory_loc, value);
}

/* Statics */
bool Cartridge::is_supported_mbc(CartridgeType type)
{
	return (type == CartridgeType::ROM_ONLY
		||	type == CartridgeType::MBC5_RAM_BATTERY 	// pk blue
		||	type == CartridgeType::MBC1);				// blarggs cpu test roms
}
