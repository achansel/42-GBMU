#pragma once

class Emulator;

#include <array>
#include "Emulator/Util/Types.hpp"

class MemoryController
{
public:
    virtual u8 get_byte_at(u16 memory_location) = 0;
    virtual void set_byte_at(u16 memory_location, u8 value) = 0;
    MemoryController(Emulator* emu) { m_emu = emu; }
    virtual ~MemoryController() {}
protected:
    Emulator* m_emu;
    std::array<u8, 0x80> m_zero_page_ram{0};
};
