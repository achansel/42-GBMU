void NOP()		{ }
void EI()		{ m_ime = true; /* std::cout << "GBMU: CPU: WARNING: INTERRUPTS ENABLED BY SOFTWARE\n";*/ }
void DI()		{ m_ime = false; }
void STOP()		{ /*std::cout << "GBMU: CPU: WARNING: STOP CALLED BUT NOT IMPLEMENTED YET\n";*/ PC++; }
void HALT()	    { m_halted = true; }