#pragma once

#include "FileParser.hpp"

class XMLParser : public FileParser {
    void parseLine(const std::string&, std::unordered_map<std::string, std::string>&) const override;
public:
    void exportFile(const std::shared_ptr<File>&, std::ostream&, const std::shared_ptr<ChecksumCalculator>&) const override;
};