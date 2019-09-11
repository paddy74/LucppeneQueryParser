#include <lqueryparser/LuceneQueryParser.hpp>

namespace lqueryparser
{
/* Public static class methods */

std::vector<BoolperatorPair> LuceneQueryParser::parse(
    std::string const & queryStr)
{
    auto phraseVect = LuceneQueryParser::extractPhrases(queryStr);
    auto outVect = LuceneQueryParser::constructBoolperators(phraseVect);
    return outVect;
}

bool LuceneQueryParser::hasPhrase(std::string const & str)
{
    // TODO: Use regexs for double quotes

    if (str.find("\"") == std::string::npos) return false;  // No phrase found
    return true;                                            // Phrase found
}

bool LuceneQueryParser::isPhrase(std::string const & str)
{
    if (str.find(' ', 1) == std::string::npos) return false;  // Not phrase
    return true;                                              // Is a phrase
}

/* Private static class methods */

void LuceneQueryParser::extractPhrases(
    std::string const & queryStr, std::vector<std::string> & outVect)
{
    // Check if the string contains a phrase.
    if (!LuceneQueryParser::hasPhrase(queryStr))
    {
        outVect.push_back(queryStr);  // Append the string and return.
        return;
    }

    // Find the phrases
    for (auto itStart = queryStr.begin(); itStart < queryStr.end() - 1;
         ++itStart)  // Ignore last character
    {
        if (*itStart == '\"')  // Found left quote
        {
            // Loop through for right quote
            for (auto itEnd = (itStart + 2); itEnd < queryStr.end(); ++itEnd)
            {
                if (*itStart == '\"')  // Found right quote
                {
                    // Append not-phrase left
                    if (itStart > queryStr.begin())  // If exists
                    {
                        std::string const notPhraseLeft = queryStr.substr(
                            0, (itStart - queryStr.begin()) - 1);
                        outVect.push_back(notPhraseLeft);
                    }

                    // Phrase
                    {
                        std::string const phrase = queryStr.substr(
                            (itStart + 1) - queryStr.begin(),
                            (itEnd - 1) - queryStr.begin());
                        outVect.push_back(phrase);
                    }

                    // The string right of phrase may have another phrase
                    if (itEnd < (queryStr.end() - 1))  // If exists
                    {
                        std::string const strRight = queryStr.substr(
                            (itEnd + 1) - queryStr.begin(),
                            queryStr.end() - queryStr.begin());

                        // Recursively append remaining not-phrases & phrases.
                        LuceneQueryParser::extractPhrases(strRight, outVect);
                    }

                    // The recursive phrase search on strRight ensures that by
                    //  this line all not-phrases and phrases are extracted, so
                    //  exit the function.
                    return;
                }
            }
        }
    }
}

std::vector<std::string> LuceneQueryParser::extractPhrases(
    std::string const & queryStr)
{
    std::vector<std::string> outVect;
    LuceneQueryParser::extractPhrases(queryStr, outVect);

    return outVect;
}

/**
 * @brief Remove the first element if it is a pair operation. Remove the last
 *  element if it is a single or pair operation.
 *
 * @param phraseTermVect
 * @return true Something was removed.
 * @return false Nothing was removed.
 */
bool rmFirstLastIfOp(std::vector<std::string> & phraseTermVect)
{
    bool wasItemRemoved = false;

    // Left can only be single
    if (BoolperatorPair::strIsPairOperator(*phraseTermVect.begin()))
    {
        phraseTermVect.erase(phraseTermVect.begin());  // Remove first
        wasItemRemoved = true;
    }
    // Right must be phrase/term
    if ((Boolperator::strIsSingleOperator(*(phraseTermVect.end() - 1))) ||
        BoolperatorPair::strIsPairOperator(*(phraseTermVect.end() - 1)))
    {
        phraseTermVect.pop_back();  // Remove last
        wasItemRemoved = true;
    }

    return wasItemRemoved;
}

void LuceneQueryParser::mergeConsecutiveOps(
    std::vector<std::string> & phraseTermVect)
{
    for (std::vector<std::string>::iterator it = phraseTermVect.begin();
         it < (phraseTermVect.end() - 1);)
    {
        auto const & val = *it;
        auto const & valNext = *(it + 1);

        bool const valIsPOp = BoolperatorPair::strIsPairOperator(val);
        bool const valNextIsPOp = BoolperatorPair::strIsPairOperator(valNext);

        if (valIsPOp && valNextIsPOp)  // Merge sequential pair operators.
        {
            // If the operations are identical
            if (val == valNext)  // TODO: Handle different but same (AND vs &&)
                it = phraseTermVect.erase(it);  // it = (it + 1)
            else                                // Operations are not identical
            {
                it = phraseTermVect.erase(it);  // it = (it + 1)
                *it = "OR";                     // TODO: Support XOR
            }
        }
        else if (  // Merge sequential single operators.
            Boolperator::strIsSingleOperator(val) &&
            Boolperator::strIsSingleOperator(valNext))
        {
            if ((val == valNext) &&
                ((val == "NOT") || (val == "!") || (val == "-")))
            {
                it = phraseTermVect.erase(it);
                it = phraseTermVect.erase(it);
            }
            else
                it = phraseTermVect.erase(it);  // TODO: Properly
        }
        else
            ++it;
    }
}

std::vector<BoolperatorPair> LuceneQueryParser::constructBoolperators(
    std::vector<std::string> phraseTermVect)
{
    /**
     * @brief
     *
     * @details Use like:
     * - {
     * -    auto const ret = handleSize1();
     * -    if (ret.first) return ret.second;
     * - }
     *
     */
    auto handleSize1 = [&phraseTermVect]() {
        if (phraseTermVect.size() == 1)  // If only one item remains
        {
            auto const & item = phraseTermVect.at(0);

            if (!Boolperator::strIsSingleOperator(item))  // Is a phrase/term
            {
                Boolperator boolp(item);
                BoolperatorPair newPair("OR", boolp);

                return std::make_pair(
                    true, std::vector<BoolperatorPair>({newPair}));
            }
            else  // Is a single operator -> unacceptable
                return std::make_pair(true, std::vector<BoolperatorPair>());
        }
        return std::make_pair(false, std::vector<BoolperatorPair>());
    };

    // Handle 0 values
    if (phraseTermVect.size() == 0)  // If no values
        return std::vector<BoolperatorPair>();
    // Handle 1 values
    {
        auto const ret = handleSize1();
        if (ret.first) return ret.second;
    }

    // Merge consecutive pair operations
    LuceneQueryParser::mergeConsecutiveOps(phraseTermVect);

    // Ensure valid first and last elements and handle 1 values
    {
        // Remove first/last elements if invalid operator
        bool wasItemRemoved = true;
        while (wasItemRemoved)
            wasItemRemoved = rmFirstLastIfOp(phraseTermVect);

        // TODO: Ensure enough values remain
    }

    // Handle 1 values
    {
        auto const ret = handleSize1();
        if (ret.first) return ret.second;
    }

    /* At this point the following properties can be assumed:

        There is more than one element
        There are no consecutive pair operators
        There are no consecutive single operators
        The last element is a phrase/term
        The first element is not a pair operator
    */
    // TODO: Merge single operators into the phrase

    std::vector<BoolperatorPair> outVect;  // The output

    for (std::vector<std::string>::iterator it = phraseTermVect.begin();
         it != phraseTermVect.end();)
    {
        auto const i = it - phraseTermVect.begin();  // The index

        std::string phrase = phraseTermVect.at(i);
        std::string phraseRight = phraseTermVect.at(i + 1);

        if (!BoolperatorPair::strIsPairOperator(phrase))
        {
            if (Boolperator::strIsSingleOperator(phrase))
            {
                BoolperatorPair boolpPair(phraseRight, phrase);
                outVect.push_back(boolpPair);
                it++;
            }
            else  // Is a phrase/term, is not a pair or single operator
            {
                // Is paired to the right
                if (BoolperatorPair::strIsPairOperator(phraseRight))
                {
                    std::string const & phraseLeft = phrase;
                    phraseRight = phraseTermVect.at(i + 2);
                    std::string const & op = phraseRight;

                    Boolperator boolpLeft(phraseLeft);
                    Boolperator boolpRight(phraseRight);

                    BoolperatorPair boolpPair(phraseLeft, op, phraseRight);
                    outVect.push_back(boolpPair);
                    it += 2;
                }
                else  // Is an OR operation
                {
                    std::string const & phraseLeft = phrase;
                    std::string const & op = "OR";

                    Boolperator boolpLeft(phraseLeft);
                    Boolperator boolpRight(phraseRight);

                    BoolperatorPair boolpPair(phraseLeft, op, phraseRight);
                    outVect.push_back(boolpPair);
                    it++;
                }
            }
        }
        else
        {
            // TODO: Something went wrong. Throw an error

            it++;
        }
    }

    return outVect;
}

}  // namespace lqueryparser
