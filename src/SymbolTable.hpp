//
//		Symbol table class.
//
#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

namespace Duck
{

// This class is our symbol table.
class SymbolTable 
{
public:
    // Get rid of constructor and destructor if you don't need them.
    SymbolTable() {};
    ~SymbolTable() {};
    
    // Add a new symbol to the symbol table.
    bool AddSymbol(const std::string &symbol, size_t location);

    // Display the symbol table.
    void DisplaySymbolTable();

    // Lookup a symbol in the symbol table.
    bool LookupSymbol(const std::string &symbol, size_t *location);

private:
    // This is the actual symbol table.  The symbol is the key to the map.
    std::map<std::string, size_t> symtab;

};

// End namespace Duck
};
#endif
