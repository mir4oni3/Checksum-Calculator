#pragma once

#include "Parsers/FileHashParser.hpp"

class NormalTextFileParser : public FileHashParser {
    void parseLine(const std::string&, std::unordered_map<std::string, std::string>&) const override;
};
