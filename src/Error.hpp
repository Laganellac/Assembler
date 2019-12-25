#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

namespace Duck
{

namespace Error
{

// void Configure(Config)

void DisplayErrors(const size_t line_num = 0);

size_t GetNumErrors();

void RecordError(const size_t line_num, const std::string &&e);

// void ResetErrors();

// void ResetConfig();

// End namespace Logger
}

// End namespace Duck
};
#endif