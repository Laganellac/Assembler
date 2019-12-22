//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <variant>

namespace Duck
{

namespace Instruction
{

enum class InstructionType
{
    Invalid,                // Invalid instruction
    MachineLanguage,        // Machine language
    AssemblerInstruction,   // Assembler instruction
    Comment,                // Comment or ignore line
    End                     // End of program
};

struct Instruction
{
    InstructionType type;
    std::string label;
    std::string opcode;
    std::variant<std::string, long> operand;
};

Instruction ParseInstruction(const std::string &line);

size_t LocationNextInstruction(const Instruction &inst, size_t current_location);

// END OF INSTRUCTION NAMEPSACE
}
// END OF DUCK NAMEPSACE
}

#endif
