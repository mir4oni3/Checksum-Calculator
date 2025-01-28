#include "Visitors/HashStreamWriter.hpp"

HashStreamWriter::HashStreamWriter(std::ostream& os, const std::shared_ptr<ChecksumCalculator>& calc) : VisitorWriter(os), calc(calc) {
    if (!calc) {
        throw std::invalid_argument("HashStreamWriter::HashStreamWriter - Invalid calculator passed");
    }

    // Adding two newline characters to separate sections in the output stream
    tempStream << "\n\n";
}

std::unordered_map<std::string, std::string> HashStreamWriter::parseFiles(std::istream& is) const {
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

void HashStreamWriter::finalizeExport() const {
    os << tempStream.str();
    tempStream.str("");
}

//the Observer should receive messages from calculator as well
void HashStreamWriter::addObserver(const std::shared_ptr<Observer>& observer) {
    VisitorWriter::addObserver(observer);
    calc->addObserver(observer);
}