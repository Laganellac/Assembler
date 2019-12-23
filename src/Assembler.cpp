//
//      Implementation of the Assembler class.
//
#include <pch.hpp>
#include <Assembler.hpp>
#include <Logger.hpp>
#include <Instruction.hpp>
#include <Duck2200Constants.hpp>
#include <iomanip>

using std::cout, std::endl;

namespace Duck
{

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv ), CompletedPassI(false)
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
    m_facc.rewind();
    auto loc = MEM_START;       // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for (;;) 
    {
        // Read the next line from the source file.
        std::string line; 
        if (!m_facc.GetNextLine(line)) 
        {
            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            break;
        }

        // Parse the line and get the instruction type.
        auto instruction =  Instruction::ParseInstruction(line);
        // cout << (int)instruction.type << " " << instruction.label << " " << instruction.opcode 
        //    << " " << std::get<std::string>(instruction.operand) << endl;


        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement.
        if (instruction.type == Instruction::InstructionType::End) break;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip other instruction types.
        if (instruction.type != Instruction::InstructionType::MachineLanguage 
            && instruction.type != Instruction::InstructionType::AssemblerInstruction) 
        {
        	continue;
        }

        // If the instruction has a label, record it and its location in the
        // symbol table.
        if(!instruction.label.empty()) 
        {
            symtab.AddSymbol(instruction.label, loc);
        }

        // Compute the location of the next instruction.
        loc = Instruction::LocationNextInstruction(instruction, loc);
        // cout << "Next location: " << loc << endl;
    }

    CompletedPassI = true;
    return;
}

void Assembler::PassII()
{
    if (!CompletedPassI)
    {
        throw std::runtime_error("Attempted to call PassII on an Assembler object that has not called PassI");
    }

    m_facc.rewind();
    auto loc = MEM_START;
    bool is_end = false;
    bool org_defined = false;
    size_t line_num = 0;

    for (;;)
    {
        line_num++;
        std::string line;
        bool eof = !m_facc.GetNextLine(line);
        
        // Good there was no more code after END statement
        if (is_end && eof)
        {
            // Errors::DisplayErrors();
            break;
        }

        // Bitwise XOR (UGLY... but kinda cool?)
        if (is_end ^ eof)
        {
            if (is_end)
            {
                // ERROR: End statement not on the last line of the file (-f-allow-early-end)
                break;
            }
            
            if (eof)
            {
                // ERROR: No end statement found (-f-no-end)
                break;
            }
        }

        auto instruction = Instruction::ParseInstruction(line);
        
        if (instruction.type == Instruction::InstructionType::Invalid)
        {
            // ERROR: Invalid OpCode
            continue;
        }

        else if (instruction.type == Instruction::InstructionType::AssemblerInstruction
            || instruction.type == Instruction::InstructionType::MachineLanguage)
        {
            if (instruction.label.size() > MAXIMUM_SYMBOL_LENGTH)
            {
                // WARNING: The Duck2200 assembly language standard limits symbols to MAXIMUM_SYMBOL_LENGTH length.
            }
            
            try
            {
                if (std::get<std::string>(instruction.operand).size() > MAXIMUM_SYMBOL_LENGTH)
                {
                    // WARNING: The Duck2200 assembly language standard limits symbols to MAXIMUM_SYMBOL_LENGTH length.
                }
            }
            catch (const std::bad_variant_access &e) { /* Means it's holding a number instead of a symbol */}

            auto memory = TranslateInstruction(instruction);
            // If this was an origin statement then it doesn't go into memory
            if (memory / MEM_MAX == ORG_CONSTANT)
            {
                if (org_defined)
                {
                    // ERROR Mutiple ORG statements
                }
                org_defined = true;
            }
            else
            {
                // Insert the memory into the emulator
                if (emulator.InsertMemory(loc, memory) == false)
                {
                    // ERROR Program will not fit in Duck2200's memory
                } 
            }
            cout << std::setfill('0') << std::setw(5) << loc << "\t" << std::setw(7) << memory 
                << "\t" << line << endl;
        }

        else if (instruction.type == Instruction::InstructionType::Comment)
        {
            continue;
        }

        else if (instruction.type == Instruction::InstructionType::End)
        {
            is_end = true;
            continue;
        }

        loc = Instruction::LocationNextInstruction(instruction, loc);
    }
}

void Assembler::RunProgramInEmulator()
{
    cout << endl;
    cout << "Starting emulation..." << endl;

    bool retval = emulator.RunProgram();
    if (retval)
    {
        cout << "Program terminated successfully." << endl;
        return;
    }
    cout << "Program encountered an error during execution." << endl;
    return;
}

long Assembler::TranslateInstruction(const Instruction::Instruction &inst)
{
    std::string opcode_upper(inst.opcode);
    for (auto &c : opcode_upper)
    {
        c = toupper(c);
    }

    int opcode_value = -1;
    auto pair = OPCODE_VALUES.find(opcode_upper);
    if (pair != OPCODE_VALUES.end())
    {
        opcode_value = pair->second;
    }
    else if (opcode_upper.compare(ORIGIN_STATEMENT) == 0)
    {
        // ORG doesnt get stored in memory but I needed a way to pass this info up
        // creating a member variable for pollute the class for no reason
        opcode_value = ORG_CONSTANT;
    }
    else
    {
        opcode_value = 00;
    }

    long mem = 0l;

    // OPCODE lives in first two digits XX00000
    mem = opcode_value * MEM_MAX;

    // Figure out if there is an operand
    try 
    {
        auto operand = std::get<std::string>(inst.operand);
        if (operand.empty())
        {
            // TODO: Check if requires operand
            return mem;
        }
    }
    catch (const std::bad_variant_access &e) 
    { 
        // It must be holding something if it's holding a long (defaults to empy string)
    }

    // Try to cast operand to a string and construct memory acoordingly
    try
    {
        auto operand = std::get<std::string>(inst.operand);
        if (opcode_upper.compare(DEFINE_STORAGE_STATEMENT) == 0
            || opcode_upper.compare(DEFINE_CONSTANT_STATEMENT) == 0
            || opcode_upper.compare(ORIGIN_STATEMENT) == 0)
        {
            // ERROR This statement expects a numeric operand
        }

        size_t loc = SYMBOL_NOT_FOUND;
        symtab.LookupSymbol(operand, &loc);

        if (loc == SYMBOL_NOT_FOUND)
        {
            // ERROR Undefined symbol
        }
        else if (loc == MULTIPLY_DEFINED_SYMBOL)
        {
            // ERROR Multiply defined symbol
        }
        else if (loc > MEM_MAX)
        {
            // ERROR Symbol out of bounds
        }

        mem += loc;
    }
    // If it's not a string it's a value, add to mem accordingly
    catch (const std::bad_variant_access &e)
    {
        auto operand = std::get<long>(inst.operand);
        if (opcode_upper.compare(DEFINE_CONSTANT_STATEMENT) != 0
            || opcode_upper.compare(DEFINE_STORAGE_STATEMENT) != 0
            || opcode_upper.compare(ORIGIN_STATEMENT) != 0)
        {
            // WARNING Manually addressing memory instead of using label, is this intended?
        }

        if ((operand > MEM_MAX || operand < 0) < 0 && opcode_upper.compare(DEFINE_CONSTANT_STATEMENT) != 0)
        {
            if (opcode_upper.compare(DEFINE_STORAGE_STATEMENT) == 0)
            {
                // ERROR Bad allocation
            }
            else
            {
                // ERROR Invalid address
            }
        }

        // ORG and DS don't end up in Duck2200 memory
        if (opcode_upper.compare(DEFINE_STORAGE_STATEMENT) == 0
            || opcode_upper.compare(ORIGIN_STATEMENT) == 0)
        {
            mem = 0;
        }
        // If the constant is negative we must preserve it's negativity
        else
        {
            if (operand < 0)
            {
                mem += (operand * -1);
                mem *= -1;
            }
            else
            {
                mem += operand;
            }
        }
    }
    
    return mem;
}

// End namespace Duck
};