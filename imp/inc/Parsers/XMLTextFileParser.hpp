#pragma once

#include "Parsers/FileHashParser.hpp"

class XMLTextFileParser : public FileHashParser {
protected:
    void parseLine(const std::string&, std::unordered_map<std::string, std::string>&) const override;
};