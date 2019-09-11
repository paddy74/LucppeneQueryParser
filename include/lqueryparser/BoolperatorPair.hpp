#pragma once

#include <lqueryparser/Boolperator.hpp>

namespace lqueryparser
{
/**
 * @brief A boolean operation object operating on a pair of boolean operation
 *  objects.
 *
 * @author Patrick Cox
 *
 */
class BoolperatorPair
{
public:
    /* Public static class members */

    // The possible boolean operators
    inline std::array<std::string, 4> const static OPERATIONS = {"AND", "&&",
                                                                 "OR", " "};

    /* Constructors */

    /**
     * @brief Construct a new BoolperatorPair object with an operation and a
     *  left and right boolperator.
     *
     * @param left
     * @param operation
     * @param right
     */
    BoolperatorPair(
        Boolperator const & left, std::string const & operation,
        Boolperator const & right);

    BoolperatorPair(Boolperator const & right, std::string const & operation);

    /* Public class methods */

    bool getIsSingle() const { return this->isSingle; }

    std::string getOperation() const { return this->operation; }

    Boolperator getLeft() const { return this->left; }

    Boolperator getRight() const;

    /* Public static class methods */

    /**
     * @brief Determines if a string is a pair operation.
     *
     * @param str
     * @return true
     * @return false
     */
    bool static strIsPairOperator(std::string const & str);

private:
    /* Private class members */

    bool isSingle = false;

    std::string operation;

    Boolperator left;
    Boolperator right;
};

}  // namespace lqueryparser
