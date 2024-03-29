#pragma once

class Emulator;

#include <array>
#include <memory>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <functional>

#include <Emulator/Memory/IMMIO.hpp>
#include <Emulator/Util/Types.hpp>

using namespace std::placeholders;

class MMU : public IMMIO {
public:
    MMU(Emulator *emu);

    u8 get_byte_at(u16 memory_location);
    s8 get_signed_byte_at(u16 memory_location);
    u16 get_word_at(u16 memory_location);

    void set_byte_at(u16 memory_location, u8 value);
    void set_word_at(u16 memory_location, u16 value);

    u8      read_byte(u16 address) { (void) address; return (0xFF); };
    void    write_byte(u16 address, u8 value) { (void) address; (void) value; };

private:
    template<typename Device, typename read_func = std::function<u8(Device *, u16)>, typename write_func = std::function<void(Device *, u16, u8)> >
    void    register_range(u16 low, u16 size, Device *obj, read_func custom_read = &Device::read_byte, write_func custom_write = &Device::write_byte)
    {
        std::function<u8(u16)>       read = std::bind(custom_read, obj, _1);
        std::function<void(u16, u8)> write = std::bind(custom_write, obj, _1, _2);

        m_memory_map.emplace_back(low, size, read, write);
    }

    bool        unmap_range(u16 low, u16 high);
    void        unmap_bootrom();

    u8          read_bootrom(u16 address);
    void        write_bootrom(u16 address, u8 value);

    u8          read_ioreg(u16 memory_location);
    void        write_ioreg(u16 address, u8 value);

    u8          read_byte_at_hram(u16 address);
    void        write_byte_at_hram(u16 address, u8 value);

    Emulator	*m_emu;
	bool		m_bios_mapped;

    struct MemoryRegion {
        MemoryRegion(u16 begin, u16 size, std::function<u8(u16)> read_func, std::function<void(u16, u8)> write_func)
            : m_begin(begin), m_size(size), m_read_func(read_func), m_write_func(write_func)
        {}

        bool operator<(const MemoryRegion &rhs)
        {
            return (this->m_begin < rhs.m_begin);
        }

        bool    is_within(const u16 address) { return (address >= m_begin && address < (m_begin + m_size)); }

        u16                 m_begin;
        u16                 m_size;

        std::function<u8(u16)>                  m_read_func;
        std::function<void(u16, u8)>            m_write_func;
    };

    //TODO: Maybe use a sort of tree to optimize memory accesses.
    std::vector<MemoryRegion>   m_memory_map;
	std::array<u8, 0x80>        m_hram {};


    u8 dmg_bios[0x100] = {
        0x31, 0xfe, 0xff, 0xaf, 0x21, 0xff, 0x9f, 0x32, 0xcb, 0x7c, 0x20, 0xfb,
        0x21, 0x26, 0xff, 0x0e, 0x11, 0x3e, 0x80, 0x32, 0xe2, 0x0c, 0x3e, 0xf3,
        0xe2, 0x32, 0x3e, 0x77, 0x77, 0x3e, 0xfc, 0xe0, 0x47, 0x11, 0x04, 0x01,
        0x21, 0x10, 0x80, 0x1a, 0xcd, 0x95, 0x00, 0xcd, 0x96, 0x00, 0x13, 0x7b,
        0xfe, 0x34, 0x20, 0xf3, 0x11, 0xd8, 0x00, 0x06, 0x08, 0x1a, 0x13, 0x22,
        0x23, 0x05, 0x20, 0xf9, 0x3e, 0x19, 0xea, 0x10, 0x99, 0x21, 0x2f, 0x99,
        0x0e, 0x0c, 0x3d, 0x28, 0x08, 0x32, 0x0d, 0x20, 0xf9, 0x2e, 0x0f, 0x18,
        0xf3, 0x67, 0x3e, 0x64, 0x57, 0xe0, 0x42, 0x3e, 0x91, 0xe0, 0x40, 0x04,
        0x1e, 0x02, 0x0e, 0x0c, 0xf0, 0x44, 0xfe, 0x90, 0x20, 0xfa, 0x0d, 0x20,
        0xf7, 0x1d, 0x20, 0xf2, 0x0e, 0x13, 0x24, 0x7c, 0x1e, 0x83, 0xfe, 0x62,
        0x28, 0x06, 0x1e, 0xc1, 0xfe, 0x64, 0x20, 0x06, 0x7b, 0xe2, 0x0c, 0x3e,
        0x87, 0xe2, 0xf0, 0x42, 0x90, 0xe0, 0x42, 0x15, 0x20, 0xd2, 0x05, 0x20,
        0x4f, 0x16, 0x20, 0x18, 0xcb, 0x4f, 0x06, 0x04, 0xc5, 0xcb, 0x11, 0x17,
        0xc1, 0xcb, 0x11, 0x17, 0x05, 0x20, 0xf5, 0x22, 0x23, 0x22, 0x23, 0xc9,
        0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b, 0x03, 0x73, 0x00, 0x83,
        0x00, 0x0c, 0x00, 0x0d, 0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e,
        0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99, 0xbb, 0xbb, 0x67, 0x63,
        0x6e, 0x0e, 0xec, 0xcc, 0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e,
        0x3c, 0x42, 0xb9, 0xa5, 0xb9, 0xa5, 0x42, 0x3c, 0x21, 0x04, 0x01, 0x11,
        0xa8, 0x00, 0x1a, 0x13, 0xbe, 0x20, 0xfe, 0x23, 0x7d, 0xfe, 0x34, 0x20,
        0xf5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xfb, 0x86, 0x20, 0xfe,
        0x3e, 0x01, 0xe0, 0x50
    };

};
