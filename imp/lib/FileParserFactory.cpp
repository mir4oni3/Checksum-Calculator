#include "../inc/FileParserFactory.hpp"
#include "../inc/NormalTextParser.hpp"
#include "../inc/XMLParser.hpp"

std::shared_ptr<FileParser> FileParserFactory::getParser(const std::string& format) {
    if (format == "text") {
        return std::make_shared<NormalTextParser>();
    }
    if (format == "xml") {
        return std::make_shared<XMLParser>();
    }
    throw std::invalid_argument("FileParserFactory::getParser - Unsupported format");
    return nullptr;
}