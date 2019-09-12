#include <lqueryparser/LuceneQueryParser.hpp>

int main()
{
    std::string const str =
        "I ! AND YOU OR me \"but not him\" with -\"you\" AND AND NOT OR AND "
        "him the one AND";

    bool bval = lqueryparser::LuceneQueryParser::hasPhrase("\"Hello World\"");
    if (!bval) throw std::runtime_error("hasPhrase() 1 failed");
    bval = lqueryparser::LuceneQueryParser::hasPhrase("Hello world");
    if (bval) throw std::runtime_error("hasPhrase() 2 failed");

    bval = lqueryparser::LuceneQueryParser::isPhrase("HelloWorld");
    if (bval) throw std::runtime_error("isPhrase() 1 failed");
    bval = lqueryparser::LuceneQueryParser::isPhrase("Hello world");
    if (!bval) throw std::runtime_error("isPhrase() 2 failed");

    auto const ret = lqueryparser::LuceneQueryParser::parse(str);

    return 0;
}
