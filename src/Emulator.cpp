#include <pch.hpp>
#include <Emulator.hpp>

using std::cout, std::cin, std::endl;

namespace Duck
{

bool Emulator::RunProgram()
{
    long acc = 0l;
    long mp = 0l;
    long ip = MEM_START;
    bool retval = false;

    for (;;)
    {
        if (ip > MEM_MAX)
        {
            // Crash
            retval = false;
            break;
        }

        int opcode_value = memory[ip] / MEM_MAX;

        // This is data
        if (opcode_value == 0)
        {
            ip++;
        }

        // ADD:         ACC <-- c(ACC) + c(ADDR)
        else if (opcode_value == OPCODE_VALUES.at(ADD_STATEMENT))
        {
            mp = memory[ip] % MEM_MAX;
            acc = acc + memory[mp];
            ip++;
        }

        // SUB:         ACC <-- c(ACC) - c(ADDR)
        else if (opcode_value == OPCODE_VALUES.at(SUB_STATEMENT))
        {
            mp = memory[ip] % MEM_MAX;
            acc = acc - memory[mp];
            ip++;
        }

        // MULTIPLY:    ACC <-- c(ACC) * c(ADDR)
        else if (opcode_value == OPCODE_VALUES.at(MULT_STATEMENT))
        {
            mp = memory[ip] % MEM_MAX;
            acc = acc * memory[mp];
            ip++;
        }

        // DIVIDE:      ACC <-- c(ACC) / c(ADDR)
        else if (opcode_value == OPCODE_VALUES.at(DIV_STATEMENT))
        {
            mp = memory[ip] % MEM_MAX;
            acc = acc / memory[mp];
            ip++;
        }

        // LOAD:        ACC <-- c(ADDR)
        else if (opcode_value == OPCODE_VALUES.at(LOAD_STATEMENT))
        {
            mp = memory[ip] % MEM_MAX;
            acc = memory[mp];
            ip++;
        }

        // STORE:       ADDR <-- c(ACC)
        else if (opcode_value == OPCODE_VALUES.at(STORE_STATEMENT))
        {
            mp = memory[ip] % MEM_MAX;
            memory[mp] = acc;
            ip++;
        }

        // READ:        Read of a number and store it at the given address
        else if (opcode_value == OPCODE_VALUES.at(READ_STATEMENT))
        {
            mp = memory[ip] % MEM_MAX;
            cin >> memory[mp];
            // cout << "DEBUG: " << memory[mp] << endl;
            ip++;
        }

        // WRITE:       c(ADDR) is displayed
        else if (opcode_value == OPCODE_VALUES.at(WRITE_STATEMENT))
        {
            mp = memory[ip] % MEM_MAX;
            cout << memory[mp] << endl;
            ip++;
        }

        // BRANCH:      Go to ADDR for next instruction
        else if (opcode_value == OPCODE_VALUES.at(BRANCH_STATEMENT))
        {
            ip = memory[ip] % MEM_MAX;
        }

        // BRANCH MINUS: Go to ADDR if c(ACC) < 0
        else if (opcode_value == OPCODE_VALUES.at(BRANCH_MINUS_STATEMENT))
        {
            if (acc < 0)
                ip = memory[ip] % MEM_MAX;
            else
                ip++;
        }

        // BRANCH ZERO:  Go to ADDR if c(ACC) = 0
        else if (opcode_value == OPCODE_VALUES.at(BRANCH_ZERO_STATEMENT))
        {
            if (acc == 0)
                ip = memory[ip] % MEM_MAX;
            else
                ip++;
        }

        // BRANCH POSITIVE: Go to ADDR if c(ACC) > 0
        else if (opcode_value == OPCODE_VALUES.at(BRANCH_POSITIVE_STATEMENT))
        {
            if (acc > 0)
                ip = memory[ip] % MEM_MAX;
            else
                ip++;
        }

        else if (opcode_value == OPCODE_VALUES.at(HALT_STATEMENT))
        {
            retval = true;
            break;
        }

        else
        {
            // Crash?, just keep iterating?
        }
    }

    return retval;
}

// End namespace Duck
}