#pragma once

#include <Emulator/Util/Types.hpp>
#include <functional>

class IMMIO {
public:
    /* Check while using an IMMIO * here, instead of void * */
    typedef void                            *read_func;
    typedef void                            *write_func;

    IMMIO() = default;

    virtual u8      read_byte(u16 address) = 0;
    virtual void    write_byte(u16 address, u8 value) = 0;
};