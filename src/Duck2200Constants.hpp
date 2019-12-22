#include <string>
#include <array>
#include <unordered_map>

namespace Duck
{

// Instruction constants, maybe go in Instruction namespace?
const char COMMENT_CHAR = ';';
const std::string DEFINE_CONSTANT_STATEMENT = "DC";
const std::string DEFINE_STORAGE_STATEMENT = "DS";
const std::string ORIGIN_STATEMENT = "ORG";
const std::string END_STATEMENT = "END";
const std::array<std::string, 3> ASSEMBLER_CODES{DEFINE_CONSTANT_STATEMENT, DEFINE_STORAGE_STATEMENT, ORIGIN_STATEMENT};

const std::string ADD_STATEMENT = "ADD";
const std::string SUB_STATEMENT = "SUB";
const std::string MULT_STATEMENT = "MULT";
const std::string DIV_STATEMENT = "DIV";
const std::string LOAD_STATEMENT = "LOAD";
const std::string STORE_STATEMENT = "STORE";
const std::string READ_STATEMENT = "READ";
const std::string WRITE_STATEMENT = "WRITE";
const std::string BRANCH_STATEMENT = "B";
const std::string BRANCH_MINUS_STATEMENT = "BM";
const std::string BRANCH_ZERO_STATEMENT = "BZ";
const std::string BRANCH_POSITIVE_STATEMENT = "BP";
const std::string HALT_STATEMENT = "HALT";
const std::array<std::string, 13> OPCODES{ADD_STATEMENT, SUB_STATEMENT, MULT_STATEMENT, DIV_STATEMENT, LOAD_STATEMENT, 
    STORE_STATEMENT, READ_STATEMENT, WRITE_STATEMENT, BRANCH_STATEMENT, BRANCH_MINUS_STATEMENT, BRANCH_ZERO_STATEMENT, 
    BRANCH_POSITIVE_STATEMENT, HALT_STATEMENT};

const std::unordered_map<std::string, int> OPCODE_VALUES{{DEFINE_CONSTANT_STATEMENT, 0}, {ADD_STATEMENT, 1},
    {SUB_STATEMENT, 2}, {MULT_STATEMENT, 3}, {DIV_STATEMENT, 4}, {LOAD_STATEMENT, 5}, {STORE_STATEMENT, 6}, {READ_STATEMENT, 7},
    {WRITE_STATEMENT, 8}, {BRANCH_STATEMENT, 9}, {BRANCH_MINUS_STATEMENT, 10}, {BRANCH_ZERO_STATEMENT, 11}, {BRANCH_ZERO_STATEMENT, 12},
    {HALT_STATEMENT, 13}};

const size_t MEM_MAX = 100000;
const size_t DEFAULT_MEM_ITER = 1;

}