#include "Factories/ParserFactory.hpp"
#include "Parsers/NormalTextFileParser.hpp"
#include "Parsers/XMLTextFileParser.hpp"

#include <regex>

std::unique_ptr<FileHashParser> ParserFactory::getParser(std::istream& stream) {
    //format is determined by stream content
    std::string firstLine;
    while (firstLine == "") {
        std::getline(stream, firstLine);
    }
    
    //first line begins with xml opening tag
    if (std::regex_match(firstLine, std::regex(RegexConstants::xmlOpeningTagRegex + ".*"))) {
        return std::make_unique<XMLTextFileParser>();
    }

    //first line is a text file line
    if (std::regex_match(firstLine, std::regex(RegexConstants::textFileLineRegex))) {
        return std::make_unique<NormalTextFileParser>();
    }

    //unknown format
    throw std::invalid_argument("ParserFactory::getParser - Unsupported format");
    return nullptr;
}