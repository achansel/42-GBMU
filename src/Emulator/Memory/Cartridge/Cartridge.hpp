#ifndef GAMEBOYPROJECT_CARTRIDGE_HPP
#define GAMEBOYPROJECT_CARTRIDGE_HPP


#include <string>
#include <array>
#include <fstream>
#include <vector>
#include <memory>
#include <cstdlib>

#include "Emulator/Util/Types.hpp"

class Cartridge {
public:
    explicit Cartridge(const std::string& path_to_game);
    ~Cartridge();
    
    int get_cart_type();

    u8 read_byte(u16 memory_loc);
    u8 read_byte_at_ext_ram(u16 memory_loc);

    void write_byte(u16 memory_loc, u8 value);
    void write_byte_at_ext_ram(u16 memory_loc, u8 value);
private:
	std::array<u8, 8192> m_external_ram{0};
	u8* m_rom;
    int rom_size;
};


#endif //GAMEBOYPROJECT_CARTRIDGE_HPP