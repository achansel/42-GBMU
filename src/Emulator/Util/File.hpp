#pragma once

#include <fstream>
#include <Emulator/Util/Types.hpp>

namespace Util
{
    inline void load_save(std::string m_sav_path, u8 *ram, u32 ram_size)
    {
        std::ifstream d(m_sav_path);

        d.read((char *) ram, ram_size);
    }

    inline void write_save(std::string m_sav_path, u8 *ram, u32 ram_size)
    {
        std::ofstream d(m_sav_path, std::ofstream::trunc);

        d.write((char *) ram, ram_size);
    }
};
