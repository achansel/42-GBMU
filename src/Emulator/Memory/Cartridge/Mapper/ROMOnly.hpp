#include <AMapper.hpp>

class ROMOnly : public AMapper
{
public:
    ROMOnly(u8 *rom, u32 rom_size, u8 *ram, u32 ram_size)
        : AMapper(rom, rom_size, ram, ram_size)
    {}

    u8  read_rom(u16 address)
    {
        return (m_rom[address]);
    }
	u8  read_ram(u16 address)
    {
        if (m_ram)
            return (m_ram[address & 0x1FFF]);
        return (0xFF);
    }

	void  write_rom(u16 address, u8 value) { return ; }
	void  write_ram(u16 address, u8 value) { if (m_ram) m_ram[address & 0x1FFF] = value; }
};