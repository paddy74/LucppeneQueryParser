#include <algorithm>
#include <lqueryparser/Boolperator.hpp>

namespace lqueryparser
{
/* Constructors */

Boolperator::Boolperator()
{
    this->operation = "NONE";
    this->str = "";
}

Boolperator::Boolperator(std::string str)
{
    // Fill the field variable
    str = this->fillField(str);

    // Get the boolean operator
    {
        char const & c0 = str.at(0);
        if ((c0 == '!') || (c0 == '-') || (c0 == '+'))
        {
            this->operation = c0;
            str = str.substr(1, str.size() - 1);  // Remove the variable
        }
        // else don't initialize
    }

    // Fill the string
    this->str = str;
}

Boolperator::Boolperator(std::string const & operation, std::string str)
{
    this->str = this->fillField(str);
    this->operation = operation;
}

/* Public class methods */

bool Boolperator::getIsPhrase() const
{
    if (this->str.find(' ') != std::string::npos) return true;
    return false;
}

std::string Boolperator::getField() const { return this->field; }

std::string Boolperator::getOperation() const { return this->operation; }

std::string Boolperator::getStr() const { return this->str; }

/* Public static class methods */

bool Boolperator::strIsSingleOperator(std::string const & str)
{
    auto const * found = std::find(
        Boolperator::OPERATIONS.begin(), Boolperator::OPERATIONS.end(), str);

    if (found != std::end(Boolperator::OPERATIONS)) return true;  // Found
    return false;                                                 // Not found
}

/* Private class methods */

std::string Boolperator::fillField(std::string const & str)
{
    // TODO: implement
    return str;
}

}  // namespace lqueryparser
