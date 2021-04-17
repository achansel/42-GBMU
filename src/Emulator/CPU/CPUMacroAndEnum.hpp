#pragma once

#include "Emulator/Util/Bitwise.hpp"

/* Useful enums def */
enum Register {RegisterB = 0, RegisterC = 1, RegisterD = 2, RegisterE = 3, RegisterH = 4, RegisterL = 5, RegisterF = 6, RegisterA = 7};
enum ComposedRegister {RegisterBC = 0x01, RegisterDE = 0x23, RegisterHL = 0x45, RegisterAF = 0x76};
enum Flag {ZeroFlag = 7, SubstractFlag = 6, HalfCarryFlag = 5, CarryFlag = 4};
enum CallJumpFlag {NZ = 0x00, NC = 0x10, Z = 0x01, C = 0x11};

/* ALL VARIABLES REUSED THREW THE MACROS */
/* NOT ELEGANT BUT QUICK */

int carry, carrybits, result;
u8 flags;
u16 value;
u8 temp;
u8 optemp;
u8 tempread;

#define GET_REG(r) (regs[r])
#define SET_REG(r, v) \
                /*if (r != RegisterF)*/ regs[r] = (v)

#define SET_REG_BIT(r, pos, v) regs[r] ^= (-(v) ^ regs[r]) & (1U << (pos));

#define INCREMENT_REG(r) regs[r]++;
#define DECREMENT_REG(r) regs[r]--;

#define MAKE_CALL_JUMP_FLAG() \
                    (static_cast<u8>( (opcode & 0x03) << 4 | ((opcode >> 3) & 1)))

#define GET_COMPOSED_REG(r) \
                    COMPOSE_BYTES(GET_REG(r >> 4), GET_REG(r & 0x0F))

#define SET_COMPOSED_REG(r, v) \
                    value = v; \
                    SET_REG(r >> 4, static_cast<u8>(value>>8)); \
                    SET_REG(r & 0x0F, static_cast<u8>(value));

#define INCREMENT_COMPOSED_REG(r) \
                    value = GET_COMPOSED_REG(r); \
                    value++; \
                    SET_COMPOSED_REG(r, value);

#define DECREMENT_COMPOSED_REG(r) \
                    value = GET_COMPOSED_REG(r); \
                    value--; \
                    SET_COMPOSED_REG(r, value);

#define GET_DEST_REG(op) regs[op >> 3 & 7]
#define GET_SRC_REG(op) regs[op & 7]
#define GET_SRC_REG_INDEX(op) op & 7
#define GET_DEST_REG_INDEX(op) (op >> 3 & 7)

#define SET_FLAG(n, v) regs[6] ^= (-(v) ^ regs[6]) & (1U << n);
#define GET_FLAG(n) ((regs[6] >> n) & 1U)

#define NOP() \
                    PC+=1; \
                    tclock += 4;
#define STOP() \
                    PC+=1; \
                    tclock += 4;
                    //TODO: IMPLEMENT STOP

#define _LD_ADDR_8(addr, imm8) \
                    m_emu->get_MMU().set_byte_at(addr, imm8); \
                    tclock += 4; \
                    PC+=1;

#define LD_ADDR_U16_A() \
                    _LD_ADDR_8(m_emu->get_MMU().get_word_at(++PC), GET_REG(RegisterA)) \
                    PC+=1; tclock += 12;

#define LD_ADDR_BC_A() \
                    _LD_ADDR_8(GET_COMPOSED_REG(RegisterBC), GET_REG(RegisterA)) \
                    tclock += 4;

#define LD_ADDR_DE_A() \
                    _LD_ADDR_8(GET_COMPOSED_REG(RegisterBC), GET_REG(RegisterA)) \
                    tclock += 4;

#define LD_ADDR_HL_R(op) \
                    _LD_ADDR_8(GET_COMPOSED_REG(RegisterHL), GET_SRC_REG(op)) \
                    tclock += 4;


#define LD_ADDR_HLI_A() \
                    _LD_ADDR_8(GET_COMPOSED_REG(RegisterHL), GET_REG(RegisterA)) \
                    INCREMENT_COMPOSED_REG(RegisterHL) \
                    tclock += 4;



#define LD_ADDR_HLD_A() \
                    _LD_ADDR_8(GET_COMPOSED_REG(RegisterHL), GET_REG(RegisterA)) \
                    DECREMENT_COMPOSED_REG(RegisterHL) \
                    tclock += 4;

#define LD_ADDR_C_A() \
                    _LD_ADDR_8(0xFF00 + GET_REG(RegisterC), GET_REG(RegisterA)) \
                    tclock += 4; 

#define LD_FF00_IMM8_A() \
                    _LD_ADDR_8(0xFF00 + m_emu->get_MMU().get_byte_at(++PC), GET_REG(RegisterA)); \
                    tclock += 8;

#define _LD_R_8(r, imm8) \
                    temp = imm8; \
                    SET_REG(r, temp); \
                    tclock += 4; \
                    PC+=1;

#define LD_A_FF00_IMM8() \
                    _LD_R_8(RegisterA, m_emu->get_MMU().get_byte_at((0xFF00 + m_emu->get_MMU().get_byte_at(PC+1)))); \
                    tclock += 8; PC+=1;

#define LD_A_ADDR_HL() \
                    tclock += 4; \
                    _LD_R_8(RegisterA, m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterHL)));

#define LD_A_ADDR_HLI() \
                    tclock += 4; \
                    _LD_R_8(RegisterA, m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterHL))); \
                    INCREMENT_COMPOSED_REG(RegisterHL);

#define LD_A_ADDR_HLD() \
                    tclock += 4; \
                    _LD_R_8(RegisterA, m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterHL))); \
                    DECREMENT_COMPOSED_REG(RegisterHL);


#define LD_A_ADDR_BC() \
                    tclock += 4; \
                    _LD_R_8(RegisterA, m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterBC)));

#define LD_A_ADDR_DE() \
                    tclock += 4; \
                    _LD_R_8(RegisterA, m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterDE)));

#define LD_B_IMM8() \
                    tclock += 4; \
                    _LD_R_8(RegisterB, m_emu->get_MMU().get_byte_at(++PC))
#define LD_C_IMM8() \
                    tclock += 4; \
                    _LD_R_8(RegisterC, m_emu->get_MMU().get_byte_at(++PC))
#define LD_D_IMM8() \
                    tclock += 4; \
                    _LD_R_8(RegisterD, m_emu->get_MMU().get_byte_at(++PC))
#define LD_E_IMM8() \
                    tclock += 4; \
                    _LD_R_8(RegisterE, m_emu->get_MMU().get_byte_at(++PC))
#define LD_H_IMM8() \
                    tclock += 4; \
                    _LD_R_8(RegisterH, m_emu->get_MMU().get_byte_at(++PC))
#define LD_L_IMM8() \
                    tclock += 4; \
                    _LD_R_8(RegisterL, m_emu->get_MMU().get_byte_at(++PC))
#define LD_A_IMM8() \
                    tclock += 4; \
                    _LD_R_8(RegisterA, m_emu->get_MMU().get_byte_at(++PC))

#define LD_ADDR_HL_IMM8() \
                    tclock += 8; \
                    _LD_ADDR_8(GET_COMPOSED_REG(RegisterHL), m_emu->get_MMU().get_byte_at(++PC))



#define LD_R_R() \
                    _LD_R_8(GET_DEST_REG_INDEX(opcode), GET_SRC_REG(opcode))

#define _LD_RR_16(r, imm16) \
                    SET_COMPOSED_REG(r,imm16) \
                    tclock += 8; \
                    PC += 1;

#define LD_SP_IMM16() \
                    SP = m_emu->get_MMU().get_word_at(++PC); \
                    tclock += 12; \
                    PC += 2;

#define LD_DE_IMM16() \
                    _LD_RR_16(RegisterDE, m_emu->get_MMU().get_word_at(++PC)) \
                    tclock += 4; \
                    PC += 1;

#define LD_HL_IMM16() \
                    _LD_RR_16(RegisterHL, m_emu->get_MMU().get_word_at(++PC)) \
                    tclock += 4; \
                    PC += 1;

#define LD_BC_IMM16() \
                    _LD_RR_16(RegisterBC, m_emu->get_MMU().get_word_at(++PC)) \
                    tclock += 4; \
                    PC += 1;

#define LD_HL_SP_IMM8() \
                    _LD_RR_16(RegisterHL, SP + m_emu->get_MMU().get_byte_at(++PC)) \
                    tclock += 8; 

#define LD_SP_HL() \
                    _LD_RR_16(SP, HL) \
                    tclock += 4;

#define _PUSH(val) \
                    tclock += 8; \
                    SP -= 2; \
                    m_emu->get_MMU().set_word_at(SP, val); \

#define PUSH_BC() \
                    PC +=1; tclock += 8; \
                    _PUSH(GET_COMPOSED_REG(RegisterBC));


#define PUSH_DE() \
                    PC +=1; tclock += 8; \
                    _PUSH(GET_COMPOSED_REG(RegisterDE));


#define PUSH_HL() \
                    PC +=1; tclock += 8; \
                    _PUSH(GET_COMPOSED_REG(RegisterHL));


#define PUSH_AF() \
                    PC +=1; tclock += 8; \
                    _PUSH(GET_COMPOSED_REG(RegisterAF));


#define _POP(r) \
                    tclock += 8; \
                    SET_COMPOSED_REG(r, m_emu->get_MMU().get_word_at(SP)); \
                    SP += 2;

#define _POP_INTO_PC() \
                    tclock += 12; \
                    PC = m_emu->get_MMU().get_word_at(SP); \
                    SP += 2;


#define POP_BC() \
                    tclock += 4; \
                    PC += 1; \
                    _POP(RegisterBC)

#define POP_DE() \
                    tclock += 4; \
                    PC += 1; \
                    _POP(RegisterDE)

#define POP_HL() \
                    tclock += 4; \
                    PC += 1; \
                    _POP(RegisterHL)

#define POP_AF() \
                    tclock += 4; \
                    PC += 1; \
                    _POP(RegisterAF)

#define _JUMP(addr) \
                    PC = addr; \
                    tclock += 4; 

#define JUMP_A16() \
                    tclock += 12; \
                    _JUMP(m_emu->get_MMU().get_word_at(PC+1)); \
                    PC += 3;

#define JR_R8() \
                    PC += 2; \
                    tclock += 4; \
                    _JUMP(PC + m_emu->get_MMU().get_signed_byte_at(PC-1)) \

#define JR_R8_COND() \
                    tclock += 8; \
                    PC += 2; \
                    flags = MAKE_CALL_JUMP_FLAG(); \
                    if ((flags == NZ && !GET_FLAG(ZeroFlag)) || (flags == Z && GET_FLAG(ZeroFlag)) || (flags == NC && !GET_FLAG(CarryFlag)) || (flags == C && GET_FLAG(CarryFlag))) \
                        _JUMP(PC + m_emu->get_MMU().get_signed_byte_at(PC-1))

#define CALL_U16() \
                    tclock += 12; \
                    PC += 3; \
                    _PUSH(PC); \
                    _JUMP(m_emu->get_MMU().get_word_at(PC-2)); 

#define CALL_U16_COND() \
                    tclock += 12; \
                    PC += 3; \
                    flags = MAKE_CALL_JUMP_FLAG(); \
                    if ((flags == NZ && !GET_FLAG(ZeroFlag)) || (flags == Z && GET_FLAG(ZeroFlag)) || (flags == NC && !GET_FLAG(CarryFlag)) || (flags == C && GET_FLAG(CarryFlag))) \
                    { \
                        _PUSH(PC); \
                        _JUMP(m_emu->get_MMU().get_word_at(PC-2)); \
                    }

#define RET() \
                    tclock += 4; \
                    _POP_INTO_PC();

#define RET_COND() \
                    tclock += 8; PC += 1; \
                    flags = MAKE_CALL_JUMP_FLAG(); \
                    if ((flags == NZ && !GET_FLAG(ZeroFlag)) || (flags == Z && GET_FLAG(ZeroFlag)) || (flags == NC && !GET_FLAG(CarryFlag)) || (flags == C && GET_FLAG(CarryFlag))) \
                    { \
                        _POP_INTO_PC(); \
                    }

#define _AND(v) \
                    SET_REG(RegisterA, v & GET_REG(RegisterA)); \
                    SET_FLAG(ZeroFlag, !!GET_REG(RegisterA)); \
                    SET_FLAG(SubstractFlag, 0); \
                    SET_FLAG(HalfCarryFlag, 1); \
                    SET_FLAG(CarryFlag, 0); \
                    tclock += 4; \
                    PC += 1;

#define AND_R() \
                    _AND(GET_SRC_REG(opcode))

#define AND_IMM8() \
                    tclock += 4; \
                    _AND(m_emu->get_MMU().get_byte_at(++PC))

#define AND_ADDR_HL() \
                    tclock += 4; \
                    _AND(m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterHL)))

#define _XOR(v) \
                    SET_REG(RegisterA, v ^ GET_REG(RegisterA)); \
                    SET_FLAG(ZeroFlag, !!GET_REG(RegisterA)); \
                    SET_FLAG(SubstractFlag, 0); \
                    SET_FLAG(HalfCarryFlag, 0); \
                    SET_FLAG(CarryFlag, 0); \
                    tclock += 4; \
                    PC += 1;

#define XOR_R() \
                    _XOR(GET_SRC_REG(opcode))

#define XOR_IMM8() \
                    tclock += 4; \
                    _XOR(m_emu->get_MMU().get_byte_at(++PC))

#define XOR_ADDR_HL() \
                    tclock += 4; \
                    _XOR(m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterHL)))

#define _OR(v) \
                    SET_REG(RegisterA, v | GET_REG(RegisterA)); \
                    SET_FLAG(ZeroFlag, !!GET_REG(RegisterA)); \
                    SET_FLAG(SubstractFlag, 0); \
                    SET_FLAG(HalfCarryFlag, 0); \
                    SET_FLAG(CarryFlag, 0); \
                    tclock += 4; \
                    PC += 1;

#define OR_R() \
                    _OR(GET_SRC_REG(opcode))

#define OR_IMM8() \
                    tclock += 4; \
                    _OR(m_emu->get_MMU().get_byte_at(++PC))

#define OR_ADDR_HL() \
                    tclock += 4; \
                    _OR(m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterHL)))

#define _CP(v) \
                    value = v; \
                    SET_FLAG(ZeroFlag, GET_REG(RegisterA) == value); \
                    SET_FLAG(SubstractFlag, 1); \
                    SET_FLAG(HalfCarryFlag, (((GET_REG(RegisterA) - value) & 0xF) > (GET_REG(RegisterA) & 0xF))); \
                    SET_FLAG(CarryFlag, GET_REG(RegisterA) < value); \
                    tclock += 4; \
                    PC += 1;

#define CP_R() \
                    _CP(GET_SRC_REG(opcode))

#define CP_IMM8()  \
                    tclock += 4; \
                    _CP(m_emu->get_MMU().get_byte_at(++PC))

#define CP_ADDR_HL() \
                    tclock += 4; \
                    _CP(m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterHL)))

#define _ADD_A(v) \
                    result = GET_REG(RegisterA) + v; \
                    carrybits = GET_REG(RegisterA) ^ v ^ result; \
                    SET_REG(RegisterA, static_cast<u8>(result)); \
                    SET_FLAG(ZeroFlag, !!result); \
                    SET_FLAG(SubstractFlag, 0); \
                    SET_FLAG(HalfCarryFlag, !!(carrybits & 0x10)); \
                    SET_FLAG(CarryFlag, !!(carrybits & 0x100)); \
                    tclock += 4; \
                    PC += 1;

#define _ADD_HL_RR(src) \
                    result = GET_COMPOSED_REG(RegisterHL) + src; \
                    carrybits = GET_COMPOSED_REG(RegisterHL) ^ src ^ result; \
                    SET_COMPOSED_REG(RegisterHL, static_cast<u16>(result)); \
                    SET_FLAG(SubstractFlag, 0); \
                    SET_FLAG(HalfCarryFlag, !!(carrybits & 0x100)); \
                    SET_FLAG(CarryFlag, !!(carrybits & 0x10000)); \
                    tclock += 8; \
                    PC += 1;


#define ADD_HL_BC() _ADD_HL_RR(GET_COMPOSED_REG(RegisterBC))
#define ADD_HL_DE() _ADD_HL_RR(GET_COMPOSED_REG(RegisterDE))
#define ADD_HL_HL() _ADD_HL_RR(GET_COMPOSED_REG(RegisterHL))
#define ADD_HL_SP() _ADD_HL_RR(GET_COMPOSED_REG(SP))


#define ADD_A_R() \
                    _ADD_A(GET_SRC_REG(opcode))

#define ADD_A_IMM8() \
                    tclock += 4; \
                    _ADD_A(m_emu->get_MMU().get_byte_at(++PC))

#define ADD_A_ADDR_HL() \
                    tclock += 4; \
                    _ADD_A(m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterHL)))

#define _ADC_A(v) \
                    carry = GET_FLAG(CarryFlag); \
                    result = GET_REG(RegisterA) + v + carry; \
                    SET_FLAG(ZeroFlag, !!result); \
                    SET_FLAG(SubstractFlag, 0); \
                    SET_FLAG(HalfCarryFlag, ((GET_REG(RegisterA) & 0xF) + (v & 0xF) + carry) > 0xF); \
                    SET_FLAG(CarryFlag, result > 0xFF); \
                    SET_REG(RegisterA, static_cast<u8>(result)); \
                    tclock += 4; \
                    PC += 1;

#define ADC_A_R() \
                    _ADC_A(GET_SRC_REG(opcode))

#define ADC_A_IMM8() \
                    tclock += 4; \
                    _ADC_A(m_emu->get_MMU().get_byte_at(++PC))

#define ADC_A_ADDR_HL() \
                    tclock += 4; \
                    _ADC_A(m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterHL)))

#define _SUB_A(v) \
                    result = GET_REG(RegisterA) - v; \
                    carrybits = GET_REG(RegisterA) ^ v ^ result; \
                    SET_REG(RegisterA, static_cast<u8>(result)); \
                    SET_FLAG(ZeroFlag, !!result); \
                    SET_FLAG(SubstractFlag, 1); \
                    SET_FLAG(HalfCarryFlag, !!(carrybits & 0x10)); \
                    SET_FLAG(CarryFlag, !!(carrybits & 0x100)); \
                    tclock += 4; PC += 1;

#define SUB_A_R() \
                    _SUB_A(GET_SRC_REG(opcode))

#define SUB_A_IMM8() \
                    tclock += 4; \
                    _SUB_A(m_emu->get_MMU().get_byte_at(++PC))

#define SUB_A_ADDR_HL() \
                    tclock += 4; \
                    _SUB_A(m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterHL)))

#define _SBC_A(v) \
                    carry = GET_FLAG(CarryFlag); \
                    result = GET_REG(RegisterA) - v - carry; \
                    SET_FLAG(ZeroFlag, !!result); \
                    SET_FLAG(SubstractFlag, 1); \
                    SET_FLAG(HalfCarryFlag, ((GET_REG(RegisterA) & 0xF) - (v & 0xF) - carry) < 0); \
                    SET_FLAG(CarryFlag, result < 0); \
                    SET_REG(RegisterA, static_cast<u8>(result)); \
                    tclock += 4; PC+=1;

#define SBC_A_R() \
                    _SBC_A(GET_SRC_REG(opcode))

#define SBC_A_IMM8() \
                    tclock += 4; \
                    _SBC_A(m_emu->get_MMU().get_byte_at(++PC))

#define SBC_A_ADDR_HL() \
                    tclock += 4; \
                    _SBC_A(m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterHL)))


#define DEC_BC() \
                    tclock += 8; \
                    PC += 1; \
                    DECREMENT_COMPOSED_REG(RegisterBC)
                    
#define DEC_DE() \
                    tclock += 8; \
                    PC += 1; \
                    DECREMENT_COMPOSED_REG(RegisterDE)
                    
#define DEC_HL() \
                    tclock += 8; \
                    PC += 1; \
                    DECREMENT_COMPOSED_REG(RegisterHL)
                    
#define DEC_SP() \
                    tclock += 8; \
                    PC += 1; \
                    SP--;

#define _DEC_R(reg) \
                    tclock += 4; \
                    PC += 1; \
                    DECREMENT_REG(reg) \
                    SET_FLAG(HalfCarryFlag, !(GET_REG(reg) & 0x0F)); \
                    SET_FLAG(ZeroFlag, !GET_REG(reg)); \
                    SET_FLAG(SubstractFlag, 1);

#define DEC_B() \
                    _DEC_R(RegisterB)
                    
#define DEC_C() \
                    _DEC_R(RegisterC)

#define DEC_D() \
                    _DEC_R(RegisterD)

#define DEC_E() \
                    _DEC_R(RegisterE)

#define DEC_H() \
                    _DEC_R(RegisterH)

#define DEC_L() \
                    _DEC_R(RegisterL)

#define DEC_A() \
                    _DEC_R(RegisterA)

#define DEC_ADDR_HL() \
                    tclock += 12; \
                    PC += 1; \
                    value = GET_COMPOSED_REG(RegisterHL); \
                    temp = m_emu->get_MMU().get_byte_at(value); \
                    temp--; \
                    SET_FLAG(HalfCarryFlag, !(temp & 0x0F)); \
                    SET_FLAG(ZeroFlag, !temp); \
                    SET_FLAG(SubstractFlag, 1); \
                    m_emu->get_MMU().set_byte_at(value, temp);

#define INC_BC() \
                    tclock += 8; \
                    PC += 1; \
                    INCREMENT_COMPOSED_REG(RegisterBC)
                    
#define INC_DE() \
                    tclock += 8; \
                    PC += 1; \
                    INCREMENT_COMPOSED_REG(RegisterDE)
                    
#define INC_HL() \
                    tclock += 8; \
                    PC += 1; \
                    INCREMENT_COMPOSED_REG(RegisterHL)
                    
#define INC_SP() \
                    tclock += 8; \
                    PC += 1; \
                    SP++;

#define _INC_R(reg) \
                    tclock += 4; \
                    PC += 1; \
                    INCREMENT_REG(reg) \
                    SET_FLAG(HalfCarryFlag, !(GET_REG(reg) & 0x0F)); \
                    SET_FLAG(ZeroFlag, !GET_REG(reg)); \
                    SET_FLAG(SubstractFlag, 0);

#define INC_B() \
                    _INC_R(RegisterB)
                    
#define INC_C() \
                    _INC_R(RegisterC)

#define INC_D() \
                    _INC_R(RegisterD)

#define INC_E() \
                    _INC_R(RegisterE)

#define INC_H() \
                    _INC_R(RegisterH)

#define INC_L() \
                    _INC_R(RegisterL)

#define INC_A() \
                    _INC_R(RegisterA)

#define INC_ADDR_HL() \
                    tclock += 12;\
                    PC += 1; \
                    value = GET_COMPOSED_REG(RegisterHL); \
                    temp = m_emu->get_MMU().get_byte_at(value); \
                    temp++; \
                    SET_FLAG(HalfCarryFlag, !(temp & 0x0F)); \
                    SET_FLAG(ZeroFlag, !temp); \
                    SET_FLAG(SubstractFlag, 0); \
                    m_emu->get_MMU().set_byte_at(value, temp);


#define RLCA() \
                    tclock += 4; \
                    PC += 1; \
                    temp = GET_REG(RegisterA); \
                    flags = !!(temp & 0x80); \
                    SET_FLAG(SubstractFlag, 0) \
                    SET_FLAG(HalfCarryFlag, 0) \
                    SET_FLAG(CarryFlag, flags) \
                    temp <<= 1; \
                    temp |= flags; \
                    SET_REG(RegisterA, temp); \
                    SET_FLAG(ZeroFlag, !temp);

#define RRCA() \
                    tclock += 4; \
                    PC += 1; \
                    temp = GET_REG(RegisterA); \
                    flags = !!(temp & 0x01); \
                    SET_FLAG(SubstractFlag, 0) \
                    SET_FLAG(HalfCarryFlag, 0) \
                    SET_FLAG(CarryFlag, flags) \
                    temp >>= 1; \
                    temp |= flags; \
                    SET_REG(RegisterA, temp); \
                    SET_FLAG(ZeroFlag, !temp);

#define RLA() \
                    carry = GET_FLAG(CarryFlag); \
                    tclock += 4; \
                    PC += 1; \
                    temp = GET_REG(RegisterA); \
                    SET_FLAG(CarryFlag, !!(temp & 0x80)); \
                    SET_FLAG(SubstractFlag, 0) \
                    SET_FLAG(HalfCarryFlag, 0) \
                    temp <<= 1; \
                    temp |= carry; \
                    SET_REG(RegisterA, temp); \
                    SET_FLAG(ZeroFlag, !temp);


#define RRA() \
                    carry = GET_FLAG(CarryFlag); \
                    tclock += 4; \
                    PC += 1; \
                    temp = GET_REG(RegisterA); \
                    SET_FLAG(CarryFlag, !!(temp & 0x01)); \
                    SET_FLAG(SubstractFlag, 0) \
                    SET_FLAG(HalfCarryFlag, 0) \
                    temp >>= 1; \
                    temp |= carry; \
                    SET_REG(RegisterA, temp); \
                    SET_FLAG(ZeroFlag, !temp);


#define CCF() \
		SET_FLAG(CarryFlag, !GET_FLAG(CarryFlag)); \
		SET_FLAG(SubstractFlag, 0); \
		SET_FLAG(HalfCarryFlag, 0); \
		PC += 1; \
		tclock += 4;

#define SCF() \
		SET_FLAG(CarryFlag, 1); \
		SET_FLAG(SubstractFlag, 0); \
		SET_FLAG(HalfCarryFlag, 0); \
		PC += 1; \
		tclock += 4;

#define CPL() \
		SET_REG(RegisterA, ~GET_REG(RegisterA)); \
		SET_FLAG(HalfCarryFlag, 1); \
		SET_FLAG(SubstractFlag, 1); \
		PC += 1; \
		tclock += 4;

/*********************************************************************/
/*                              OPCODES CB                           */
/*********************************************************************/
// FINISHED
// SHOULDNT HAVE BUGS
// TODO: CHECK

#define RLC_R(op) \
                    tclock += 8; \
                    PC += 2; \
                    temp = GET_SRC_REG(op); \
                    flags = !!(temp & 0x80); \
                    SET_FLAG(SubstractFlag, 0) \
                    SET_FLAG(HalfCarryFlag, 0) \
                    SET_FLAG(CarryFlag, flags) \
                    temp <<= 1; \
                    temp |= flags; \
                    SET_REG(GET_SRC_REG_INDEX(op), temp); \
                    SET_FLAG(ZeroFlag, !temp);

#define RLC_ADDR_HL() \
                    value = GET_COMPOSED_REG(RegisterHL); \
                    tclock += 16; \
                    PC += 2; \
                    temp = m_emu->get_MMU().get_byte_at(value); \
                    flags = !!(temp & 0x80); \
                    SET_FLAG(SubstractFlag, 0) \
                    SET_FLAG(HalfCarryFlag, 0) \
                    SET_FLAG(CarryFlag, flags) \
                    temp <<= 1; \
                    temp |= flags; \
                    m_emu->get_MMU().set_byte_at(value, temp); \
                    SET_FLAG(ZeroFlag, !temp);


#define RRC_R(op) \
                    tclock += 8; \
                    PC += 2; \
                    temp = GET_SRC_REG(op); \
                    flags = !!(temp & 0x01); \
                    SET_FLAG(SubstractFlag, 0) \
                    SET_FLAG(HalfCarryFlag, 0) \
                    SET_FLAG(CarryFlag, flags) \
                    temp >>= 1; \
                    temp |= flags; \
                    SET_REG(GET_SRC_REG_INDEX(op), temp); \
                    SET_FLAG(ZeroFlag, !temp);

#define RRC_ADDR_HL() \
                    value = GET_COMPOSED_REG(RegisterHL); \
                    tclock += 16; \
                    PC += 2; \
                    temp = m_emu->get_MMU().get_byte_at(value); \
                    flags = !!(temp & 0x01); \
                    SET_FLAG(SubstractFlag, 0) \
                    SET_FLAG(HalfCarryFlag, 0) \
                    SET_FLAG(CarryFlag, flags) \
                    temp >>= 1; \
                    temp |= flags; \
                    m_emu->get_MMU().set_byte_at(value, temp); \
                    SET_FLAG(ZeroFlag, !temp);

#define RL_R(op) \
                    carry = GET_FLAG(CarryFlag); \
                    tclock += 8; \
                    PC += 2; \
                    temp = GET_SRC_REG(op); \
                    SET_FLAG(CarryFlag, !!(temp & 0x80)); \
                    SET_FLAG(SubstractFlag, 0) \
                    SET_FLAG(HalfCarryFlag, 0) \
                    temp <<= 1; \
                    temp |= carry; \
                    SET_REG(GET_SRC_REG_INDEX(op), temp); \
                    SET_FLAG(ZeroFlag, !temp);

#define RL_ADDR_HL() \
                    value = GET_COMPOSED_REG(RegisterHL); \
                    carry = GET_FLAG(CarryFlag); \
                    tclock += 16; \
                    PC += 2; \
                    temp = m_emu->get_MMU().get_byte_at(value); \
                    SET_FLAG(CarryFlag, !!(temp & 0x80)); \
                    SET_FLAG(SubstractFlag, 0) \
                    SET_FLAG(HalfCarryFlag, 0) \
                    temp <<= 1; \
                    temp |= carry; \
                    m_emu->get_MMU().set_byte_at(value, temp); \
                    SET_FLAG(ZeroFlag, !temp);


#define RR_R(op) \
                    carry = GET_FLAG(CarryFlag); \
                    tclock += 8; \
                    PC += 2; \
                    temp = GET_SRC_REG(op); \
                    SET_FLAG(CarryFlag, !!(temp & 0x01)); \
                    SET_FLAG(SubstractFlag, 0) \
                    SET_FLAG(HalfCarryFlag, 0) \
                    temp >>= 1; \
                    temp |= carry; \
                    SET_REG(GET_SRC_REG_INDEX(op), temp); \
                    SET_FLAG(ZeroFlag, !temp);

#define RR_ADDR_HL() \
                    value = GET_COMPOSED_REG(RegisterHL); \
                    carry = GET_FLAG(CarryFlag); \
                    tclock += 16; \
                    PC += 2; \
                    temp = m_emu->get_MMU().get_byte_at(value); \
                    SET_FLAG(CarryFlag, !!(temp & 0x01)); \
                    SET_FLAG(SubstractFlag, 0) \
                    SET_FLAG(HalfCarryFlag, 0) \
                    temp >>= 1; \
                    temp |= carry; \
                    m_emu->get_MMU().set_byte_at(value, temp); \
                    SET_FLAG(ZeroFlag, !temp);

#define SLA_R(op) \
                tclock += 8;\
                PC += 2;\
                SET_FLAG(CarryFlag, !!(GET_SRC_REG(op) & 0x80)); \
                SET_FLAG(SubstractFlag, 0); \
                SET_FLAG(HalfCarryFlag, 0); \
                temp = GET_SRC_REG(op) << 1; \
                SET_REG(GET_SRC_REG_INDEX(op), temp); \
                SET_FLAG(ZeroFlag, !temp);

#define SLA_ADDR_HL() \
                value = GET_COMPOSED_REG(RegisterHL); \
                tclock += 16;\
                PC += 2;\
                SET_FLAG(CarryFlag, !!(m_emu->get_MMU().get_byte_at(value) & 0x80)); \
                SET_FLAG(SubstractFlag, 0); \
                SET_FLAG(HalfCarryFlag, 0); \
                temp = m_emu->get_MMU().get_byte_at(value) << 1; \
                m_emu->get_MMU().set_byte_at(value, temp); \
                SET_FLAG(ZeroFlag, !temp);

#define SRA_R(op) \
                tclock += 8;\
                PC += 2;\
                temp = GET_SRC_REG(op); \
                SET_FLAG(CarryFlag, !!(GET_SRC_REG(op) & 0x01)); \
                SET_FLAG(SubstractFlag, 0); \
                SET_FLAG(HalfCarryFlag, 0); \
                temp >>= 1; \
                temp |= !!(temp & 0x80) ? 0x80 : 0; \
                SET_REG(GET_SRC_REG_INDEX(op), temp); \
                SET_FLAG(ZeroFlag, !temp);

#define SRA_ADDR_HL() \
                value = GET_COMPOSED_REG(RegisterHL); \
                tclock += 8;\
                PC += 2;\
                temp = m_emu->get_MMU().get_byte_at(value); \
                SET_FLAG(CarryFlag, !!(temp & 0x01)); \
                SET_FLAG(SubstractFlag, 0); \
                SET_FLAG(HalfCarryFlag, 0); \
                temp >>= 1; \
                temp |= !!(temp & 0x80) ? 0x80 : 0; \
                m_emu->get_MMU().set_byte_at(value, temp); \
                SET_FLAG(ZeroFlag, !temp);

#define GET_TARGET_BIT(base_op, opcode) \
                (opcode - base_op) >> 3

#define SWAP_R(op) \
                temp = GET_SRC_REG(op) & 0x0F; \
                optemp = (GET_SRC_REG(op) >> 4) & 0x0F; \
                SET_REG(GET_SRC_REG_INDEX(op), (temp << 4) + optemp); \
                SET_FLAG(ZeroFlag, !(GET_SRC_REG(op))); \
                SET_FLAG(SubstractFlag, 0); \
                SET_FLAG(HalfCarryFlag, 0); \
                SET_FLAG(CarryFlag, 0); \
                PC += 2; tclock += 8;

#define SWAP_ADDR_HL() \
                value = GET_COMPOSED_REG(RegisterHL); \
                tempread = m_emu->get_MMU().get_byte_at(value); \
                temp = tempread & 0x0F; \
                optemp = (tempread >> 4) & 0x0F; \
                m_emu->get_MMU().set_byte_at(value, (temp << 4) + optemp); \
                SET_FLAG(ZeroFlag, !(m_emu->get_MMU().get_byte_at(value))); \
                SET_FLAG(SubstractFlag, 0); \
                SET_FLAG(HalfCarryFlag, 0); \
                SET_FLAG(CarryFlag, 0); \
                PC += 2; tclock += 16;

#define SRL_R(op) \
                temp = GET_SRC_REG(op); \
                SET_FLAG(CarryFlag, !!(temp & 0x01)); \
                temp >>= 1; \
                SET_REG(GET_SRC_REG_INDEX(op), temp); \
                SET_FLAG(ZeroFlag, !temp); \
                SET_FLAG(SubstractFlag, 0); \
                SET_FLAG(HalfCarryFlag, 0); \
                PC += 2; tclock += 8;

#define SRL_ADDR_HL() \
                value = GET_COMPOSED_REG(RegisterHL); \
                temp = m_emu->get_MMU().get_byte_at(value); \
                SET_FLAG(CarryFlag, !!(temp & 0x01)); \
                temp >>= 1; \
                m_emu->get_MMU().set_byte_at(value, temp); \
                SET_FLAG(ZeroFlag, !temp); \
                SET_FLAG(SubstractFlag, 0); \
                SET_FLAG(HalfCarryFlag, 0); \
                PC += 2; tclock += 16;

#define _BIT_N(tb, imm8) \
                PC += 2; tclock += 8; \
                optemp = tb; \
                temp = imm8; \
                SET_FLAG(ZeroFlag, !CHECK_BIT(temp, optemp)); \
                SET_FLAG(SubstractFlag, 0); \
                SET_FLAG(HalfCarryFlag, 1);

#define BIT_N_R(op) \
                _BIT_N(GET_TARGET_BIT(0x40, op), GET_SRC_REG(op));

#define BIT_N_ADDR_HL(op) \
                tclock += 8; \
                _BIT_N(GET_TARGET_BIT(0x40, op), m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterHL)));

#define RES_N_R(op) \
                tclock += 8; PC += 2; \
                SET_REG_BIT(GET_SRC_REG_INDEX(op), GET_TARGET_BIT(0x80, op), 0)

#define RES_N_ADDR_HL(op) \
                tclock += 16; PC += 2; \
                temp = m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterHL)); \
                SET_BIT(temp, GET_TARGET_BIT(0x80, op), 1); \
                m_emu->get_MMU().set_byte_at(GET_COMPOSED_REG(RegisterHL), temp); 

#define SET_N_R(op) \
                tclock += 8; PC += 2; \
                SET_REG_BIT(GET_SRC_REG_INDEX(op), GET_TARGET_BIT(0x80, op), 1)

#define SET_N_ADDR_HL(op) \
                tclock += 16; PC += 2; \
                temp = m_emu->get_MMU().get_byte_at(GET_COMPOSED_REG(RegisterHL)); \
                SET_BIT(temp, GET_TARGET_BIT(0x80, op), 1); \
                m_emu->get_MMU().set_byte_at(GET_COMPOSED_REG(RegisterHL), temp);
