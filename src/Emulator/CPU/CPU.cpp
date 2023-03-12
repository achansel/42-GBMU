#include <Emulator/CPU/CPU.hpp>
#include <Emulator/Emulator.hpp>

CPU::CPU(Emulator* emu)
{
    m_emu = emu;
    regs[7] = 1;
}

void CPU::saveafterinstruction()
{
    if (savestate == nullptr)
        savestate = fopen("states.bin", "wb");
    fwrite((void*)regs, sizeof(u8), 10, savestate);
    fwrite((void*)&PC, sizeof(u16), 1, savestate);
}

void CPU::tick()
{
    execute_next_instruction();
    //saveafterinstruction();
    step_lcd();
}

void CPU::debug_stop()
{
	#ifndef NDEBUG
		std::cout << "GBMU: CPU FATAL:" << std::endl;
		u16 SP = GET_COMPOSED_REG(RegisterSP);
		printf("\tREGS: A: 0x%02X, B:0x%02X, C:0x%02X, D:0x%02X, E:0x%02X, H:0x%02X, L:0x%02X, F:0x%02X\n\t\tSP:0x%04X, PC:0x%04X\n", GET_REG(RegisterA), GET_REG(RegisterB), GET_REG(RegisterC), GET_REG(RegisterD), GET_REG(RegisterE), GET_REG(RegisterH), GET_REG(RegisterL), GET_REG(RegisterF), SP, PC);
		printf("\tHALTED OPCODE (could be wrong lol) (NOT XCUTED): %02X\n", m_emu->get_MMU().get_byte_at(PC));
		std::cout << "\tat " << std::hex << SP << ": " << m_emu->get_MMU().get_word_at(SP) << std::endl;
		std::cout << "\tat " << std::hex << SP + 2 << ": " << m_emu->get_MMU().get_word_at(SP + 2) << std::endl;
		std::cout << "\tat " << std::hex << SP + 4 << ": " << m_emu->get_MMU().get_word_at(SP + 4) << std::endl;
		std::cout << "\tat " << std::hex << SP + 6 << ": " << m_emu->get_MMU().get_word_at(SP + 6) << std::endl;
		std::cout << "\tat " << std::hex << SP + 8 << ": " << m_emu->get_MMU().get_word_at(SP + 8) << std::endl;
		std::cout << "\tat " << std::hex << SP + 10 << ": " << m_emu->get_MMU().get_word_at(SP + 10) << std::endl;
		std::cout << "\tat " << std::hex << SP + 12 << ": " << m_emu->get_MMU().get_word_at(SP + 12) << std::endl;

		exit(1);
	#endif

}

inline void CPU::step_lcd()
{
    m_emu->get_lcd().update(m_tclock);
}

inline void CPU::execute_next_instruction()
{
    // Fetch the instruction
    u8 opcode = m_emu->get_MMU().get_byte_at(PC);

    // Extended opcode table
    if (opcode == 0xCB)
    {
        opcode = m_emu->get_MMU().get_byte_at(PC+1);
    
        switch (opcode)
        {
            /* RLC R or RLC (RR) */
            case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x07:
                RLC_R(opcode)
                break;
            case 0x06:
                RLC_ADDR_HL()
                break;

            /* RRC R or RRC (RR) */
            case 0x08: case 0x09: case 0x0A: case 0x0B: case 0x0C: case 0x0D: case 0x0F:
                RRC_R(opcode)
                break;
            case 0x0E:
                RRC_ADDR_HL()
                break;

            /* RL R or RL (RR) */
            case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x17:
                RL_R(opcode)
                break;
            case 0x16:
                RL_ADDR_HL()
                break;
            
            /* RR R or RR (RR) */
            case 0x18: case 0x19: case 0x1A: case 0x1B: case 0x1C: case 0x1D: case 0x1F:
                RR_R(opcode)
                break;
            case 0x1E:
                RR_ADDR_HL()
                break;
            
            /* SLA R or SLA (RR) */
            case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x27:
                SLA_R(opcode);
                break;
            case 0x26:
                SLA_ADDR_HL();
                break;

            /* SRA R or SRA (RR) */
            case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D: case 0x2F:
                SRA_R(opcode);
                break;
            case 0x2E:
                SRA_ADDR_HL();
                break;

            /* SWAP R or SWAP (RR) */
            case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x37:
                SWAP_R(opcode)
                break;
            case 0x36:
                SWAP_ADDR_HL();
                break;

            /* SRL R or SRL (RR) */
            case 0x38: case 0x39: case 0x3A: case 0x3B: case 0x3C: case 0x3D: case 0x3F:
                SRL_R(opcode);
                break;
            case 0x3E:
                SRL_ADDR_HL();
                break;

            /* BIT n, R or BIT n, (RR) */
            case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x47:
            case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4f:
            case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x57:
            case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5f:
            case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x67:
            case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6f:
            case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x77:
            case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7f:
                BIT_N_R(opcode);
                break;

            case 0x46: case 0x4E: case 0x56: case 0x5E: case 0x66: case 0x6E: case 0x76:
            case 0x7E:
                BIT_N_ADDR_HL(opcode);
                break;

            /* RES n, R or RES n, (RR) */
            case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x87:
            case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8f:
            case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x97:
            case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9f:
            case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA7:
            case 0xA8: case 0xA9: case 0xAa: case 0xAb: case 0xAc: case 0xAd: case 0xAf:
            case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB7:
            case 0xB8: case 0xB9: case 0xBa: case 0xBb: case 0xBc: case 0xBd: case 0xBf:
                RES_N_R(opcode);
                break;
            case 0x86: case 0x8E: case 0x96: case 0x9E: case 0xA6: case 0xAE: case 0xB6:
            case 0xBE:
                RES_N_ADDR_HL(opcode);
                break;

            /* SET n, R or SET n, (RR) */
            case 0xC0: case 0xC1: case 0xC2: case 0xC3: case 0xC4: case 0xC5: case 0xC7:
            case 0xC8: case 0xC9: case 0xCa: case 0xCb: case 0xCc: case 0xCd: case 0xCf:
            case 0xD0: case 0xD1: case 0xD2: case 0xD3: case 0xD4: case 0xD5: case 0xD7:
            case 0xD8: case 0xD9: case 0xDa: case 0xDb: case 0xDc: case 0xDd: case 0xDf:
            case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE4: case 0xE5: case 0xE7:
            case 0xE8: case 0xE9: case 0xEa: case 0xEb: case 0xEc: case 0xEd: case 0xEf:
            case 0xF0: case 0xF1: case 0xF2: case 0xF3: case 0xF4: case 0xF5: case 0xF7:
            case 0xF8: case 0xF9: case 0xFa: case 0xFb: case 0xFc: case 0xFd: case 0xFf:
                SET_N_R(opcode);
                break;
            case 0xC6: case 0xCE: case 0xD6: case 0xDE: case 0xE6: case 0xEE: case 0xF6:
            case 0xFE:
                SET_N_ADDR_HL(opcode);
                break;

            default:
                m_exit = true;
                #ifndef NDEBUG
                    std::cout << "Undefined opcode CB: " << std::hex << +opcode << std::endl;
                    printf("Register states: \nA: 0x%02X, B:0x%02X, C:0x%02X, D:0x%02X, E:0x%02X, H:0x%02X, L:0x%02X, F:0x%02X\nSP:0x%04X, PC:0x%04X\n", GET_REG(RegisterA), GET_REG(RegisterB), GET_REG(RegisterC), GET_REG(RegisterD), GET_REG(RegisterE), GET_REG(RegisterH), GET_REG(RegisterL), GET_REG(RegisterF), SP, PC);
                #endif
                break;

        }
    }
    // Standard opcode table
    else
    {
        switch (opcode)
        {      
            case 0x00:
                NOP();
                break;

            /* RLA, RRA, RRCA, RLCA */
            case 0x07:
                RLCA();
                break;
            case 0x17:
                RLA();
                break;
            case 0x0F:
                RRCA();
                break;
            case 0x1F:
                RRA();
                break;

            /* PUSH & POP */
            case 0xC5:
                PUSH_BC();
                break;
            case 0xD5:
                PUSH_DE();
                break;
            case 0xE5:
                PUSH_HL();
                break;
            case 0xF5:
                PUSH_AF();
                break;

            case 0xC1:
                POP_BC();
                break;
            case 0xD1:
                POP_DE();
                break;
            case 0xE1:
                POP_HL();
                break;
            case 0xF1:
                POP_AF();
                break;
            
            /* CPL, SCF, CCF */
            case 0x2F:
                CPL();
                break;
            case 0x37:
                SCF();
                break;
            case 0x3F:
                CCF();
                break;
		

            /* All the JR */
            case 0x18: // JR R8
                JR_R8()
                break;
            case 0x20: case 0x28: case 0x30: case 0x38: // JR COND, R8 
                JR_R8_COND()
                break;
            
            /* All the JUMP */
            case 0xC3:    
                JUMP_A16()
                break;

            /* All the CALL */
            case 0xCD:
                CALL_U16()
                break;
            case 0xC4: case 0xD4: case 0xCC: case 0xDC:
                CALL_U16_COND()
                break;

            /* RET (COND) and RET */
            case 0xC0: case 0xC8: case 0xD0: case 0xD8:
                RET_COND()
                break;
            case 0xC9:
                RET()
                break;

            /* AND A, u8 */
            case 0xE6:
                AND_IMM8()
                break;

            /* LD RR, IMM16 */
            case 0x01:
                LD_BC_IMM16()
                break;
            case 0x11:
                LD_DE_IMM16()
                break;
            case 0x21:
                LD_HL_IMM16()
                break;
            case 0x31:
                LD_SP_IMM16()
                break;

            /* LD (RR), A or LD (R), A */
            case 0x02:
                LD_ADDR_BC_A()
                break;
            case 0x12:
                LD_ADDR_DE_A()
                break;
            case 0x22:
                LD_ADDR_HLI_A()
                break;
            case 0x32:
                LD_ADDR_HLD_A()
                break;
            case 0xE2:
                LD_ADDR_C_A()
                break;
            case 0xE0:
                LD_FF00_IMM8_A()
                break;
            
            /* INC RR */
            case 0x03:
                INC_BC()
                break;
            case 0x13:
                INC_DE()
                break;
            case 0x23:
                INC_HL()
                break;
            case 0x33:
                INC_SP()
                break;
            
            
            /* INC R or INC (RR) */
            case 0x04:
                INC_B()
                break;
            case 0x14:
                INC_D()
                break;
            case 0x24:
                INC_H()
                break;
            case 0x34:
                INC_ADDR_HL()
                break;

            /* DEC R or DEC (RR) */
            case 0x05:
                DEC_B()
                break;
            case 0x15:
                DEC_D()
                break;
            case 0x25:
                DEC_H()
                break;
            case 0x35:
                DEC_ADDR_HL()
                break;

            /* LD R, imm8 or LD (RR), imm8 or LD (IMM16), A */
            case 0x06:
                LD_B_IMM8()
                break;
            case 0x16:
                LD_D_IMM8()
                break;
            case 0x26:
                LD_H_IMM8()
                break;
            case 0x36:
                LD_ADDR_HL_IMM8()
                break;
            case 0xEA:
                LD_ADDR_U16_A()
                break;
			case 0xFA:
				LD_A_ADDR_U16()
				break;

            /* ADD HL, RR */
            case 0x09:
                ADD_HL_BC()
                break;
            case 0x19:
                ADD_HL_DE()
                break;
            case 0x29:
                ADD_HL_HL()
                break;
            case 0x39:
                ADD_HL_SP()
                break;
            
            /* LD A, (RR) OR LD A, (FF00+IMM8) */
            case 0x0A:
                LD_A_ADDR_BC()
                break;

            case 0x1A:
                LD_A_ADDR_DE()
                break;
            
            case 0x2A:
                LD_A_ADDR_HLI()
                break;

            case 0x3A:
                LD_A_ADDR_HLD()
                break;

			case 0xE9:
				JUMP_HL()
				break;

			//TODO: Maybe implement with LD_R_ADDR_HL macro that auto detects the register
            case 0x66:
                LD_H_ADDR_HL()
                break; 
            case 0x56:
                LD_D_ADDR_HL()
                break; 
            case 0x46:
                LD_B_ADDR_HL()
                break; 

            case 0x7E:
                LD_A_ADDR_HL()
                break; 
            case 0x6E:
                LD_L_ADDR_HL()
                break; 
            case 0x5E:
                LD_E_ADDR_HL()
                break; 
            case 0x4E:
                LD_C_ADDR_HL()
                break; 


            case 0xF0:
                LD_A_FF00_IMM8()
                break;

            /* DEC RR */
            case 0x0B:
                DEC_BC()
                break;
            case 0x1B:
                DEC_DE()
                break;
            case 0x2B:
                DEC_HL()
                break;
            case 0x3B:
                DEC_SP()
                break;

            /* INC R */
            case 0x0C:
                INC_C()
                break;
            case 0x1C:
                INC_E()
                break;
            case 0x2C:
                INC_L()
                break;
            case 0x3C:
                INC_A()
                break;

            /* DEC R */
            case 0x0D:
                DEC_C()
                break;
            case 0x1D:
                DEC_E()
                break;
            case 0x2D:
                DEC_L()
                break;
            case 0x3D:
                DEC_A()
                break;

            /* LD R, d8 */
            case 0x0E:
                LD_C_IMM8()
                break;
            case 0x1E:
                LD_E_IMM8()
                break;
            case 0x2E:
                LD_L_IMM8()
                break;
            case 0x3E:
                LD_A_IMM8()
                break;


            /* LD r, r opcodes */
            case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x47:
            case 0x48: case 0x49: case 0x4a: case 0x4b: case 0x4c: case 0x4d: case 0x4f:
            case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x57:
            case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5d: case 0x5f:
            case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x67:
            case 0x68: case 0x69: case 0x6a: case 0x6b: case 0x6c: case 0x6d: case 0x6f:
            case 0x78: case 0x79: case 0x7a: case 0x7b: case 0x7c: case 0x7d: case 0x7f:
                LD_R_R()
                break;

            /* LD (HL), R */
            case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x77:
                LD_ADDR_HL_R(opcode)
                break;

            /* ADD (HL) */
            case 0x86:
                ADD_A_ADDR_HL();
                break;

            /* ADD HL, DE */

            /* ADD A, r opcodes */
            case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x87:
                ADD_A_R()
                break;

            /* ADC A, r opcodes */
            case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8F:
                ADC_A_R()
                break; 

            /* SUB A, r opcodes */
            case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x97:
                SUB_A_R()
                break;
            
            case 0xD6:
                SUB_A_IMM8()
                break;

            /* SBC A, r opcodes */
            case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9F:
                SBC_A_R()
                break; 

            /* AND r opcodes */
            case 0xA0: case 0xA1: case 0xA2: case 0xA3: case 0xA4: case 0xA5: case 0xA7:
                AND_R()
                break;
            
            /* XOR r opcodes */
            case 0xA8: case 0xA9: case 0xAA: case 0xAB: case 0xAC: case 0xAD: case 0xAF:
                XOR_R()
                break;

            /* OR r opcodes */
            case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB7:
                OR_R()
                break;

            /* CP R or CP d8 or CP (HL) */
            case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBF:
                CP_R()
                break;
            case 0xBE:
                CP_ADDR_HL()
                break;
            case 0xFE:
                CP_IMM8()
                break;
			
			/* RST xx */
			case 0xCF: case 0xDF: case 0xEF: case 0xFF:
				RST_XX();
				break;

			/* DI AND EI */
			case 0xF3:
				DI()
				break;
			case 0xFB:
				EI()
				break;

            default:
				debug_stop();
                m_exit = true;
                break;
        }
    }
}

u8 CPU::read_byte_at_working_ram(u16 position) {
    return m_working_ram[position];
}


void CPU::write_byte_at_working_ram(u16 position, u8 value) {
    m_working_ram[position] = value;
}
