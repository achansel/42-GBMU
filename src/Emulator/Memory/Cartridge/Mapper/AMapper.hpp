#pragma once

#include <Emulator/Util/Types.hpp>
#include <Emulator/Util/File.hpp>

#include <memory>

class AMapper {
public:
	template<typename Mapper, typename ...Args>
    static std::unique_ptr<AMapper> make(Args... args)
    {
        return (std::make_unique<Mapper>(args...));
    }

	AMapper(std::string rom_path, u8 *rom, u32 rom_size, u32 ram_size)
		:	m_rom_path(rom_path),
			m_rom(rom), m_rom_size(rom_size),
			m_ram_size(ram_size)
	{
		m_ram = nullptr;
		if (m_ram_size != 0)
			m_ram = static_cast<u8*>(malloc(ram_size));
	}

	virtual ~AMapper()
	{
		free(m_ram);
		free(m_rom);
	}

	virtual u8  	read_rom(u16 address) = 0;
	virtual u8  	read_ram(u16 address) = 0;

	virtual void  	write_rom(u16 address, u8 value) = 0;
	virtual void  	write_ram(u16 address, u8 value) = 0;

	u32	get_rom_size() { return (m_rom_size); }
	u32 get_ram_size() { return (m_ram_size); }

protected:
	std::string	m_rom_path;

	u8  *m_rom;
	u32 m_rom_size;

	u8  *m_ram;
	u32 m_ram_size;
};