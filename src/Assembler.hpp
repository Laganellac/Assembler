//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <FileAccess.hpp>
#include <Instruction.hpp>
#include <Emulator.hpp>
#include <SymbolTable.hpp>

namespace Duck
{

class Assembler 
{
public:
    Assembler(int argc, char **argv);
    ~Assembler();

    // Pass I - establish the locations of the symbols
    void PassI();

    // Pass II - generate a translation
    void PassII();

    // Display the symbols in the symbol table.
    void DisplaySymbolTable() { symtab.DisplaySymbolTable(); }
    
    // Run emulator on the translation.
    void RunProgramInEmulator();

private:
    bool ReportSyntaxErrors(const std::string &line, 
        const Instruction::Instruction &inst, const size_t line_num);
    long TranslateInstruction(const Instruction::Instruction& inst);

    // Some flags for managing state
    bool CompletedPassI;
    bool invalidOpcode;
    bool invalidOperand;

    FileAccess m_facc;	    // File Access object
    SymbolTable symtab;   // Symbol table object
    Emulator emulator;        // Emulator object
};

// End namespace duck
};
#endif
