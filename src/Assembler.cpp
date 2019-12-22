//
//      Implementation of the Assembler class.
//
#include <pch.hpp>
#include <Assembler.hpp>
#include <Errors.hpp>
#include <Instruction.hpp>

using std::cout, std::endl;

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
    // Nothing else to do here at this point.
}

// Destructor currently does nothing.  You might need to add something as you develope this project.
Assembler::~Assembler( )
{
}

// Pass I establishes the location of the labels.  You will write better function comments according to the coding standards.
void Assembler::PassI( ) 
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for (;;) 
    {
        // Read the next line from the source file.
        string line; 
        if (!m_facc.GetNextLine(line)) 
        {
            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }

        // Parse the line and get the instruction type.
        auto instruction =  Duck::Instruction::ParseInstruction(line);
        // cout << (int)instruction.type << " " << instruction.label << " " << instruction.opcode 
        //    << " " << std::get<std::string>(instruction.operand) << endl;


        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement.
        if (instruction.type == Duck::Instruction::InstructionType::End) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip other instruction types.
        if (instruction.type != Duck::Instruction::InstructionType::MachineLanguage 
            && instruction.type != Duck::Instruction::InstructionType::AssemblerInstruction) 
        {
        	continue;
        }

#if 0
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if(!instruction.label.empty()) 
        {
            m_symtab.AddSymbol(instruction.label, loc);
        }
#endif

        // Compute the location of the next instruction.
        loc = Duck::Instruction::LocationNextInstruction(instruction, loc);
        // cout << "Next location: " << loc << endl;
    }
}

