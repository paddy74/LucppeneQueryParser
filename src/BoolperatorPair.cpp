#include <algorithm>
#include <lqueryparser/BoolperatorPair.hpp>

namespace lqueryparser
{
/* Constructors */

BoolperatorPair::BoolperatorPair(
    std::string const & leftStr, std::string const & operation,
    std::string const & rightStr)
    : BoolperatorPair(Boolperator(leftStr), operation, Boolperator(rightStr))
{
}

BoolperatorPair::BoolperatorPair(
    Boolperator const & leftBoolp, std::string const & operation,
    Boolperator const & rightBoolp)
{
    this->operation = operation;

    this->left = leftBoolp;
    this->right = rightBoolp;
}

BoolperatorPair::BoolperatorPair(
    std::string const & operation, std::string const & rightStr)
{
    this->isSingle = true;
    this->operation = "OR";
    this->right = Boolperator(operation, rightStr);
}

/* Public class methods */

std::string BoolperatorPair::toString() const
{
    std::string outStr = "";

    // Left
    if (!this->isSingle) outStr = "( " + this->left.toString() + " )";

    // Operation
    outStr.append(' ' + this->operation + ' ');

    // Right
    outStr.append("( " + this->right.toString() + " )");

    return outStr;
}

bool BoolperatorPair::getIsSingle() const { return this->isSingle; }

std::string BoolperatorPair::getOperation() const { return this->operation; }

Boolperator BoolperatorPair::getLeft() const { return this->left; }

Boolperator BoolperatorPair::getRight() const { return this->right; }

/* Public static class methods */

bool BoolperatorPair::strIsPairOperator(std::string const & str)
{
    auto const * found = std::find(
        BoolperatorPair::OPERATIONS.begin(), BoolperatorPair::OPERATIONS.end(),
        str);

    if (found != std::end(BoolperatorPair::OPERATIONS)) return true;  // Found
    return false;  // Not found
}

}  // namespace lqueryparser
