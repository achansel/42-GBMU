#pragma once

#include "Types.hpp"

#define COMPOSE_BYTES(low, high) \
                        static_cast<u16>((low<<8)|high)

#define CHECK_BIT(v, pos) \
                        ((v) & (1 <<(pos)))

#define SET_BIT(v, pos, n) \
                        v ^= (-n ^ v) & (1U << n)

namespace Bitwise {
    inline u16 compose_bytes(u8 low, u8 high)
    {
        return static_cast<u16>((low<<8)|high);
    }
}
