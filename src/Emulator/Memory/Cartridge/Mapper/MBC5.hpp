#include <AMapper.hpp>

class MBC5 : public AMapper
{
public:
    MBC5(std::string m_rom_path, u8 *rom, u32 rom_size, u8 *ram, u32 ram_size, bool battery, bool rumble)
        :   AMapper(m_rom_path, rom, rom_size, ram, ram_size),
            m_battery(battery), m_rumble(rumble)
    {
        if (m_battery)
            Util::load_save(m_rom_path + ".sav", ram, ram_size);
    }

    ~MBC5()
    {
        if (m_battery)
            Util::write_save(m_rom_path + ".sav", ram, ram_size);
    }

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
private:
    bool    m_battery;
    bool    m_rumble;
};