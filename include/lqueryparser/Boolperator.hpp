#pragma once

#include <array>

namespace lqueryparser
{
/**
 * @brief A boolean operation object acting on a single string value.
 *
 * @author Patrick Cox
 *
 */
class Boolperator
{
public:
    /* Public static class members */

    // The possible boolean operators
    inline std::array<std::string, 5> const static OPERATIONS = {
        "NONE", "NOT", "!", "-", "+"};

    /* Constructors */

    /**
     * @brief Default constructor.
     *
     */
    Boolperator();

    /**
     * @brief Construct a new Boolperator object from the given string.
     *
     * @details Construct a new boolean operation object from the given
     * string. The string's first character may contain the characters '!',
     * '-', or
     *  '+' to indicate a boolean operation. Otherwise none will be
     * indicated.
     *
     * @param str
     */
    Boolperator(std::string str);

    /**
     * @brief Construct a new Boolperator object from the given string with the
     *  given operation.
     *
     * @details Construct a new boolean operation object from the given string
     *  with the given boolean operation.
     *
     * @param operation
     * @param str
     */
    Boolperator(std::string const & operation, std::string str);

    /* Public class methods */

    /**
     * @brief Is this object's string a Lucene phrase.
     *
     * @return true The string is a Lucene phrase.
     * @return false The string is a Lucene term.
     */
    bool getIsPhrase() const;

    /**
     * @brief Get the field name to operate on.
     *
     * @return std::string
     */
    std::string getField() const;

    /**
     * @brief Get the boolean operation of this object.
     *
     * @return std::string
     */
    std::string getOperation() const;

    /**
     * @brief Get the string content of this object.
     *
     * @return std::string
     */
    std::string getStr() const;

    /* Public static class methods */

    /**
     * @brief Determines if a string is a boolean operator that acts on a
     *  single string.
     *
     * @param str
     * @return true
     * @return false
     */
    bool static strIsSingleOperator(std::string const & str);

private:
    /* Private class members */

    std::string field = "content";  // The field name to operate on.
    std::string operation;          // The boolean operation.
    std::string str;                // The string content.

    /* Private class methods */

    /**
     * @brief Parse for the field variable if it exists and remove it from the
     *  string.
     *
     * @param str
     * @return std::string
     */
    std::string fillField(std::string const & str);
};

}  // namespace lqueryparser
