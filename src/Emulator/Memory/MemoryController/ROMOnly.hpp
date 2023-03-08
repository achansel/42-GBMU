#pragma once

#include <Emulator/Memory/MemoryController/MemoryController.hpp>

class ROMOnly : public MemoryController
{
public:
    ROMOnly(Emulator* emu);
    ~ROMOnly() {};

    u8 get_byte_at(u16 memory_location);
    void set_byte_at(u16 memory_location, u8 value);  
};