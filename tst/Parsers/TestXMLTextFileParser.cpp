#include <catch2/catch_test_macros.hpp>

#include "Parsers/XMLTextFileParser.hpp"

class TestXMLTextFileParser : public XMLTextFileParser {
public:
    void parseLine(const std::string& line, std::unordered_map<std::string, std::string>& files) const override {
        XMLTextFileParser::parseLine(line, files);
    }
};

TEST_CASE("TestParseLine", "[XML_text_file_parser]") {
    TestXMLTextFileParser parser;
    std::unordered_map<std::string, std::string> files;

    SECTION("Empty line") {
        REQUIRE_NOTHROW(parser.parseLine("", files));
        REQUIRE(files.empty());
    }

    SECTION("Invalid path") {
        REQUIRE_NOTHROW(parser.parseLine("<item>", files));
        REQUIRE(files.empty());
        REQUIRE_NOTHROW(parser.parseLine("<path>non_existent_file</path>", files));
        REQUIRE(files.empty());
        REQUIRE_NOTHROW(parser.parseLine("<checksum>abcd1234</checksum>", files));
        REQUIRE(files.empty());
        REQUIRE_NOTHROW(parser.parseLine("</item>", files));
        REQUIRE(files.empty());
    }

    SECTION("Checksum sanity check not passed") {
        REQUIRE_NOTHROW(parser.parseLine("<item>", files));
        REQUIRE(files.empty());
        REQUIRE_NOTHROW(parser.parseLine("<path>.</path>", files));
        REQUIRE(files.empty());
        REQUIRE_NOTHROW(parser.parseLine("<checksum>invalid checksum</checksum>", files));
        REQUIRE(files.empty());
        REQUIRE_NOTHROW(parser.parseLine("</item>", files));
        REQUIRE(files.empty());
    }

    SECTION("Different item test") {
        REQUIRE_NOTHROW(parser.parseLine("<item>", files));
        REQUIRE(files.empty());
        REQUIRE_NOTHROW(parser.parseLine("<path>.</path>", files));
        REQUIRE(files.empty());
        REQUIRE_NOTHROW(parser.parseLine("123cccc4444 *some/str", files));
        REQUIRE(files.empty());
        REQUIRE_NOTHROW(parser.parseLine("</item>", files));
        REQUIRE(files.empty());
        REQUIRE_NOTHROW(parser.parseLine("<item>", files));
        REQUIRE(files.empty());
        REQUIRE_NOTHROW(parser.parseLine("<checksum>abcd1234</checksum>", files));
        REQUIRE(files.empty()); //checksum and path provided, but in different items
        REQUIRE_NOTHROW(parser.parseLine("</item>", files));
        REQUIRE(files.empty());
    }

    SECTION("Valid line") {
        REQUIRE_NOTHROW(parser.parseLine("  <item>  ", files));
        REQUIRE_NOTHROW(parser.parseLine("  <path>.</path>  ", files));
        REQUIRE_NOTHROW(parser.parseLine("  <checksum>acd123</checksum>  ", files));
        REQUIRE_NOTHROW(parser.parseLine("  <item>  ", files));
        REQUIRE(files.size() == 1);
        REQUIRE(files.at(".") == "acd123");
    }

    SECTION("Half valid item") {
        REQUIRE_NOTHROW(parser.parseLine("  <item>  ", files));
        REQUIRE_NOTHROW(parser.parseLine("  <path>.</path>  ", files));
        REQUIRE_NOTHROW(parser.parseLine("  </item>  ", files));
        REQUIRE(files.empty());
        REQUIRE_NOTHROW(parser.parseLine("  <item>  ", files));
        REQUIRE_NOTHROW(parser.parseLine("  <path>..</path>  ", files));
        REQUIRE_NOTHROW(parser.parseLine("  <checksum>acd123</checksum>  ", files));
        REQUIRE_NOTHROW(parser.parseLine("  </item>  ", files));
        REQUIRE(files.size() == 1);
        REQUIRE(files.at("..") == "acd123");
    }
}