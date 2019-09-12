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
    inline std::array<std::string, 3> const static OPERATIONS = {"AND", "&&",
                                                                 "OR"};

    /* Constructors */

    /**
     * @brief Construct a new BoolperatorPair object with an operation and a
     *  left and right string.
     *
     * @param left
     * @param operation
     * @param right
     */
    BoolperatorPair(
        std::string const & leftStr, std::string const & operation,
        std::string const & rightStr);

    /**
     * @brief Construct a new BoolperatorPair object with an operation and a
     *  left and right boolperator.
     *
     * @param leftBoolp
     * @param operation
     * @param rightBoolp
     */
    BoolperatorPair(
        Boolperator const & leftBoolp, std::string const & operation,
        Boolperator const & rightBoolp);

    /**
     * @brief Construct a new BoolperatorPair that is a single Boolperator.
     *
     * @param operation
     * @param right
     */
    BoolperatorPair(
        std::string const & operation, std::string const & rightStr);

    /* Public class methods */

    std::string toString() const;

    /**
     * @brief Ask if this object is a single Boolperator.
     *
     * @return true
     * @return false
     */
    bool getIsSingle() const;

    /**
     * @brief Get the boolean operation for this object.
     *
     * @return std::string
     */
    std::string getOperation() const;

    /**
     * @brief Get the left Boolperator.
     *
     * @return Boolperator
     */
    Boolperator getLeft() const;

    /**
     * @brief Get the right boolperator.
     *
     * @return Boolperator
     */
    Boolperator getRight() const;

    /* Public static class methods */

    /**
     * @brief Determines if a string is a boolean operator that acts on two
     *  strings.
     *
     * @param str
     * @return true
     * @return false
     */
    bool static strIsPairOperator(std::string const & str);

private:
    /* Private class members */

    bool isSingle = false;  // Indicates this is a single Boolperator.

    std::string operation;  // The object's boolean operation.

    Boolperator left;   // The left Boolperator.
    Boolperator right;  // The right Boolperator.
};

}  // namespace lqueryparser
