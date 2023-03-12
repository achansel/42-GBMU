/* INSTRUCTIONS FOR GB EMU */

#include <Emulator/Bitwise.hpp>
#include <Emulator/Types.hpp>
#include <Emulator/CPU/CPU.hpp>

/* Registers instructions */
// TODO: Maybe try casting to u16 the regs array and read from it
ALWAYS_INLINE u8	CPU::GET_REG(u8 reg)						{ return m_regs[reg]; }
ALWAYS_INLINE u8	CPU::GET_REG_BIT(u8 reg, u8 bit)			{ return (m_regs[reg] >> bit) & 1; }
ALWAYS_INLINE void	CPU::SET_REG(u8 reg, u8 value)				{ m_regs[reg] = value; }
ALWAYS_INLINE void	CPU::SET_REG_BIT(u8 reg, u8 bit, u8 value)	{ m_regs[reg] ^= ((-value) ^ m_regs[reg]) & (1U << bit); }
ALWAYS_INLINE void	CPU::INC_REG(u8 reg)						{ regs[r]++; }
ALWAYS_INLINE void	CPU::DEC_REG(u8 reg)						{ regs[r]--; }


ALWAYS_INLINE u16 CPU::GET_COMPOSED_REG(u8 reg)
{
	return Bitwise::compose_word(m_regs[reg >> 4], m_regs[reg & 0xF]);
}
ALWAYS_INLINE void CPU::SET_COMPOSED_REG(u8 reg, u16 value)
{
    this->SET_REG(reg & 0x0F, static_cast<u8>(value));
	this->SET_REG(reg >> 4,	  static_cast<u8>(value >> 8));
}
ALWAYS_INLINE void	CPU::INC_COMPOSED_REG(u8 reg) { value = GET_COMPOSED_REG(r); value++; SET_COMPOSED_REG(r, value); }
ALWAYS_INLINE void	CPU::DEC_COMPOSED_REG(u8 reg) {	value = GET_COMPOSED_REG(r); value--; SET_COMPOSED_REG(r, value); }

ALWAYS_INLINE void CPU::SET_FLAG(u8 flag, u8 value)
{
	this->SET_REG_BIT(RegisterF, flag, value);
}
ALWAYS_INLINE u8 CPU::GET_FLAG(u8 flag) 
{
	return this->GET_REG_BIT(RegisterF, flag);
}

/* Memory instructions */
ALWAYS_INLINE u8 CPU::FETCH_BYTE(u16 address)
{
	m_tclock += 4;
	PC += 1;

	return m_emu->get_MMU().get_byte_at(address);
}
ALWAYS_INLINE u16 CPU::FETCH_WORD(u16 address)
{
	m_tclock += 8;
	PC += 2;

	return m_emu->get_MMU().get_word_at(address);
}
ALWAYS_INLINE u8 CPU::GET_BYTE(u16 address)
{
	m_tclock += 4;

	return m_emu->get_MMU().get_byte_at(address);
}
ALWAYS_INLINE u16 CPU::GET_WORD(u16 address)
{
	m_tclock += 8;

	return m_emu->get_MMU().get_word_at(address);
}
ALWAYS_INLINE void CPU::WRITE_BYTE(u16 address, u8 byte)
{
	m_tclock += 4;

	m_emu->get_MMU().set_byte_at(address, byte);
}
ALWAYS_INLINE void CPU::WRITE_WORD(u16 address, u16 word)
{
	m_tclock += 8;

	m_emu->get_MMU().set_word_at(address, word);
}

ALWAYS_INLINE u8 CPU::GET_TARGET_BIT(u8 base, u8 op)
{
	return ((op - base) >> 3)
}