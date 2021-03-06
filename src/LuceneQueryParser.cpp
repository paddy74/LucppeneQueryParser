#include <lqueryparser/LuceneQueryParser.hpp>
#include <regex>

namespace lqueryparser
{
/* Public static class methods */

std::vector<BoolperatorPair> LuceneQueryParser::parse(
    std::string const & queryStr)
{
    auto phraseVect = LuceneQueryParser::extractPhrases(queryStr);
    auto const phraseTermVect =
        LuceneQueryParser::constructBoolperators(phraseVect);
    return phraseTermVect;
}

bool LuceneQueryParser::hasPhrase(std::string const & str)
{
    std::regex rgx("\".*\"");
    return std::regex_search(str, rgx);
}

bool LuceneQueryParser::isPhrase(std::string const & str)
{
    if (str.find(' ') == std::string::npos) return false;  // Not phrase
    return true;                                           // Is a phrase
}

/* Private static class methods */

template <typename T>
std::vector<T> prependVector(std::vector<T> a, std::vector<T> const & b)
{
    a.insert(a.end(), b.begin(), b.end());
    return a;
}

void LuceneQueryParser::extractPhrases(
    std::string const & queryStr, std::vector<std::string> & outPhraseTermVect)
{
    // TODO: Trim queryStr

    // Check if the string contains a phrase.
    if (!LuceneQueryParser::hasPhrase(queryStr))
    {
        std::vector const notPhraseVect =
            LuceneQueryParser::strSplit(queryStr, ' ');
        outPhraseTermVect = prependVector(outPhraseTermVect, notPhraseVect);

        return;
    }

    // TODO: There is a better way to do this
    // Else there is at least one phrase
    bool endLoop = false;
    for (auto itStart = queryStr.begin();
         (itStart < queryStr.end() - 1) && (!endLoop);
         ++itStart)  // Ignore last character
    {
        if (*itStart == '\"')  // Found left quote
        {
            // Loop through for right quote
            for (auto itEnd = (itStart + 2);
                 (itEnd < queryStr.end()) && (!endLoop); ++itEnd)
            {
                if (*itEnd == '\"')  // Found right quote
                {
                    // Append not-phrase left
                    if (itStart > queryStr.begin())  // If exists
                    {
                        // Get not-phrase left
                        std::string const notPhraseLeft = queryStr.substr(
                            0, (itStart - queryStr.begin()) - 1);

                        // Split on ' '
                        auto notPhraseLeftVect =
                            LuceneQueryParser::strSplit(notPhraseLeft, ' ');

                        // Prepend
                        outPhraseTermVect = prependVector(
                            notPhraseLeftVect, outPhraseTermVect);
                    }

                    // Phrase
                    {
                        // Iterator based substring
                        auto const phraseSize =
                            std::distance(itStart, itEnd + 1);
                        std::string const phrase = queryStr.substr(
                            itStart - queryStr.begin(), phraseSize);
                        outPhraseTermVect.push_back(phrase);
                    }

                    // The string right of phrase may have another phrase
                    if (itEnd < (queryStr.end() - 1))  // If exists
                    {
                        std::string const strRight = queryStr.substr(
                            (itEnd + 1) - queryStr.begin(),
                            queryStr.end() - queryStr.begin());

                        // Recursively append remaining not-phrases & phrases.
                        LuceneQueryParser::extractPhrases(
                            strRight, outPhraseTermVect);
                    }

                    // The recursive phrase search on strRight ensures that by
                    //  this line all not-phrases and phrases are extracted, so
                    //  exit the function.
                    endLoop = true;
                }
            }
        }
    }
}

std::vector<std::string> LuceneQueryParser::extractPhrases(
    std::string const & queryStr)
{
    std::vector<std::string> outPhraseTermVect;
    LuceneQueryParser::extractPhrases(queryStr, outPhraseTermVect);

    return outPhraseTermVect;
}

/**
 * @brief Remove the first element if it is a pair operation. Remove the last
 *  element if it is a single or pair operation.
 *
 * @param phraseTermVect
 * @return true Something was removed.
 * @return false Nothing was removed.
 */
bool trimOps(std::vector<std::string> & phraseTermVect)
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

std::vector<BoolperatorPair> LuceneQueryParser::constructBoolperators(
    std::vector<std::string> & phraseTermVect)
{
    // Handle 0 values
    if (phraseTermVect.size() == 0)  // If no values
        return std::vector<BoolperatorPair>();

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
            auto const & item = phraseTermVect.front();

            if (!Boolperator::strIsSingleOperator(item))  // Is a phrase/term
                return std::make_pair(
                    true, std::vector<BoolperatorPair>(
                              {BoolperatorPair("OR", item)}));
            else  // Is a single operator -> unacceptable
                return std::make_pair(true, std::vector<BoolperatorPair>());
        }
        return std::make_pair(false, std::vector<BoolperatorPair>());
    };

    // Handle 1 values
    {
        auto const ret = handleSize1();
        if (ret.first) return ret.second;
    }

    // Merge consecutive pair operations (inplace)
    LuceneQueryParser::mergeConsecutiveOps(phraseTermVect);

    // Trim invalid operators
    {
        bool wasItemRemoved = true;
        while (wasItemRemoved) wasItemRemoved = trimOps(phraseTermVect);

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

    std::vector<BoolperatorPair> outBoolpPairVect;  // The output

    for (auto it = phraseTermVect.begin(); it != phraseTermVect.end() - 1;
         ++it)
    {
        auto const idx = it - phraseTermVect.begin();  // The index
        std::string const & phraseLeft = phraseTermVect.at(idx);

        if (!BoolperatorPair::strIsPairOperator(phraseLeft))
        {  // phraseLeft is not a pairOp
            // Element to the right of phraseLeft
            std::string phraseRight = phraseTermVect.at(idx + 1);

            if (!Boolperator::strIsSingleOperator(phraseLeft))
            {  // phraseLeft is a phrase/term
                // Handle phraseRight
                // Phrase right is either a pairOp, singleOp, or phrase/term
                if (BoolperatorPair::strIsPairOperator(phraseRight))
                {  // phraseRight is a pairOp
                    std::string const pairOp = phraseRight;
                    phraseRight =
                        phraseTermVect.at(idx + 2);  // Update phraseRight

                    Boolperator rightBoolp;
                    // Handle updated phraseRight
                    if (Boolperator::strIsSingleOperator(phraseRight))
                    {  // updated phraseRight is a singleOp
                        // Consecutive single operators have been removed so
                        //  prhaseRight + 1 is not a single operator.
                        std::string const singleOp = phraseRight;
                        phraseRight =
                            phraseTermVect.at(idx + 3);  // Update phraseRight

                        rightBoolp = Boolperator(singleOp, phraseRight);
                        it++;  // Skip extra
                    }
                    else  // Updated phraseRight is a phrase/term
                        rightBoolp = Boolperator(phraseRight);
                    Boolperator const leftBoolp(phraseLeft);

                    outBoolpPairVect.push_back(
                        BoolperatorPair(leftBoolp, pairOp, rightBoolp));
                    it++;  // Skip
                }
                else if (Boolperator::strIsSingleOperator(phraseRight))
                {  // phraseRight is a singleOp
                    auto const singleOp = phraseRight;
                    phraseRight =
                        phraseTermVect.at(idx + 2);  // Update phraseRight

                    Boolperator leftBoolp;
                    Boolperator rightBoolp;

                    if (BoolperatorPair::strIsPairOperator(phraseRight))
                    {  // Updated phraseRight is a pairOp
                        auto const pairOp = phraseRight;
                        phraseRight =
                            phraseTermVect.at(idx + 3);  // Update phraseRight

                        if (singleOp == "+")
                        {
                            leftBoolp = Boolperator(phraseLeft);
                            rightBoolp = Boolperator(phraseRight);
                        }
                        else  // pairOp is of AND/OR type
                        {     // NOT AND, NOT OR (NAND, NOR)
                            leftBoolp = Boolperator("NOT", phraseLeft);
                            rightBoolp = Boolperator("NOT", phraseRight);
                        }

                        outBoolpPairVect.push_back(
                            BoolperatorPair(leftBoolp, pairOp, rightBoolp));
                        it++;  // Skip extra
                    }
                    else
                    {  // Updated phraseRight is a phrase/term
                        leftBoolp = Boolperator(phraseLeft);
                        rightBoolp = Boolperator(singleOp, phraseRight);
                        outBoolpPairVect.push_back(
                            BoolperatorPair(leftBoolp, "OR", rightBoolp));
                    }

                    it++;  // Skip
                }
                else
                {  // phraseRight is a phrase/term
                    outBoolpPairVect.push_back(
                        BoolperatorPair(phraseLeft, "OR", phraseRight));
                }
            }
            else
            {  // phrase is a singleOp, phraseRight is a phrase/term
                auto const & singleOp = phraseLeft;
                if (phraseRight.size() > 1)
                    outBoolpPairVect.push_back(
                        BoolperatorPair(singleOp, phraseRight));
            }
        }
        // TODO: Else something went wrong,
        //  Pair are surrounded by phrases or single operators
    }

    return outBoolpPairVect;
}

void LuceneQueryParser::mergeConsecutiveOps(
    std::vector<std::string> & phraseTermVect)
{
    for (std::vector<std::string>::iterator it = phraseTermVect.begin();
         it < (phraseTermVect.end() - 1);)
    {
        auto & val = *it;
        auto & valNext = *(it + 1);

        bool const valIsPOp = BoolperatorPair::strIsPairOperator(val);
        bool const valNextIsPOp = BoolperatorPair::strIsPairOperator(valNext);

        if (valIsPOp && valNextIsPOp)  // Merge sequential pair operators.
        {
            if (val == "&&") val = "AND";
            if (valNext == "&&") valNext = "AND";

            // If the operations are identical
            if (val == valNext)
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
            // TODO: Handle '-', "prohibits docs that contain the term"
            if ((val == "!") || (val == "-")) val = "NOT";
            if ((valNext == "!") || (valNext == "-")) valNext = "NOT";

            if (val == valNext)  // Same operators
            {
                if (val == "NOT")  // NOT
                {
                    // Cancel out
                    it = phraseTermVect.erase(it);
                    it = phraseTermVect.erase(it);
                }
                else                                // Both are '+'
                    it = phraseTermVect.erase(it);  // Erase one
            }
            else  // Different ('+' and "NOT")
            {
                it = phraseTermVect.erase(it);
                *it = "NOT";
            }
        }
        else if ((val == "+") && valNextIsPOp)
        {
            it = phraseTermVect.erase(it);
        }
        else  // Not sequential operators
            ++it;
    }
}

std::vector<std::string> LuceneQueryParser::strSplit(
    std::string const & str, char const & delim)
{
    std::vector<std::string> tokens;

    // Skip delimiters at the beginning
    std::string::size_type lastPos = str.find_first_not_of(delim, 0);
    // Find first non-delimiter
    std::string::size_type pos = str.find_first_of(delim, lastPos);

    while ((std::string::npos != pos || std::string::npos != lastPos))
    {
        // Found token, add to the token vector
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters
        lastPos = str.find_first_not_of(delim, pos);
        // Find next non-delimiter
        pos = str.find_first_of(delim, lastPos);
    }

    return tokens;
}

}  // namespace lqueryparser
