#include <catch2/catch_test_macros.hpp>

#include "Parsers/NormalTextFileParser.hpp"

class TestNormalTextFileParser : public NormalTextFileParser {
public:
    void parseLine(const std::string& line, std::unordered_map<std::string, std::string>& files) const override {
        NormalTextFileParser::parseLine(line, files);
    }
};

TEST_CASE("TestParseLine", "[normal_text_file_parser]") {
    TestNormalTextFileParser parser;
    std::unordered_map<std::string, std::string> files;

    SECTION("Empty line") {
        REQUIRE_NOTHROW(parser.parseLine("", files));
        REQUIRE(files.empty());
    }

    SECTION("Invalid line") {
        REQUIRE_THROWS_AS(parser.parseLine("invalid line", files), std::runtime_error);
        REQUIRE_THROWS_AS(parser.parseLine("invalid@hash *valid/path", files), std::runtime_error);
        REQUIRE_THROWS_AS(parser.parseLine("123cccc4444 *invalid/path/", files), std::runtime_error);
    }

    SECTION("Valid line") {
        REQUIRE_NOTHROW(parser.parseLine("acd123 *path/to/file", files));
        REQUIRE_NOTHROW(parser.parseLine("fccad *path////to///file", files));
        REQUIRE_NOTHROW(parser.parseLine("fff333 *path/to/another/file", files));
        REQUIRE(files.size() == 3);
        REQUIRE(files.find("path/to/file") != files.end());
        REQUIRE(files.find("path/to/another/file") != files.end());
        REQUIRE(files.find("path////to///file") != files.end());
        REQUIRE(files["path/to/file"] == "acd123");
        REQUIRE(files["path/to/another/file"] == "fff333");
        REQUIRE(files["path////to///file"] == "fccad");
    }
}