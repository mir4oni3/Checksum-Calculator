#pragma once

#include "Parsers/FileHashParser.hpp"

#include <memory>
#include <istream>

class ParserFactory {
public:
    static std::unique_ptr<FileHashParser> getParser(std::istream& format);
};