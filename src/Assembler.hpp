//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <SymbolTable.hpp>
#include <FileAccess.hpp>
#include <Emulator.hpp>

namespace Duck
{

class Assembler {
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

    FileAccess m_facc;	    // File Access object
    SymbolTable symtab;   // Symbol table object
    emulator m_emul;        // Emulator object
};

// End namespace duck
};
#endif
