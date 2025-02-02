#include <catch2/catch_test_macros.hpp>

#include "Factories/ParserFactory.hpp"
#include "Parsers/NormalTextFileParser.hpp"
#include "Parsers/XMLTextFileParser.hpp"

TEST_CASE("TestParserFactoryGetParser", "[ParserFactory]") {
    std::shared_ptr<FileHashParser> parser;

    REQUIRE_NOTHROW(parser = ParserFactory::getParser("text"));
    REQUIRE(std::dynamic_pointer_cast<NormalTextFileParser>(parser));

    REQUIRE_NOTHROW(parser = ParserFactory::getParser("xml"));
    REQUIRE(std::dynamic_pointer_cast<XMLTextFileParser>(parser));

    REQUIRE_THROWS_AS(ParserFactory::getParser("unsupported"), std::invalid_argument);
}