#include "AMapper.hpp"

class MBC5 : public AMapper
{
public:
    MBC5(std::string rom_path, u8 *rom, u32 rom_size, u32 ram_size, bool battery, bool rumble)
        :   AMapper(rom_path, rom, rom_size, ram_size),
            m_battery(battery), m_rumble(rumble), m_ram_enabled(false), m_selected_rom_bank(1), m_selected_ram_bank(0)
    {
        if (m_battery)
            Util::load_save(m_rom_path + ".sav", m_ram, ram_size);
    }

    ~MBC5()
    {
        if (m_battery)
            Util::write_save(m_rom_path + ".sav", m_ram, m_ram_size);
    }

	void debug()
	{
		std::cout << "GBMU: CARTRIDGE: MBC5 (ROM BANK/RAM BANK): " << std::hex << m_selected_rom_bank << "/" << m_selected_ram_bank << std::endl;
		std::cout << "\t\t\t\t\t\t\t\t\t\tBATTERY: " << std::boolalpha << m_battery << ", RUMBLE: " << m_rumble << std::endl;
	}

    u8  read_rom(u16 address)
    {
        /* TODO: Prevent this function from accessing out of bounds memory, by checking if the mapped bank is correct in write_rom and write ram,
			Do it there and not in this function for obvious performance reasons */
        if (address <= 0x3FFF)
            return (m_rom[address]);
        else
            return (m_rom[m_selected_rom_bank * 0x4000 + (address & 0x3FFF)]);
    }
	u8  read_ram(u16 address)
    {
        /* TODO: Prevent this function from accessing out of bounds memory, by checking if the mapped bank is correct in write_rom and write ram,
			Do it there and not in this function for obvious performance reasons */
        if (m_ram && m_ram_enabled)
            return (m_ram[m_selected_ram_bank * 0x2000 + (address & 0x1FFF)]);
		else
		{
			std::cout << "GBMU: DEBUG: RAM READ WITHOUT HAVING IT ENABLED" << std::endl;
        	return (0xFF);
		}
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
        else if (address <= 0x2FFF)
            m_selected_rom_bank = (m_selected_ram_bank & (1 << 8)) | value;
        else if (address <= 0x3FFF)
            m_selected_rom_bank = (m_selected_ram_bank & 0xFF) | ((value & 1) << 8);
        else if (address <= 0x5FFF)
            m_selected_ram_bank = value & 0x0F;
    }
	void  write_ram(u16 address, u8 value)
    {
        if (m_ram && m_ram_enabled)
            m_ram[m_selected_ram_bank * 0x2000 + (address & 0x1FFF)] = value;
		else
		{
			std::cout << "GBMU: CART: DEBUG: RAM READ WITHOUT HAVING IT ENABLED" << std::endl;
		}

    }
private:
    bool    m_battery;
    bool    m_rumble;

    bool    m_ram_enabled;
    u16     m_selected_rom_bank;
    u16     m_selected_ram_bank;
};