//
//      Implementation of the Assembler class.
//
#include <pch.hpp>
#include <Assembler.hpp>
#include <Error.hpp>
#include <Instruction.hpp>
#include <Duck2200Constants.hpp>
#include <iomanip>

using std::cout, std::endl;

namespace Duck
{

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler(int argc, char **argv)
: m_facc( argc, argv ), CompletedPassI(false), invalidOpcode(false), invalidOperand(false)
{
    // Nothing else to do here at this point.
}

// Destructor currently does nothing.  You might need to add something as you develope this project.
Assembler::~Assembler()
{
}

// Pass I establishes the location of the labels.  You will write better function comments according to the coding standards.
void Assembler::PassI() 
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
            break;

        // Bitwise XOR (UGLY... but kinda cool?)
        if (is_end ^ eof)
        {
            if (is_end)
                Error::RecordError(line_num, std::string("Early end statement, end statement should be the o the last line of the file"));
            
            if (eof)
                Error::RecordError(line_num, std::string("No end statment found, the Duck assembly standard dictates that \
files contain an END statement on the last line."));

            Error::DisplayErrors(line_num);
            break;
        }

        auto instruction = Instruction::ParseInstruction(line);

        if (instruction.type == Instruction::InstructionType::Unknown)
        {
            Error::RecordError(line_num, std::string("Unable to parse line, too many symbols. Did you forget a \';\' to indicate a comment?"));
            cout << "-- " << line << endl;
            Error::DisplayErrors(line_num);
            continue;
        }
        else if (instruction.type == Instruction::InstructionType::End)
            is_end = true;

        auto result = ReportSyntaxErrors(line, instruction, line_num);
        
        if (instruction.type != Instruction::InstructionType::AssemblerInstruction
            && instruction.type != Instruction::InstructionType::MachineLanguage)
            continue;

        auto memory = TranslateInstruction(instruction);
        // If this was an origin statement then it doesn't go into memory
        if (memory / MEM_MAX == ORG_CONSTANT)
        {
            if (org_defined)
                Error::RecordError(line_num, std::string("Multiple org statements."));
            org_defined = true;
        }
        else
        {
            // Insert the memory into the emulator
            if (emulator.InsertMemory(loc, memory) == false)
                Error::RecordError(line_num, std::string("Program will no fit in the Duck2200's memory."));
        }

        cout << std::setfill('0') << std::setw(5) << loc << "\t";
        if (invalidOpcode && invalidOperand)
            cout << "???????";
        else if (invalidOpcode)
            cout << "??" << std::setfill('0') << std::setw(5) << memory % MEM_MAX;
        else if (invalidOperand)
            cout << std::setfill('0') << std::setw(2) << memory / MEM_MAX << "?????";
        else
            cout << std::setfill('0') << std::setw(7) << memory;
        cout << "\t" << line << endl;
        
        Error::DisplayErrors(line_num);

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

bool Assembler::ReportSyntaxErrors(const std::string &line, 
    const Instruction::Instruction &inst, const size_t line_num)
{
    bool retval = false;
    invalidOpcode = false;
    invalidOperand = false;

    // Check for errors with the OpCode
    std::string opcode_upper;
    if (!inst.opcode.empty())
    {
        // This will be used further down
        opcode_upper = inst.opcode;
        for (auto &c : opcode_upper)
            c = std::toupper(c);

        // A syntax error is the OpCode is actually already marked by ParseInstruction
        if (inst.type == Instruction::InstructionType::Invalid)
        {
            Error::RecordError(line_num, std::string("Syntax error, invalid OpCode " + inst.opcode));
            retval = invalidOpcode = true;
        }
    }
    else if (inst.type != Instruction::InstructionType::Comment)
    {
        std::cerr << line_num << ": " << " ERROR: The assembler should have marked this line as a comment." << endl;
    }

    // Check for errors with the operand
    // Try to treat it as a symbol
    try
    {
        auto operand = std::get<std::string>(inst.operand);

        if (!operand.empty())
        {
            if (opcode_upper == HALT_STATEMENT)
            {
                Error::RecordError(line_num, std::string("Invalid operand, OpCode " + inst.opcode + " does not take an operand."));
                retval = invalidOperand = true;
            }
            else if (opcode_upper == ORIGIN_STATEMENT || opcode_upper == DEFINE_CONSTANT_STATEMENT
                || opcode_upper == DEFINE_STORAGE_STATEMENT)
            {
                Error::RecordError(line_num, std::string("Invalid operand, OpCode " + inst.opcode + " requires a numeric operand."));
                retval = invalidOperand = true;
            }
            else if (!std::isalpha(operand[0])
                    || operand.size() > MAXIMUM_SYMBOL_LENGTH)
            {
                Error::RecordError(line_num, std::string("Invalid operand, attempting to use invalid symbol " + operand));
            }

            size_t loc = SYMBOL_NOT_FOUND;
            symtab.LookupSymbol(operand, &loc);

            if (loc == SYMBOL_NOT_FOUND)
            {
                Error::RecordError(line_num, std::string("Invalid operand, undefined symbol " + operand));
                retval = invalidOperand = true;
            }
            else if (loc == MULTIPLY_DEFINED_SYMBOL)
            {
                Error::RecordError(line_num, std::string("Invalid operand, attempting to use multiply defined symbol " + operand + "."));
                retval = invalidOperand = true;
            }
            else if (loc > MEM_MAX)
            {
                Error::RecordError(line_num, std::string("Invalid operand, " + operand + " resides outside of Duck2200 address space."));
                retval = invalidOperand = true;
            }
        }
        else if (opcode_upper != HALT_STATEMENT)
        {
            Error::RecordError(line_num, std::string("Missing operand, OpCode " + inst.opcode + " requires an operand."));
            retval = invalidOperand = true;
        }
    }
    catch (const std::bad_variant_access &e) 
    { 
        // Means it's holding a number instead of a symbol
        auto operand = std::get<long>(inst.operand);

        if (opcode_upper != ORIGIN_STATEMENT && opcode_upper != DEFINE_CONSTANT_STATEMENT
            && opcode_upper != DEFINE_STORAGE_STATEMENT)
        {
            Error::RecordError(line_num, std::string("Invalid operand, " + inst.opcode + " expects a symbolic operand."));
            retval = invalidOperand = true;
        }
        else if (opcode_upper == HALT_STATEMENT)
        {
            Error::RecordError(line_num, std::string("Invalid operand, OpCode " + inst.opcode + " does not take an operand."));
            retval = invalidOperand = true;
        }

        if ((operand > MEM_MAX || operand < 0) < 0 && opcode_upper != DEFINE_CONSTANT_STATEMENT)
        {
            if (opcode_upper == DEFINE_STORAGE_STATEMENT)
            {
                Error::RecordError(line_num, std::string("Invalid operand, " + std::to_string(operand) + " is too large for Duck2200 memory."));
                retval = invalidOperand = true;
            }
            else
            {
                Error::RecordError(line_num, std::string("Invalid operand, " + std::to_string(operand) + " is outside of the Duck2200 address space."));
                retval = invalidOperand = true;
            }
        }

        if ((operand > CONSTANT_MAX || operand < (-1 * CONSTANT_MAX)) && opcode_upper == DEFINE_CONSTANT_STATEMENT)
        {
            Error::RecordError(line_num, std::string("Invalid operand, " + std::to_string(operand) + " constant too large for Duck2200 memory."));
            retval = invalidOperand = true;
        }
    }

    // Check for errors with the label
    if (!inst.label.empty())
    {
        if (inst.label.size() > MAXIMUM_SYMBOL_LENGTH)
        {
            Error::RecordError(line_num, std::string("Invalid label, " + inst.label + " the Duck assembly standard limits labels to " 
                + std::to_string(MAXIMUM_SYMBOL_LENGTH) + "."));
            retval = true;
        }

        if (!std::isalpha(inst.label[0]))
        {
            Error::RecordError(line_num, std::string("Invalid label, " + inst.label + " the Duck assembly standard states that labels must begin with a letter."));
            retval = true;
        }
    }

    return retval;
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

    // Try to cast operand to a string and construct memory acoordingly
    try
    {
        auto operand = std::get<std::string>(inst.operand);
        size_t loc = SYMBOL_NOT_FOUND;
        if (symtab.LookupSymbol(operand, &loc))
        {
            mem += loc;
        }
    }
    catch (const std::bad_variant_access &e)
    {
        // If it's not a string it's a value, add to mem accordingly
        auto operand = std::get<long>(inst.operand);

        // ORG and DS don't end up in Duck2200 memory
        if (opcode_upper.compare(DEFINE_STORAGE_STATEMENT) == 0
            || opcode_upper.compare(ORIGIN_STATEMENT) == 0)
        {
            mem = 0;
        }
        else
        {
            // If the constant is negative we must preserve it's negativity
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