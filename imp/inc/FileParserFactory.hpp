#pragma once

#include "FileParser.hpp"

class FileParserFactory {
public:
    static std::shared_ptr<FileParser> getParser(const std::string& format);
};