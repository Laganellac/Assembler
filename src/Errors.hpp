//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#ifndef ERRORS_HPP
#define ERRORS_HPP

namespace Duck
{

class Errors 
{
public:
    // Initializes error reports.
    static void InitErrorReporting();

    // Records an error message.
    static void RecordError(std::string &a_emsg);

    // Displays the collected error message.
    static void DisplayErrors();

private:

    static std::vector<std::string> m_ErrorMsgs;
};

// End namespace Duck
};
#endif