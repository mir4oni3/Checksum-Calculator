#include "Parsers/XMLTextFileParser.hpp"

#include <regex>

void XMLTextFileParser::parseLine(const std::string& line, std::unordered_map<std::string, std::string>& files) const {
    if (line == "") {
        return;
    }

    static const std::regex lineRegex(RegexConstants::xmlOneLineElRegex);
    static std::pair<std::string, std::string> toAdd;
    std::smatch match;

    if (std::regex_match(line, match, lineRegex)) {
        //current line is of the form <tag>content</tag>
        std::string tag = match[1];
        std::string content = match[2];
        if (tag == "path") {
            toAdd.first = content;
        }
        if (tag == "checksum") {
            toAdd.second = content;
        }
    }

    static const std::regex openingTagRegex(RegexConstants::xmlOpeningTagRegex);
    if (std::regex_match(line, match, openingTagRegex) && match[1] == "item") {
        //current line is an opening tag for a new item, clear current item
        toAdd = {};
    }

    if (toAdd.first != "" && toAdd.second != "") {
        files.insert(toAdd);
        toAdd = {};
    }
}