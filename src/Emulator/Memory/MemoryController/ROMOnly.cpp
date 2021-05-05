#include "ROMOnly.hpp"
#include <Emulator/Emulator.hpp>

ROMOnly::ROMOnly(Emulator* emu) : MemoryController(emu)
{
}

u8 ROMOnly::get_byte_at(u16 memory_location)
{   
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

        // Cartridge External Ram (Unmapped)
        case 0xA000:
        case 0xB000:
            return 0;

        // Working Ram (8K)
        case 0xC000:
        case 0xD000:
        case 0xE000:
            return m_emu->get_CPU().read_byte_at_working_ram(memory_location & 0x1FFF);

        // Working Ram Shadow, OAM, I/O, Zero Page Ram
        case 0xF000:
            switch (memory_location & 0x0F00)
            {
                // Working Ram Shadow
                case 0x000: case 0x100: case 0x200: case 0x300: case 0x400: case 0x500: case 0x600: 
                case 0x700: case 0x800: case 0x900: case 0xA00: case 0xB00: case 0xC00: case 0xD00:
                        return m_emu->get_CPU().read_byte_at_working_ram(memory_location & 0x1FFF);
                // OAM
                case 0xE00:
                        // Actually a quick impl here, if one tries to read above the OAM, (0xFEA0 to 0xFF00),
                        // it returns junk aka a byte from OAM, this shouldn't be a problem cause this
                        // memory is normally not mapped, and also returns junk on a physical console
                        return m_emu->get_lcd().read_byte_at_oam(memory_location & 0x9F);
                case 0xF00:
                    // ZERO PAGE RAM
                    if (memory_location >= 0xFF80)
                    {
                        return m_zero_page_ram[memory_location & 0x7F];
                    }
                    // IO REGS
                    else
                    {
                        switch (memory_location & 0x00F0)
                        {
                            // LCD QUICK DONE LOL
                            case 0x40: case 0x50:
                                return m_emu->get_lcd().read_byte(memory_location);
                            default:
                                return 0; 
                        }
                    }
                default:
                    return 0;
            }

        // We didn't match any case
        default:
            return 0;
    }        
}

void ROMOnly::set_byte_at(u16 memory_location, u8 value)
{   
    switch (memory_location & 0xF000)
    {
        // Video Ram (8K)
        case 0x8000:
        case 0x9000:
            m_emu->get_lcd().write_byte(memory_location, value);
            m_emu->get_lcd().updatetile(memory_location);
            break;

        // Working Ram (8K)
        case 0xC000:
        case 0xD000:
        case 0xE000:
            m_emu->get_CPU().write_byte_at_working_ram(memory_location & 0x1FFF, value);
            break;

        // Working Ram Shadow, OAM, I/O, Zero Page Ram
        case 0xF000:
            switch (memory_location & 0x0F00)
            {
                // Working Ram Shadow
                case 0x000: case 0x100: case 0x200: case 0x300: case 0x400: case 0x500: case 0x600: 
                case 0x700: case 0x800: case 0x900: case 0xA00: case 0xB00: case 0xC00: case 0xD00:
                        m_emu->get_CPU().write_byte_at_working_ram(memory_location & 0x1FFF, value);
                        break;

                // OAM
                case 0xE00:
                        // Actually a quick impl here, if one tries to read above the OAM, (0xFEA0 to 0xFF00),
                        // it returns junk aka a byte from OAM, this shouldn't be a problem cause this
                        // memory is normally not mapped, and also returns junk on a physical console
                        m_emu->get_lcd().write_byte_at_oam(memory_location & 0x9F, value);
                        break;
                case 0xF00:
                    // ZERO PAGE RAM
                    if (memory_location >= 0xFF80)
                    {
                        m_zero_page_ram[memory_location & 0x7F] = value;
                    }
                    // IO REGS
                    else
                    {
                        switch (memory_location & 0x00F0)
                        {
                            // LCD QUICK DONE LOL
                            case 0x40: case 0x50:
                                m_emu->get_lcd().write_byte(memory_location, value);
                                break;
                        }
                    }
                    break;
                default:
                    break;
            }

        // We didn't match any case bruv
        default:
            break;
    }        
}