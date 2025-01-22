#pragma once

#include "File.hpp"
#include "ChecksumCalculator.hpp"

#include <memory>
#include <unordered_map>
#include <string>
#include <iostream>


class FileParser {
protected:
    std::shared_ptr<ChecksumCalculator> calculator;
public:
    FileParser(const std::shared_ptr<ChecksumCalculator>&);
    virtual ~FileParser() = default;

    //extract files and their checksums from a stream
    static std::unordered_map<std::string, std::string> parseFiles(std::istream&);
    //write files and their checksums to a stream
    virtual void exportFile(std::ostream&, const std::shared_ptr<File>&) const = 0;

    std::shared_ptr<ChecksumCalculator> getCalculator() const;
    void setCalculator(const std::shared_ptr<ChecksumCalculator>&);
};

class XMLParser : public FileParser {
public:
    XMLParser(const std::shared_ptr<ChecksumCalculator>&);
    void exportFile(std::ostream&, const std::shared_ptr<File>&) const override;
};

class NormalTextParser : public FileParser {
public:
    NormalTextParser(const std::shared_ptr<ChecksumCalculator>&);
    void exportFile(std::ostream&, const std::shared_ptr<File>&) const override;
};