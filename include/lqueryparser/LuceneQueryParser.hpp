#pragma once

#include <lqueryparser/BoolperatorPair.hpp>
#include <vector>

namespace lqueryparser
{
/**
 * @brief
 *
 * @author Patrick Cox
 *
 */
class LuceneQueryParser
{
public:
    /* Public static class methods */

    /**
     * @brief Parse a query string according to the Lucene Query Syntax.
     *
     * @param queryStr
     * @return std::vector<BoolperatorPair>
     */
    std::vector<BoolperatorPair> static parse(std::string const & queryStr);

    /**
     * @brief Determine if a string contains a phrase contained by double
     * quotes.
     *
     * @param str The string to search.
     * @return true The string has atleast one phrase.
     * @return false The string has no phrases.
     */
    bool static hasPhrase(std::string const & str);

    /**
     * @brief Determine if a string is a phrase. Phrases contain spaces.
     *
     * @param str
     * @return true
     * @return false
     */
    bool static isPhrase(std::string const & str);

private:
    /**
     * @brief Default constructor. Private encapsulation indicates this should
     *  be treated as a static class.
     *
     */
    LuceneQueryParser(){};

    /* Private static class methods */

    /**
     * @brief Parse phrases contained and not contained by double quotes into a
     *  vector by reference.
     *
     * @details Split a string into a vector based on \".*\" and space
     *  delimiters.
     *
     * @param str The string to split into phrases and not-phrases.
     * @param outPhraseTermVect The vector in which to place the output.
     */
    void static extractPhrases(
        std::string const & queryStr,
        std::vector<std::string> & outPhraseTermVect);

    /**
     * @brief Parse phrases contained and not contained by double quotes into a
     *  vector.
     *
     * @param queryStr The query string.
     * @return std::vector<std::string> The phrases and not-phrases of the
     *  query.
     */
    std::vector<std::string> static extractPhrases(
        std::string const & queryStr);

    /**
     * @brief Remove operators from a phrase term vector.
     *
     */
    std::vector<std::string> static removeOperators(
        std::vector<std::string> const & phraseTermVect);

    /**
     * @brief Construct the boolean operations vector.
     *
     * @param phraseTermVect The phrases and not-phrases of the query.
     * @return std::vector<BoolperatorPair>
     */
    std::vector<BoolperatorPair> static constructBoolperators(
        std::vector<std::string> & phraseTermVect);

    /**
     * @brief Merge consecutive operations.
     *
     * @param phraseTermVect
     */
    void static mergeConsecutiveOps(std::vector<std::string> & phraseTermVect);

    std::vector<std::string> static strSplit(
        std::string const & str, char const & delim);
};

}  // namespace lqueryparser
