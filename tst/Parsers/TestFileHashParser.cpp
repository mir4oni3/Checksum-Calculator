#include <catch2/catch_test_macros.hpp>

#include "Parsers/FileHashParser.hpp"

#include <sstream>

class TestFileHashParser : public FileHashParser {
    void parseLine(const std::string& line, std::unordered_map<std::string, std::string>& files) const override {
        files[line] = "";
    }
};

TEST_CASE("TestParseFiles", "[file_hash_parser]") {
    TestFileHashParser parser;
    
    SECTION("Bad stream") {
        std::istringstream is;
        is.setstate(std::ios::badbit);
        REQUIRE_THROWS_AS(parser.parseFiles(is), std::invalid_argument);
    }

    SECTION("Invalid stream") {
        std::unordered_map<std::string, std::string> files;
        std::stringstream ss;
        ss << "line1\nline2\nline3\n";

        REQUIRE_NOTHROW(files = parser.parseFiles(ss));
        REQUIRE(files.size() == 3);
        REQUIRE(files.find("line1") != files.end());
        REQUIRE(files.find("line2") != files.end());
        REQUIRE(files.find("line3") != files.end());
    }
}