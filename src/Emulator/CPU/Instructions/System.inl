void NOP()		{ }
void EI()		{ std::cout << "GBMU: CPU: WARNING: INTERRUPTS ENABLED BY SOFTWARE BUT NOT IMPLEMENTED YET\n"; m_ime = true; }
void DI()		{ m_ime = false; }
//void STOP()	{ m_tclock += 4; PC+=2; }
//void HALT()	{ m_tclock += 4; PC++; }