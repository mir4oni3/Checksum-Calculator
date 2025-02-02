#pragma once

#include "Parsers/FileHashParser.hpp"

#include <memory>
#include <istream>

class ParserFactory {
public:
    static std::shared_ptr<FileHashParser> getParser(std::istream& format);
};