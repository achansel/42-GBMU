#include <Emulator/CPU/CPU.hpp>

void CPU::NOP()		{ m_tclock += 4; PC++; }
void CPU::EI()		{ std::cout << "GBMU: CPU: WARNING: INTERRUPTS ENABLED BY SOFTWARE BUT NOT IMPLEMENTED YET\n"; m_ime = true; m_tclock += 4; PC++; }
void CPU::DI()		{ m_ime = false; m_tclock += 4; PC++; }
//void CPU::STOP()	{ m_tclock += 4; PC+=2; }
//void CPU::HALT()	{ m_tclock += 4; PC++; }