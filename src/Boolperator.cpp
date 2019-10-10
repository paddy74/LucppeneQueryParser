#include <algorithm>
#include <lqueryparser/Boolperator.hpp>

namespace lqueryparser
{
/* Constructors */

Boolperator::Boolperator() {}

Boolperator::Boolperator(std::string str)
{
    // Fill the operator if first character (before field)
    char const c = Boolperator::popCharOperator(str);
    if (c != 0) this->operation = c;  // If found

    // Fill the field variable
    {
        std::string const f = Boolperator::popField(str);
        if (!f.empty()) this->field = f;
    }

    // Fill the operator if first char
    {
        char const c2 = Boolperator::popCharOperator(str);
        if ((c != 0) && (c2 != 0))  // Both
            this->operation = Boolperator::mergeSingleOps(c, c2);
        else if (c2 != 0)  // Only c2
            this->operation = c2;
    }

    // Fill the string
    this->str = str;
    // Phrase setting
    this->setIsPhrase();
}

Boolperator::Boolperator(std::string const & operation, std::string str)
    : Boolperator(str)
{
    this->operation = Boolperator::mergeSingleOps(this->operation, operation);
}

/* Public class methods */

std::string Boolperator::toString() const
{
    std::string outStr = "";

    // Field
    outStr += this->field + ':';

    // String content
    if (this->isPhrase)
        outStr += "\"" + this->str + "\"";
    else
        outStr += this->str;

    // Operation
    if (!this->operation.empty())
    {
        outStr.insert(0, "(");
        outStr.insert(0, this->operation);
        outStr.append(")");
    }

    return outStr;
}

std::string Boolperator::getField() const { return this->field; }

std::string Boolperator::getOperation() const { return this->operation; }

std::string Boolperator::getStr() const { return this->str; }

bool Boolperator::getIsPhrase() const { return this->isPhrase; }

/* Public static class methods */

bool Boolperator::strIsSingleOperator(std::string const & str)
{
    auto const * found = std::find(
        Boolperator::OPERATIONS.begin(), Boolperator::OPERATIONS.end(), str);

    if (found != std::end(Boolperator::OPERATIONS)) return true;  // Found
    return false;                                                 // Not found
}

std::string Boolperator::mergeSingleOps(std::string opA, std::string opB)
{
    // If eitehr are empty
    if (opA.empty() && opB.empty())
        return "";
    else if (opA.empty() || opB.empty())  // Only one is empty
    {
        if (opA.empty()) return opB;
        return opA;  // opB is empty
    }

    // TODO: - is different from NOT and !
    if (opA == "-") opA = "!";
    if (opB == "-") opB = "!";

    if (opA == opB)  // Same
    {
        if (opA == "!")
            return "";  // Cancel out
        else            // Both are "+"
            return opA;
    }
    // else different
    return "NOT";  // NOT wins
}

std::string Boolperator::mergeSingleOps(char const & opA, char const & opB)
{
    return Boolperator::mergeSingleOps(
        std::string(1, opA), std::string(1, opB));
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
