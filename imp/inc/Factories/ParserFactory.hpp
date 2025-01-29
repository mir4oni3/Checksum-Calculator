#pragma once

#include "Parsers/FileHashParser.hpp"

#include <memory>

class ParserFactory {
public:
    static std::shared_ptr<FileHashParser> getParser(const std::string& format);
};