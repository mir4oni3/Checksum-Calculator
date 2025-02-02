#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "Factories/ParserFactory.hpp"
#include "Parsers/NormalTextFileParser.hpp"
#include "Parsers/XMLTextFileParser.hpp"


static std::string getResourceFilePath(const std::string& filename) {
    std::filesystem::path sourcePath = __FILE__; 
    std::filesystem::path sourceDir = sourcePath.parent_path().parent_path();  

    std::filesystem::path xmlPath = sourceDir / "TestResources";
    xmlPath = xmlPath / filename;
    return xmlPath.string();
}

TEST_CASE("TestParserFactoryGetParser", "[ParserFactory]") {
    std::shared_ptr<FileHashParser> parser;

    std::ifstream xmlStream(getResourceFilePath("sampleXMLfile.txt"));
    REQUIRE_NOTHROW(parser = ParserFactory::getParser(xmlStream));
    REQUIRE(std::dynamic_pointer_cast<XMLTextFileParser>(parser));

    std::ifstream textStream(getResourceFilePath("sampleNormalTextFile.txt"));
    REQUIRE_NOTHROW(parser = ParserFactory::getParser(textStream));
    REQUIRE(std::dynamic_pointer_cast<NormalTextFileParser>(parser));

    std::istringstream emptyStream(getResourceFilePath("gibberishFile.txt"));
    REQUIRE_THROWS_AS(ParserFactory::getParser(emptyStream), std::invalid_argument);
}