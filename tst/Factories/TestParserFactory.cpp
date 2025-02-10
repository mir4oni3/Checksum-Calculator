#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>

#include "Factories/ParserFactory.hpp"
#include "Parsers/NormalTextFileParser.hpp"
#include "Parsers/XMLTextFileParser.hpp"
#include "TestResources/HelperFunctions.hpp"

TEST_CASE("TestParserFactoryGetParser", "[ParserFactory]") {
    std::shared_ptr<FileHashParser> parser;
    std::string xmlFilePath = HelperTestFunctions::getResourceFilePath("sampleXMLfile.txt");
    std::string normalTextFilePath = HelperTestFunctions::getResourceFilePath("sampleNormalTextFile.txt");
    std::string giberrishFilePath = HelperTestFunctions::getResourceFilePath("gibberishFile.txt");

    std::ifstream xmlStream(xmlFilePath);
    REQUIRE_NOTHROW(parser = ParserFactory::getParser(xmlStream));
    REQUIRE(std::dynamic_pointer_cast<XMLTextFileParser>(parser));

    std::ifstream textStream(normalTextFilePath);
    REQUIRE_NOTHROW(parser = ParserFactory::getParser(textStream));
    REQUIRE(std::dynamic_pointer_cast<NormalTextFileParser>(parser));

    std::ifstream emptyStream(giberrishFilePath);
    REQUIRE_THROWS_AS(ParserFactory::getParser(emptyStream), std::invalid_argument);
}