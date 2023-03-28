/* Micro instructions */
template<int SIGN, int CARRY>
ALWAYS_INLINE void ADD_A_8(u8 operand)	
{
	u8	a = GET_REG(RegisterA);
	u8	carry = GET_FLAG(CarryFlag) * CARRY;
	u16 res;

	if constexpr (SIGN == -1)
		res = a - operand - carry;
	else
		res = a + operand + carry;

	SET_FLAG(ZeroFlag, static_cast<u8>(res) == 0);
	if constexpr (SIGN == -1)
	{
		SET_FLAG(SubstractFlag, 1);
		SET_FLAG(HalfCarryFlag, (a & 0xF) < (operand & 0xF) + carry);
	}
	else
	{
		SET_FLAG(SubstractFlag, 0);
		SET_FLAG(HalfCarryFlag, ((a & 0xF) + (operand & 0xF)) + carry > 0xF);
	}

	SET_FLAG(CarryFlag,	(res >> 8) != 0);

	SET_REG(RegisterA, static_cast<u8>(res));
}

template<ComposedRegister dst, ComposedRegister src>
void ADD_REG16_REG16()
{
	m_tclock += 4;

	u16 r0 = GET_COMPOSED_REG(dst);
	u16 r1 = GET_COMPOSED_REG(src);

	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag,	((r0 & 0xFFF) + (r1 & 0xFFF)) > 0xFFF);
	SET_FLAG(CarryFlag,		(r0 + r1) >> 16);

	SET_COMPOSED_REG(dst, static_cast<u16>(r0 + r1));
}

void ADD_SP_IMM8()
{
	m_tclock += 8;

	s8 byte = FETCH_BYTE();
	u16 sp = GET_COMPOSED_REG(RegisterSP);

	SET_FLAG(ZeroFlag,		0);
	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, ((byte & 0xF)  + (sp & 0xF))  > 0xF);
	SET_FLAG(CarryFlag,		((byte & 0xFF) + (sp & 0xFF)) > 0xFF);

	SET_COMPOSED_REG(RegisterSP, static_cast<u16>(sp + byte));
}

enum class ALUOP {
	ADC, ADD, SBC, SUB, AND, XOR, OR, CP
};

ALWAYS_INLINE void ADC(u8 operand)			{ ADD_A_8<1, 1>(operand); }
ALWAYS_INLINE void SBC(u8 operand)			{ ADD_A_8<-1, 1>(operand); }
ALWAYS_INLINE void ADD(u8 operand)			{ ADD_A_8<1, 0>(operand); }
ALWAYS_INLINE void SUB(u8 operand)			{ ADD_A_8<-1, 0>(operand); }

ALWAYS_INLINE void AND(u8 operand)			{ u8 a = GET_REG(RegisterA) & operand; SET_REG(RegisterA, a); SET_FLAG(ZeroFlag, a == 0); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, 1); SET_FLAG(CarryFlag, 0); }
ALWAYS_INLINE void XOR(u8 operand)			{ u8 a = GET_REG(RegisterA) ^ operand; SET_REG(RegisterA, a); SET_FLAG(ZeroFlag, a == 0); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, 0); SET_FLAG(CarryFlag, 0); }
ALWAYS_INLINE void OR(u8 operand)			{ u8 a = GET_REG(RegisterA) | operand; SET_REG(RegisterA, a); SET_FLAG(ZeroFlag, a == 0); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, 0); SET_FLAG(CarryFlag, 0); }
ALWAYS_INLINE void CP(u8 operand)			{ u8 a = GET_REG(RegisterA); SET_FLAG(ZeroFlag, a == operand); SET_FLAG(SubstractFlag, 1); SET_FLAG(HalfCarryFlag, ((a - operand) & 0xF) > (a & 0xF)); SET_FLAG(CarryFlag, a < operand); }

template<ALUOP op>
ALWAYS_INLINE void ALU_OP(u8 operand)
{
	if constexpr (op == ALUOP::ADC)
		ADC(operand);
	else if constexpr (op == ALUOP::ADD)
		ADD(operand);
	else if constexpr (op == ALUOP::SBC)
		SBC(operand);
	else if constexpr (op == ALUOP::SUB)
		SUB(operand);
	else if constexpr (op == ALUOP::AND)
		AND(operand);
	else if constexpr (op == ALUOP::XOR)
		XOR(operand);
	else if constexpr (op == ALUOP::OR)
		OR(operand);
	else
		CP(operand);
}

template<ALUOP op, Register r>
void ALU_OP_REG()							{ ALU_OP<op>(GET_REG(r)); }
template<ALUOP op>
void ALU_OP_ADDR_HL()						{ ALU_OP<op>(GET_BYTE(GET_COMPOSED_REG(RegisterHL))); }
template<ALUOP op>
void ALU_OP_IMM8()							{ ALU_OP<op>(FETCH_BYTE()); }

template<ComposedRegister reg>
void DEC_REG() 								{ m_tclock += 4; DEC_COMPOSED_REG(reg); }
template<ComposedRegister reg>
void INC_REG() 								{ m_tclock += 4; INC_COMPOSED_REG(reg); }

template<Register reg>
void DEC_REG()  							{ DEC_REG(reg); SET_FLAG(ZeroFlag, GET_REG(reg) == 0); SET_FLAG(SubstractFlag, 1); SET_FLAG(HalfCarryFlag, (GET_REG(reg) & 0xF) == 0xF); }
template<Register reg>
void INC_REG() 								{ INC_REG(reg); SET_FLAG(ZeroFlag, GET_REG(reg) == 0); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, (GET_REG(reg) & 0xF) == 0x00); }

void DEC_ADDR_HL()							{ u16 address = GET_COMPOSED_REG(RegisterHL); u8 b = GET_BYTE(address) - 1; SET_FLAG(ZeroFlag, b == 0); SET_FLAG(SubstractFlag, 1); SET_FLAG(HalfCarryFlag, (b >> 4) & 1); WRITE_BYTE(address, b); }
void INC_ADDR_HL()							{ u16 address = GET_COMPOSED_REG(RegisterHL); u8 b = GET_BYTE(address) + 1; SET_FLAG(ZeroFlag, b == 0); SET_FLAG(SubstractFlag, 0); SET_FLAG(HalfCarryFlag, (b >> 4) & 1); WRITE_BYTE(address, b); }

// DAA: Decimal Adjust A, the way the GB handles BCD (Binary-Coded Decimal) arithmetic
void DAA()
{
	u8 a = GET_REG(RegisterA);
	if (GET_FLAG(SubstractFlag))
	{
		if (GET_FLAG(CarryFlag))
			a -= 0x60;
		if (GET_FLAG(HalfCarryFlag))
			a -= 0x6;
	}
	else
	{
		if (GET_FLAG(CarryFlag) || a > 0x99)
		{
			a += 0x60;
			SET_FLAG(CarryFlag, 1);
		}
		if (GET_FLAG(HalfCarryFlag) || ((a & 0x0F) > 0x09))
			a += 0x6;
	}
	SET_FLAG(ZeroFlag, a == 0);
	SET_FLAG(HalfCarryFlag, 0);

	SET_REG(RegisterA, a);
}

void CPL()
{
	SET_REG(RegisterA, ~GET_REG(RegisterA));
	SET_FLAG(SubstractFlag, 1);
	SET_FLAG(HalfCarryFlag, 1);
}

void CCF()
{
	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, 0);
	SET_FLAG(CarryFlag, !GET_FLAG(CarryFlag));
}

void SCF()
{
	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, 0);
	SET_FLAG(CarryFlag, 1);
}
