#pragma once

#include "File.hpp"
#include "ChecksumCalculator.hpp"

#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>

namespace ParserConstants {
    //filenames with \n are not allowed, hex letters need to be small
    const std::string fileLineRegex = "^[a-f0-9]+ \\*(\\/[^\\/\n]*)+$";
}

class FileParser {
public:
    virtual ~FileParser() = default;

    //extract files and their checksums from a stream
    virtual std::unordered_map<std::string, std::string> parseFiles(std::istream&) = 0;
    //write files and their checksums to a stream
    virtual void exportFile(const std::shared_ptr<File>&, std::ostream&, const std::shared_ptr<ChecksumCalculator>&) const = 0;
};

class XMLParser : public FileParser {
public:
    std::unordered_map<std::string, std::string> parseFiles(std::istream&) override;
    void exportFile(const std::shared_ptr<File>&, std::ostream&, const std::shared_ptr<ChecksumCalculator>&) const override;
};

class NormalTextParser : public FileParser {
    std::pair<std::string, std::string> parseLine(const std::string& line) const;
public:
    std::unordered_map<std::string, std::string> parseFiles(std::istream&) override;
    void exportFile(const std::shared_ptr<File>&, std::ostream&, const std::shared_ptr<ChecksumCalculator>&) const override;
};