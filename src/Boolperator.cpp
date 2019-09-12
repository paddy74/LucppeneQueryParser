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
    std::string const f = Boolperator::popField(str);
    if (!f.empty()) this->field = f;

    // Fill the operator if first char
    char const c = Boolperator::popCharOperator(str);
    if (c != 0) this->operation = c;

    // Fill the string
    this->str = str;
}

Boolperator::Boolperator(std::string const & operation, std::string str)
{
    this->str = this->popField(str);
    this->operation = operation;
}

/* Public class methods */

std::string Boolperator::toString() const
{
    std::string outStr = "";

    // Opearation
    if (this->operation != "NONE") outStr = this->operation + ' ';
    outStr += this->field + ':';  // Field

    // String content
    if (this->isPhrase)
        outStr += "\"" + this->str + "\"";
    else
        outStr += this->str;

    return outStr;
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

void Boolperator::setIsPhrase()
{
    if ((this->str.front() == '"') && (this->str.back() == '"'))  // Is phrase
    {
        this->isPhrase = true;
        this->str.erase(str.begin());
        this->str.erase(str.end() - 1);
    }
    else if (str.find(' ', 1) != std::string::npos)  // Is multiword phrase
    {
        this->isPhrase = true;
    }
    else  // Not a phrase
        this->isPhrase = false;
}

/* Private static class methods */

std::string Boolperator::popField(std::string & str)
{
    if (str.size() < 3)  // Must have atleast three characters
        return "";

    auto const found = str.find(':');
    if (found != std::string::npos)  // Found
    {
        std::string const field = str.substr(0, found);
        str = str.substr(found + 1);
        return field;
    }
    return "";
}

char Boolperator::popCharOperator(std::string & str)
{
    if (str.size() < 2)  // Must have atleast two characters
        return 0;

    std::string const firstChar(1, str.front());
    auto const * found = std::find(
        Boolperator::OPERATIONS.begin(), Boolperator::OPERATIONS.end(),
        firstChar);
    if (found != std::end(Boolperator::OPERATIONS))  // Found
    {
        str = str.substr(1);  // Remove first character
        return firstChar.front();
    }
    return 0;  // Else
}

}  // namespace lqueryparser
