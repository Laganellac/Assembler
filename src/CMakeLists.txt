
set(
DASM_SOURCES
    Assem.cpp
    Assembler.cpp
    FileAccess.cpp
    SymbolTable.cpp)

set(
DASM_HEADERS
    Assembler.hpp
    Emulator.hpp
    Errors.hpp
    FileAccess.hpp
    Instruction.hpp
    SymbolTable.hpp)

# No external dependencies

add_executable(dasm ${DASM_SOURCES})

target_include_directories(dasm
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR})

target_precompile_headers(dasm
PRIVATE
    pch.hpp)

set_target_properties(dasm
PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)