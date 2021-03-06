#include "pch.hpp"
#include <Error.hpp>

using std::cout, std::endl;

namespace Duck
{

namespace Error
{

typedef std::string Error;
static std::multimap<size_t, Error> errors;

void DisplayErrors(const size_t line_num)
{
    if (line_num == 0)
    {
        for (const auto &error : errors)
        {
            cout << error.first << " - ERROR: " << error.second << endl;
        }
    }
    else
    {
        auto range = errors.equal_range(line_num);
        for (auto itr = range.first; itr != range.second; itr++)
        {
            cout << "ERROR: " << itr->second << endl;
        }
    }
}

size_t GetNumErrors()
{
    return errors.size();
}

void RecordError(const size_t line_num, std::string &&e)
{
    errors.insert({line_num, e});
}

// End namespace Logger
}

// End namespace Duck
}