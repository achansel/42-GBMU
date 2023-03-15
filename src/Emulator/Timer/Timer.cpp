#include <Emulator/Timer/Timer.hpp>
#include <Emulator/Emulator.hpp>

Timer::Timer(Emulator *emu)
	: m_emu(emu),
	m_tclock(0),
	m_dclock(0),
	m_enabled(false)
{}

void Timer::step(u8 tcycles)
{
	m_tclock += tcycles;
	m_dclock += tcycles;

	if (!m_emu->get_CPU().is_stopped() && m_dclock >= Timer::s_div_register_clocks)
	{
		m_register_div++;
		m_dclock -= Timer::s_div_register_clocks;
	}

	if (!m_enabled)
		return ;

	if (m_clock_speed == FREQ_262144HZ && m_tclock >= s_262144hz)
		m_tclock -= s_262144hz;
	else if (m_clock_speed == FREQ_65536HZ && m_tclock >= s_65536hz)
		m_tclock -= s_65536hz;
	else if (m_clock_speed == FREQ_16384HZ && m_tclock >= s_16384hz)
		m_tclock -= s_16384hz;
	else if (m_clock_speed == FREQ_4096HZ && m_tclock >= s_4096hz)
		m_tclock -= s_4096hz;
	else 
		return ;
	
	// Timer overflow
	if (m_register_tima == 0xFF)
	{
		m_emu->get_CPU().request_interrupt(CPU::Interrupt::TIMER);
		m_register_tima = m_register_tma;
	}
}


//TODO: If a TMA write is executed on the same cycle as the content of TMA is transferred to TIMA due to a timer overflow, the old value is transferred to TIMA.
void Timer::write_byte(u16 address, u8 value)
{
	switch (address)
	{
		case 0xFF04:
			m_register_div = 0;
			break ;
		case 0xFF05:
			m_register_tima = value;
			break ;
		case 0xFF06:
			m_register_tma = value;
			break ;
		case 0xFF07:
			m_tclock		= 0; //maybe needs removal?
			m_enabled		= (value >> 2) & 1;
			m_clock_speed	= static_cast<ClockSpeed>((value >> 0) & 3);
			break ;
	}
}

u8 Timer::read_byte(u16 address)
{
	switch (address)
	{
		case 0xFF04:
			return (m_register_div);
		case 0xFF05:
			return (m_register_tima);
		case 0xFF06:
			return (m_register_tma);
		case 0xFF07:
			return (m_enabled 						<< 2 |
					static_cast<u8>(m_clock_speed)	<< 0);
		default:
			return (0xFF);
	}
}