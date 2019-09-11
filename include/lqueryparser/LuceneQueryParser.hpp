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
    LuceneQueryParser(){};

    /* Private static class methods */

    /**
     * @brief Parse phrases contained and not contained by double quotes into a
     *  vector by reference.
     *
     * @param str
     * @param outVect
     */
    void static extractPhrases(
        std::string const & queryStr, std::vector<std::string> & outVect);

    /**
     * @brief Parse phrases contained and not contained by double quotes into a
     *  vector.
     *
     * @param queryStr
     * @return std::vector<std::string>
     */
    std::vector<std::string> static extractPhrases(
        std::string const & queryStr);

    std::vector<BoolperatorPair> static constructBoolperators(
        std::vector<std::string> phraseTermVect);

    /**
     * @brief Merge consecutive operations.
     *
     * @param phraseTermVect
     */
    void static mergeConsecutiveOps(std::vector<std::string> & phraseTermVect);
};

}  // namespace lqueryparser
