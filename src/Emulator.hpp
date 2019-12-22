//
//		Emulator class - supports the emulation of Duck2200 programs
//
#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <Duck2200Constants.hpp>

namespace Duck
{

class Emulator 
{
public:
    // Records instructions and data into Duck2200 memory.
    bool InsertMemory(int location, long contents) 
    {
        try
        {
            memory.at(location) = contents;
        }
        catch (const std::out_of_range &e)
        {
            return false;
        }
        return true;
    };
    
    // Runs the Duck2200 program recorded in memory.
    bool RunProgram();

private:
    std::array<long, MEM_MAX> memory;      // The memory of the Duck2200.
};

// End namespace Duck
};

#endif
