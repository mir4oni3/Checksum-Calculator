#include "Factories/ParserFactory.hpp"
#include "Parsers/NormalTextFileParser.hpp"
#include "Parsers/XMLTextFileParser.hpp"

std::shared_ptr<FileHashParser> ParserFactory::getParser(const std::string& format) {
    if (format == "text") {
        return std::make_shared<NormalTextFileParser>();
    }
    if (format == "xml") {
        return std::make_shared<XMLTextFileParser>();
    }
    throw std::invalid_argument("FileParserFactory::getParser - Unsupported format");
    return nullptr;
}