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
    inline std::array<std::string, 4> const static OPERATIONS = {"NOT", "!",
                                                                 "-", "+"};

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

    std::string toString() const;

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
    std::string operation = "";     // The boolean operation.
    std::string str = "";           // The string content.
    bool isPhrase = false;          // The string is a phrase.

    /* Private class methods */

    /**
     * @brief Check if the string is a phrase (contained by double quotes).
     *
     */
    void setIsPhrase();

    /* Private static class methods */

    /**
     * @brief Pop the field from a string and return it, if found.
     *
     * @param str
     * @return std::string The field.
     */
    std::string static popField(std::string & str);

    /**
     * @brief Pop the first character from a string and return it if it is a
     *  boolean operator.
     *
     * @param str
     * @return char The single character operator.
     */
    char static popCharOperator(std::string & str);
};

}  // namespace lqueryparser
