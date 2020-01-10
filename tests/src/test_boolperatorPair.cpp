#include <lqueryparser/Boolperator.hpp>
#include <lqueryparser/BoolperatorPair.hpp>

int main()
{
    std::string const left = "-hello";
    std::string const right = "good world";
    std::string const op = "AND";

    // Test constructors
    lqueryparser::BoolperatorPair boolpPair(op, right);
    boolpPair = lqueryparser::BoolperatorPair(
        lqueryparser::Boolperator(left), op, lqueryparser::Boolperator(right));
    boolpPair = lqueryparser::BoolperatorPair(left, op, right);

    // Test methods
    boolpPair.toString();
    boolpPair.getIsSingle();
    boolpPair.getOperation();
    boolpPair.getLeft();
    boolpPair.getRight();

    // Test static methods
    lqueryparser::BoolperatorPair::strIsPairOperator("Hello");
    lqueryparser::BoolperatorPair::strIsPairOperator("AND");
    lqueryparser::BoolperatorPair::strIsPairOperator("AND OR");

    return 0;
}
