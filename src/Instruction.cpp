#include "pch.hpp"
#include <Duck2200Constants.hpp>
#include <Instruction.hpp>
#include <sstream>
#include <algorithm>

using std::cout, std::endl;

namespace Duck
{
namespace Instruction
{

static std::string RemoveComment(const std::string &line)
{
    auto loc = line.find(COMMENT_CHAR);
    if (loc == std::string::npos)
    {
        return line;
    }
    else
    {
        return line.substr(0, loc);
    }
}

static InstructionType CheckInstructionType(const std::string &opcode)
{
    std::string opcode_upper(opcode);
    for (auto &c : opcode_upper)
    {
        c = toupper(c);
    }

    // Check if it's machine code
    for (const auto &code : OPCODES)
    {
        if (opcode_upper.compare(code) == 0)
        {
            return InstructionType::MachineLanguage;
        }
    }

    // Check if it's assembler code
    for (const auto &code : ASSEMBLER_CODES)
    {
        if (opcode_upper.compare(code) == 0)
        {
            return InstructionType::AssemblerInstruction;
        }
    }

    // Check if it is the end statement
    if (opcode_upper.compare(END_STATEMENT) == 0)
    {
        return InstructionType::End;
    }

    return InstructionType::Invalid;
}

Instruction ParseInstruction(const std::string &line)
{
    auto new_line = RemoveComment(line);
    // cout << new_line << endl;

    if (new_line.empty())
    {
        return {InstructionType::Comment, "", "", ""};
    }

    std::istringstream iss(new_line);
    std::string s1, s2, s3, s4;
    iss >> s1 >> s2 >> s3 >> s4;

    /** VALID FORMATS
     * Label OpCode Operand
     * Label OpCode
     * OpCode Operand
     * OpCode
     * **/

    Instruction instr;
    instr.type = InstructionType::Invalid;

    if (!s4.empty())
    {
        // Log a warning about potential missing ';'
        instr.label = s1;
        instr.opcode = s2;
        instr.operand = s3;
        instr.type = InstructionType::Unknown;
    }

    // Has to be Label OpCode Operand
    else if (!s3.empty())
    {
        instr.label = s1;
        instr.opcode = s2;
        instr.operand = s3;
    }

    // Has to be OpCode
    else if (s2.empty())
    {
        instr.label = "";
        instr.opcode = s1;
        instr.operand = "";
    }

    // Either Label OpCode or OpCode Operand
    else
    {
        auto s1_type = CheckInstructionType(s1);
        // Has to be a Label OpCode
        if (s1_type == InstructionType::Invalid)
        {
            instr.label = s1;
            instr.opcode = s2;
            instr.operand = "";
        }
        // Has to be OpCode Operand
        else
        {
            instr.label = "";
            instr.opcode = s1;
            instr.operand = s2;
        }
    }

    // Convert the operand to a number if possible
    try
    {
        instr.operand = static_cast<long>(std::stol(std::get<std::string>(instr.operand)));
    }
    catch (const std::exception &e)
    {
        // This is supposed to throw most of the time so ignore it.
    }

    // If the type is not already marked as unknown figure out the type
    if (instr.type != InstructionType::Unknown)
        instr.type = CheckInstructionType(instr.opcode);

    return instr;
}

size_t LocationNextInstruction(const Instruction &inst, const size_t current_location)
{
    auto new_loc = current_location + DEFAULT_MEM_ITER;

    // Make ToUpper() function?
    std::string opcode_upper(inst.opcode);
    for (auto &c : opcode_upper)
    {
        c = toupper(c);
    }

    if (inst.type == InstructionType::AssemblerInstruction)
    {
        size_t operand_value;
        try
        {
            operand_value = std::get<long>(inst.operand);
        }
        catch (std::bad_variant_access &e)
        {
            // Log FATAL ERROR
            cout << "FATAL ERROR: " << inst.opcode << " requires numeric operand, was given " << std::get<std::string>(inst.operand) << "." << endl;
            exit(1);
        }
        
        if (opcode_upper.compare(DEFINE_STORAGE_STATEMENT) == 0)
        {
            new_loc = current_location + operand_value;
        }
        else if (opcode_upper.compare(ORIGIN_STATEMENT) == 0)
        {
            new_loc = operand_value;
        }
    } 
    return new_loc;
}


// End namepsace instruction
}
// End namepsace Duck
}