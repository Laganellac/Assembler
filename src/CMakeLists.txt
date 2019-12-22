
set(
DASM_SOURCES
    Assem.cpp
    Assembler.cpp
    FileAccess.cpp
    Instruction.cpp
    SymbolTable.cpp)

# No external dependencies

add_executable(dasm ${DASM_SOURCES})

target_include_directories(dasm
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR})

target_precompile_headers(dasm
PRIVATE
    pch.hpp)

target_compile_features(dasm 
PRIVATE
    cxx_std_17) # std::variant

set_target_properties(dasm
PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)