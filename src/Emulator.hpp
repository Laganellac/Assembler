//
//		Emulator class - supports the emulation of Duck2200 programs
//
#ifndef EMULATOR_HPP
#define EMULATOR_HPP

namespace Duck
{

class emulator 
{
public:
    const static int MEMSZ = 100000;	// The size of the memory of the Duck2200.
    emulator() {

        // memset( m_memory, 0, MEMSZ * sizeof(int) );
    }
    // Records instructions and data into Duck2200 memory.
    bool insertMemory( int a_location, int a_contents );
    
    // Runs the Duck2200 program recorded in memory.
    bool runProgram( );

private:
    int m_memory[MEMSZ];       // The memory of the Duck2200.
    int accumulator;			// The accumulator for the Duck2200
};

// End namespace Duck
};

#endif
