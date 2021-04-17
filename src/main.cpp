#include "Emulator/Emulator.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    if (argc == 1)
    {
        std::cerr << "Please specify a rom file as an argument" << std::endl;
	    return 1;
    } 
    Emulator emu(argv[1]);
    emu.run();

    return 0;
}
