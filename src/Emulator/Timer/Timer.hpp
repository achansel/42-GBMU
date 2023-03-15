#pragma once

#include <Emulator/Util/Types.hpp>

class Emulator;

class Timer {
public:
	Timer(Emulator *emu);

	void	step(u8 tcycles);

	void	write_byte(u16 address, u8 value);
	u8		read_byte(u16 address);

	enum ClockSpeed {
		FREQ_4096HZ = 0, FREQ_262144HZ = 1,  FREQ_65536HZ = 2, FREQ_16384HZ = 3
	};
private:
	Emulator	*m_emu;

	static const u32 s_processor_freq	= 4194304;

	static const u32 s_4096hz			= 1024;
	static const u32 s_16384hz			= 256;
	static const u32 s_65536hz			= 64;
	static const u32 s_262144hz			= 16;

	static const u32 s_div_register_clocks = s_16384hz;

	u8 m_register_div;
	u8 m_register_tima;
	u8 m_register_tma;

	u32 m_tclock;
	u32 m_dclock;

	bool m_enabled;
	ClockSpeed m_clock_speed;
};