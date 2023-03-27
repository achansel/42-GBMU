#ifndef GAMEBOYPROJECT_CARTRIDGE_HPP
#define GAMEBOYPROJECT_CARTRIDGE_HPP


#include <string>
#include <array>
#include <fstream>
#include <vector>
#include <memory>
#include <cstdlib>

#include <Emulator/Util/Types.hpp>
#include <Emulator/Memory/Cartridge/Mapper/AMapper.hpp>
#include <Emulator/Memory/Cartridge/Mapper/ROMOnly.hpp>
#include <Emulator/Memory/Cartridge/Mapper/MBC5.hpp>

enum class CartridgeType {
	ROM_ONLY						= 0x00,
	MBC1							= 0x01,
	MBC1_RAM						= 0x02,
	MBC1_RAM_BATTERY				= 0x03,
	MBC2							= 0x05,
	MBC2_BATTERY					= 0x06,
	ROM_RAM							= 0x08,
	ROM_RAM_BATTERY					= 0x09,
	MMM_01							= 0x0B,
	MMM_01_RAM						= 0x0C,
	MMM_01_RAM_BATTERY				= 0x0D,
	MBC3_TIMER_BATTERY				= 0x0F,
	MBC3_TIMER_RAM_BATTERY			= 0x10,
	MBC3							= 0x11,
	MBC3_RAM						= 0x12,
	MBC3_RAM_BATTERY				= 0x13,
	MBC5							= 0x19,
	MBC5_RAM						= 0x1A,
	MBC5_RAM_BATTERY				= 0x1B,
	MBC5_RUMBLE						= 0x1C,
	MBC5_RUMBLE_RAM					= 0x1D,
	MBC5_RUMBLE_RAM_BATTERY			= 0x1E,
	MBC6							= 0x20,
	MBC7_SENSOR_RUMBLE_RAM_BATTERY	= 0x22,
	POCKET_CAMERA					= 0xFC,
	BANDAI_TAMA5					= 0xFD,
	HUC3							= 0xFE,
	HUC1_RAM_BATTERY				= 0xFF,
};

class Cartridge {
public:
    explicit Cartridge(const std::string& path_to_game);
    ~Cartridge();
    
	void 	debug();
    u8		read_byte(u16 memory_loc);
    u8		read_byte_at_ext_ram(u16 memory_loc);

    void 	write_byte(u16 memory_loc, u8 value);
    void	write_byte_at_ext_ram(u16 memory_loc, u8 value);

	/*
	u8		raw_get_rom(u32 address);
	u8		raw_get_ram(u32 address);

	void	raw_write_rom(u32 address, u8 byte);
	void	raw_write_ram(u32 address, u8 byte);
	*/

	static bool is_supported_mbc(CartridgeType t);
private:
	std::string		m_title;
	std::string 	m_manufacturer_code;

	u8				m_cgb_flag;
	u8				m_destination_code;
	u8				m_mask_rom_version_number;

	std::unique_ptr<AMapper>	m_mapper;
};


#endif //GAMEBOYPROJECT_CARTRIDGE_HPP