#include "Parsers/FileHashParser.hpp"

std::unordered_map<std::string, std::string> FileHashParser::parseFiles(std::istream& is) const {
    if (!is) {
        throw std::invalid_argument("FileParser::parseFiles - Invalid stream passed");
    }

    std::unordered_map<std::string, std::string> files;
    std::string line;

    while (std::getline(is, line)) {
        parseLine(line, files);
    }

    if (is.bad()) {
        throw std::runtime_error("FileParser::parseFiles - Error reading from stream");
    }

    is.clear();
    is.seekg(0, std::ios::beg);
    is.clear();
    
    return files;
}
