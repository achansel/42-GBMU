#pragma once

#include <Emulator/Util/Types.hpp>

namespace Bitwise {
    inline u16 compose_word(u8 low, u8 high)
    {
        return static_cast<u16>((low<<8)|high);
    }
}
