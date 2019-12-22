//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include <pch.hpp>
#include <SymbolTable.hpp>
#include <Duck2200Constants.hpp>
#include <iomanip>

namespace Duck
{
/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( char *a_symbol, int a_loc );

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
bool SymbolTable::AddSymbol(const std::string &sym, size_t loc)
{
    auto ret = symtab.insert({sym, loc});
    if (ret.second == false)
    {
        ret.first->second = MULTIPLY_DEFINED_SYMBOL;
    }
    return ret.second;
}

void SymbolTable::DisplaySymbolTable()
{
    int i = 0;
    cout << std::fixed << std::setw(6) << "Sym #" << "|" << std::setw(10) << "Sym Name" << "|" << std::setw(8) << "Sym loc" << endl;
    for (const auto& item : symtab)
    {
        cout << std::right << std::setw(6) << i++ << std::setw(10) << item.first << std::setw(6) << item.second << endl;
    }
}

bool SymbolTable::LookupSymbol(const std::string &sym, size_t  &loc)
{
    bool retval = false;
    loc = SYMBOL_NOT_FOUND;

    auto item = symtab.find(sym);
    if (item != symtab.end())
    {
        retval = true;
        loc = item->second;
    }
    return retval;
}

};