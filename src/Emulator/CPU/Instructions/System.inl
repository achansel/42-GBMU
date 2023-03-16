void NOP()		{ }
void EI()		{ std::cout << "GBMU: CPU: WARNING: INTERRUPTS ENABLED BY SOFTWARE BUT NOT FULLY IMPLEMENTED YET\n"; m_ime = true; }
void DI()		{ m_ime = false; }
void STOP()		{ std::cout << "GBMU: CPU: WARNING: STOP CALLED BUT NOT IMPLEMENTED YET\n"; PC++; }
void HALT()	    {  }