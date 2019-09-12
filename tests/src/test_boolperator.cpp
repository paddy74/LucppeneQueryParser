#include <lqueryparser/Boolperator.hpp>

int main()
{
    std::string const str = "-Hello world";

    // Test constructors
    {
        lqueryparser::Boolperator boolp();
    }
    {
        lqueryparser::Boolperator boolp(str);
    }
    lqueryparser::Boolperator boolp("NOT", str);

    // Test methods
    boolp.toString();
    boolp.getField();
    boolp.getOperation();
    boolp.getStr();

    // Test static methods
    lqueryparser::Boolperator::strIsSingleOperator("AND");
    lqueryparser::Boolperator::strIsSingleOperator("NOT");
    lqueryparser::Boolperator::strIsSingleOperator("hello");
    lqueryparser::Boolperator::strIsSingleOperator(" world!");
    lqueryparser::Boolperator::mergeSingleOps("NOT", "NOT");
    lqueryparser::Boolperator::mergeSingleOps("!", "+");

    return 0;
}
