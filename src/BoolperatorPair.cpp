#include <algorithm>
#include <lqueryparser/BoolperatorPair.hpp>

namespace lqueryparser
{
/* Constructors */

BoolperatorPair::BoolperatorPair(
    Boolperator const & left, std::string const & operation,
    Boolperator const & right)
{
    this->operation = operation;

    this->left = left;
    this->right = right;
}

BoolperatorPair::BoolperatorPair(
    std::string const & operation, Boolperator const & right)
{
    this->isSingle = true;

    this->operation = operation;
    this->right = right;
}

/* Public class methods */

std::string BoolperatorPair::toString() const
{
    std::string outStr = "";

    if (!this->isSingle) outStr = '(' + this->left.toString() + ") ";
    outStr += this->operation + ' ';
    outStr += '(' + this->right.toString() + ')';

    return outStr;
}

bool BoolperatorPair::getIsSingle() const { return this->isSingle; }

std::string BoolperatorPair::getOperation() const { return this->operation; }

Boolperator BoolperatorPair::getLeft() const
{
    return this->left;  // TODO: If single, left is null, so throw error
}

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
