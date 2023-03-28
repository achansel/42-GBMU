template<bool main_instruction_set = false>
u8 RLC(u8 operand)
{
	u8 a = operand;
	a = (a << 1) | (a >> 7);

	if constexpr (main_instruction_set)
		SET_FLAG(ZeroFlag, 0);
	else
		SET_FLAG(ZeroFlag, a == 0);
	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, 0);
	SET_FLAG(CarryFlag, (a >> 0) & 1);

	return (a);
}

template<bool main_instruction_set = false>
u8 RRC(u8 operand)
{
	u8 a = operand;
	a = (a >> 1) | (a << 7);

	if constexpr (main_instruction_set)
		SET_FLAG(ZeroFlag, 0);
	else
		SET_FLAG(ZeroFlag, a == 0);
	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, 0);
	SET_FLAG(CarryFlag, (a >> 7) & 1); //maybe wrong swap & 1 and & 0x80

	return (a);
}

template<bool main_instruction_set = false>
u8 RL(u8 operand)
{
	u8 a = operand;
	a = (a << 1) | (GET_FLAG(CarryFlag) << 0);

	if constexpr (main_instruction_set)
		SET_FLAG(ZeroFlag, 0);
	else
		SET_FLAG(ZeroFlag, a == 0);
	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, 0);
	SET_FLAG(CarryFlag, (operand >> 7) & 1); //maybe wrong swap & 1 and & 0x80

	return (a);
}

template<bool main_instruction_set = false>
u8 RR(u8 operand)
{
	u8 a = operand;
	a = (a >> 1) | (GET_FLAG(CarryFlag) << 7);

	if constexpr (main_instruction_set)
		SET_FLAG(ZeroFlag, 0);
	else
		SET_FLAG(ZeroFlag, a == 0);
	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, 0);
	SET_FLAG(CarryFlag, operand & 1); //maybe wrong swap & 1 and & 0x80

	return (a);
}

void RRA(void)	{ SET_REG(RegisterA, RR<true>(GET_REG(RegisterA)));  }
void RLA(void)	{ SET_REG(RegisterA, RL<true>(GET_REG(RegisterA)));  }
void RRCA(void) { SET_REG(RegisterA, RRC<true>(GET_REG(RegisterA))); }
void RLCA(void) { SET_REG(RegisterA, RLC<true>(GET_REG(RegisterA))); }

u8 SRA(u8 operand)
{
	u8 a = operand;
	a = (a >> 1) | (a & 0x80);

	SET_FLAG(ZeroFlag, a == 0);
	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, 0);
	SET_FLAG(CarryFlag, operand & 0x01);

	return a;
}

u8 SLA(u8 operand)
{
	u8 a = operand;
	a <<= 1;

	SET_FLAG(ZeroFlag, a == 0);
	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, 0);
	SET_FLAG(CarryFlag, (operand >> 7) & 0x01);

	return a;
}

u8 SWAP(u8 operand)
{
	u8 a = operand;
	a = (a >> 4) | ((a & 0xf) << 4);

	SET_FLAG(ZeroFlag, a == 0);
	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, 0);
	SET_FLAG(CarryFlag, 0);

	return a;
}


u8 SRL(u8 operand)
{
	u8 a = operand;
	a >>= 1;

	SET_FLAG(ZeroFlag, a == 0);
	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, 0);
	SET_FLAG(CarryFlag, operand & 0x01);

	return a;
}

/* Technically there is a loss of performance on those 3, since they break the rule of doing most
of the stuff at compile time. Need to investigate if its relevant */
void	BIT(u8 operand)
{
	u8 bit = GET_TARGET_BIT(0x40, m_opcode);

	SET_FLAG(ZeroFlag, !((operand >> bit) & 1));
	SET_FLAG(SubstractFlag, 0);
	SET_FLAG(HalfCarryFlag, 1);
}

u8		RES(u8 operand)
{
	u8 bit = GET_TARGET_BIT(0x80, m_opcode);

	return (operand & ~(1 << bit));
}

u8		SET(u8 operand)
{
	u8 bit = GET_TARGET_BIT(0xC0, m_opcode);

	return (operand | (1 << bit));
}


enum class PREFIXEDOP {
	RLC, RRC, RL, RR, SLA, SRA, SWAP, SRL, BIT, RES, SET
};

template<PREFIXEDOP op>
ALWAYS_INLINE auto PREFIXED_OP(u8 operand)
{
	if constexpr (op == PREFIXEDOP::RLC)
		return RLC(operand);
	else if constexpr (op == PREFIXEDOP::RRC)
		return RRC(operand);
	else if constexpr (op == PREFIXEDOP::RL)
		return RL(operand);
	else if constexpr (op == PREFIXEDOP::RR)
		return RR(operand);
	else if constexpr (op == PREFIXEDOP::SLA)
		return SLA(operand);
	else if constexpr (op == PREFIXEDOP::SRA)
		return SRA(operand);
	else if constexpr (op == PREFIXEDOP::SWAP)
		return SWAP(operand);
	else if constexpr (op == PREFIXEDOP::SRL)
		return SRL(operand);
	else if constexpr (op == PREFIXEDOP::RES)
		return RES(operand);
	else if constexpr (op == PREFIXEDOP::SET)
		return SET(operand);
	else
		BIT(operand);

}

template<PREFIXEDOP op, Register r>
void PREFIXED_OP_REG()
{
	if constexpr (op != PREFIXEDOP::BIT)
		SET_REG(r, PREFIXED_OP<op>(GET_REG(r)));
	else
		PREFIXED_OP<op>(GET_REG(r));
}

template<PREFIXEDOP op>
void PREFIXED_OP_ADDR_HL()
{
	u16 HL = GET_COMPOSED_REG(RegisterHL);
	if constexpr (op != PREFIXEDOP::BIT)
		WRITE_BYTE(HL, PREFIXED_OP<op>(GET_BYTE(HL)));
	else
		PREFIXED_OP<op>(GET_BYTE(HL));
}

template<PREFIXEDOP op>
void PREFIXED_OP_DEDUCE()
{
	Register reg = static_cast<Register>(m_opcode & 7);

	if (reg == RegisterF)
	{
		u16 HL = GET_COMPOSED_REG(RegisterHL);
		if constexpr (op != PREFIXEDOP::BIT)
			WRITE_BYTE(HL, PREFIXED_OP<op>(GET_BYTE(HL)));
		else
			PREFIXED_OP<op>(GET_BYTE(HL));

	}
	else
	{
		if constexpr (op != PREFIXEDOP::BIT)
			SET_REG(reg, PREFIXED_OP<op>(GET_REG(reg)));
		else
			PREFIXED_OP<op>(GET_REG(reg));

	}
}