#include "MMU.hpp"
#include "../Emulator.hpp"

MMU::MMU(Emulator* emu)
	: m_emu(emu), m_bios_mapped(true)
{
}

// TODO: CLEAN and greatly improve readability of this switch statement
u8 MMU::get_byte_at(u16 memory_location)
{
	//std::cout << "GBMU: MMU: Memory read at " << std::hex << memory_location << std::endl;
    //if (memory_location >= 0x100 && memory_location < 0xff00)
    //std::cout << "Requested byte read at: " << std::hex << memory_location << std::endl;
    if (memory_location < 0x100 && m_bios_mapped)
        return dmg_bios[memory_location];
    switch (memory_location & 0xF000)
    {
        // Cartridge ROM
        case 0x0000:
        case 0x1000:
        case 0x2000:
        case 0x3000:
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            return m_emu->get_cartridge().read_byte(memory_location);

        // Video Ram (8K)
        case 0x8000:
        case 0x9000:
            return m_emu->get_lcd().read_byte(memory_location);

        // Cartridge External Ram
        case 0xA000:
        case 0xB000:
            return m_emu->get_cartridge().read_byte_at_ext_ram(memory_location);

        // Working Ram (8K)
        case 0xC000:
        case 0xD000:
            return m_emu->get_CPU().read_byte_at_working_ram(memory_location & 0x1FFF);

        // Working Ram Shadow, OAM, I/O, Zero Page Ram
        case 0xF000:
			/* OAM is actually not that wide, but since the region above is normally ummapped, we can return anything and its fine */
			if (memory_location < 0xFF00)
                		return m_emu->get_lcd().read_byte_at_oam(memory_location & 0x9F);

			else if (memory_location < 0xFF80)
            {
				if (memory_location == 0xFF00)
					return m_emu->get_joypad().read_byte();
				else if (memory_location >= 0xFF04 && memory_location <= 0xFF07)
					return m_emu->get_timer().read_byte(memory_location);
				else if (memory_location == 0xFF0F)
					return m_emu->get_CPU().read_if();
                else if (memory_location >= 0xFF40 && memory_location < 0xFF50)
                    return m_emu->get_lcd().read_byte(memory_location);
                else if (!(memory_location >= 0xFF10 && memory_location < 0xFF40))
				{
					//std::cout << "GBMU: MMU: READ TO UNKNOWN/UNIMPLMENTED IO REGISTER " << std::hex << memory_location << std::endl;
					return (0);
				}
	        }

			else if (memory_location < 0xFFFF)
               return m_hram[memory_location & 0x7F];

			else if (memory_location == 0xFFFF)
				return m_emu->get_CPU().read_ie();

    }
	return (0);
}

s8 MMU::get_signed_byte_at(u16 memory_location) {
    return static_cast<s8>(get_byte_at(memory_location));
}

u16 MMU::get_word_at(u16 memory_location)
{
    return Bitwise::compose_word(get_byte_at(memory_location+1), get_byte_at(memory_location));
}

// TODO: CLEAN and greatly improve readability of this switch statement
void MMU::set_byte_at(u16 memory_location, u8 value) {
	//std::cout << "GBMU: MMU: Memory write at " << std::hex << memory_location << ": " << +value << std::endl;
    switch (memory_location & 0xF000)
	{
		case 0x0000:
		case 0x1000:
		case 0x2000:
		case 0x3000:
		case 0x4000:
		case 0x5000:
		case 0x6000:
		case 0x7000:
			m_emu->get_cartridge().write_byte(memory_location, value);
			break ;
        // Video Ram (8K)
        case 0x8000:
        case 0x9000:
            m_emu->get_lcd().write_byte(memory_location, value);
			if (memory_location < 0x9800)
            	m_emu->get_lcd().updatetile(memory_location);
            break;

		case 0xA000:
		case 0xB000:
			m_emu->get_cartridge().write_byte_at_ext_ram(memory_location, value);
			break;

        // Working Ram (8K)
        case 0xC000:
        case 0xD000:
            m_emu->get_CPU().write_byte_at_working_ram(memory_location & 0x1FFF, value);
            break;

        // Working Ram Shadow, OAM, I/O, Zero Page Ram
        case 0xF000:
			/* OAM is actually not that wide, but since the region above is normally ummapped, we can return anything and its fine */
			if (memory_location < 0xFF00)
                m_emu->get_lcd().write_byte_at_oam(memory_location & 0x9F, value);
			else if (memory_location < 0xFF80)
            {
				if (memory_location == 0xFF00)
					m_emu->get_joypad().write_byte(value);
				if (memory_location == 0xFF01)
					std::cout << value;
				else if (memory_location >= 0xFF04 && memory_location <= 0xFF07)
					m_emu->get_timer().write_byte(memory_location, value);
				else if (memory_location == 0xFF0F)
					m_emu->get_CPU().write_if(value);
                else if (memory_location >= 0xFF40 && memory_location < 0xFF50)
                    m_emu->get_lcd().write_byte(memory_location, value);
				else if (memory_location == 0xFF50 && m_bios_mapped)
					m_bios_mapped = !value;
                else if (!(memory_location >= 0xFF10 && memory_location < 0xFF40))
				{
					//std::cout << "GBMU: MMU: WRITE TO UNKNOWN/UNIMPLMENTED IO REGISTER " << std::hex << memory_location << ", VALUE: " << +value << std::endl;
				}
	        }

			else if (memory_location < 0xFFFF)
            	m_hram[memory_location & 0x7F] = value;

			else if (memory_location == 0xFFFF)
				m_emu->get_CPU().write_ie(value);

        // We didn't match any case bruv
        default:
            break;
    }        
}

void MMU::set_word_at(u16 memory_location, u16 value)
{
    set_byte_at(memory_location, static_cast<u8>(value));
    set_byte_at(memory_location+1, static_cast<u8>(value >> 8));
}
