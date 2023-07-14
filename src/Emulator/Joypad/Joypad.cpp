#include <iostream>

#include <Emulator/Emulator.hpp>
#include <Emulator/Joypad/Joypad.hpp>

Joypad::Joypad(Emulator *emu)
	: m_emu(emu)
{}

void Joypad::update(SDL_Event e)
{
    switch (e.type) {

        case SDL_CONTROLLERBUTTONDOWN:
            controller_down(e);
            break;

        case SDL_KEYDOWN:
            key_down(e);
            break;

        case SDL_CONTROLLERBUTTONUP:
            controller_up(e);
            break;

        case SDL_KEYUP:
            key_up(e);
            break;

    }
}

void Joypad::write_byte(u8 value)
{
    m_column = (value >> 4) & 3;
}

u8 Joypad::read_byte()
{
    switch (m_column)
    {
        case 1: return m_rows[0];
        case 2: return m_rows[1];
		default: return (0xFF);
    }
}

void Joypad::controller_down(SDL_Event e)
{
	u8 old_state = read_byte();

    switch (e.cbutton.button)
    {
        case SDL_CONTROLLER_BUTTON_A:
            m_rows[0] &= ~(0x1); // A gameboy key
            break;
        case SDL_CONTROLLER_BUTTON_B:
            m_rows[0] &= ~(0x2); // B gameboy key
            break;
        case SDL_CONTROLLER_BUTTON_BACK:
            m_rows[0] &= ~(0x4); // SELECT gameboy key
            break;
        case SDL_CONTROLLER_BUTTON_START:
            m_rows[0] &= ~(0x8); // START gameboy key
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            m_rows[1] &= ~(0x1); // >
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            m_rows[1] &= ~(0x2); // <
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            m_rows[1] &= ~(0x4); // ^
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            m_rows[1] &= ~(0x8); // v
            break;
        default:
            break;
    }

	if (old_state != this->read_byte())
		m_emu->get_CPU().request_interrupt(CPU::Interrupt::JOYPAD);
}

void Joypad::key_down(SDL_Event e)
{
	u8 old_state = read_byte();

    switch (e.key.keysym.sym)
    {
        case SDLK_z:
            m_rows[0] &= ~(0x1); // A gameboy key
            break;
        case SDLK_x:
            m_rows[0] &= ~(0x2); // B gameboy key
            break;
        case SDLK_SPACE:
            m_rows[0] &= ~(0x4); // SELECT gameboy key
            break;
        case SDLK_RETURN:
            m_rows[0] &= ~(0x8); // START gameboy key
            break;
        case SDLK_RIGHT:
            m_rows[1] &= ~(0x1); // >
            break;
        case SDLK_LEFT:
            m_rows[1] &= ~(0x2); // <
            break;
        case SDLK_UP:
            m_rows[1] &= ~(0x4); // ^
            break;
        case SDLK_DOWN:
            m_rows[1] &= ~(0x8); // v
            break;
        default:
            break;
    }

	if (old_state != this->read_byte())
		m_emu->get_CPU().request_interrupt(CPU::Interrupt::JOYPAD);
}

void Joypad::controller_up(SDL_Event e)
{
    switch (e.cbutton.button)
    {
        case SDL_CONTROLLER_BUTTON_A:
            m_rows[0] |= 0x1; // A gameboy key
            break;
        case SDL_CONTROLLER_BUTTON_B:
            m_rows[0] |= 0x2; // B gameboy key
            break;
        case SDL_CONTROLLER_BUTTON_BACK:
            m_rows[0] |= 0x4; // SELECT gameboy key
            break;
        case SDL_CONTROLLER_BUTTON_START:
            m_rows[0] |= 0x8; // ENTER gameboy key
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            m_rows[1] |= 0x1;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            m_rows[1] |= 0x2;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            m_rows[1] |= 0x4;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            m_rows[1] |= 0x8;
            break;
        default:
            break;
    }
}

void Joypad::key_up(SDL_Event e)
{
    switch (e.key.keysym.sym) {
        case SDLK_z:
            m_rows[0] |= 0x1; // A gameboy key
            break;
        case SDLK_x:
            m_rows[0] |= 0x2; // B gameboy key
            break;
        case SDLK_SPACE:
            m_rows[0] |= 0x4; // SELECT gameboy key
            break;
        case SDLK_RETURN:
            m_rows[0] |= 0x8; // ENTER gameboy key
            break;
        case SDLK_RIGHT:
            m_rows[1] |= 0x1;
            break;
        case SDLK_LEFT:
            m_rows[1] |= 0x2;
            break;
        case SDLK_UP:
            m_rows[1] |= 0x4;
            break;
        case SDLK_DOWN:
            m_rows[1] |= 0x8;
            break;
        default:
            break;
    }
}