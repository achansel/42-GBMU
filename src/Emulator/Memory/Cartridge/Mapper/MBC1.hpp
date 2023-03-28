#include "AMapper.hpp"

/* TODO: Implement a proper MBC1, this one was done quickly in order to test a few games. */
class MBC1 : public AMapper
{
public:
    MBC1(std::string rom_path, u8 *rom, u32 rom_size, u32 ram_size, bool battery)
        :   AMapper(rom_path, rom, rom_size, ram_size),
            m_battery(battery), m_ram_enabled(false), m_selected_rom_bank(1), m_selected_ram_bank(0)
    {
        if (m_battery)
            Util::load_save(m_rom_path + ".sav", m_ram, ram_size);
    }

    ~MBC1()
    {
        if (m_battery)
            Util::write_save(m_rom_path + ".sav", m_ram, m_ram_size);
    }

    u8  read_rom(u16 address)
    {
        //TODO: PREVENT IT FROM SEGFAULT BY CHANGING WRITE ROM
        if (address <= 0x3FFF)
            return (m_rom[address]);
        else
            return (m_rom[m_selected_rom_bank * 0x4000 + (address & 0x3FFF)]);
    }
	u8  read_ram(u16 address)
    {
        //TODO: PREVENT IT FROM SEGFAULT BY CHANGING WRITE ROM
        if (m_ram && m_ram_enabled)
            return (m_ram[m_selected_ram_bank * 0x2000 + (address & 0x1FFF)]);
        return (0xFF);
    }

	void  write_rom(u16 address, u8 value)
    {
        if (address <= 0x1FFF)
        {
            if (value == 0x0A)
                m_ram_enabled = true;
            else if (value == 0x00)
                m_ram_enabled = false;
        }
        else if (address >= 0x2000 && address <= 0x3FFF)
            m_selected_rom_bank = (value ? value & 0x1F : 1);
        else if (address <= 0x5FFF)
            m_selected_ram_bank = value & 0x0F;
    }
	void  write_ram(u16 address, u8 value)
    {
        if (m_ram && m_ram_enabled)
            m_ram[m_selected_ram_bank * 0x2000 + (address & 0x1FFF)] = value;
    }
private:
    bool    m_battery;

    bool    m_ram_enabled;
    u16     m_selected_rom_bank;
    u16     m_selected_ram_bank;
};