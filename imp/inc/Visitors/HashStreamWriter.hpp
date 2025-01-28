#pragma once

#include "VisitorWriter.hpp"

#include <sstream>

namespace RegexConstants {
    //filenames with \n are not allowed, hex letters need to be small
    const std::string hashRegex = "[a-f0-9]+";
    const std::string filepathRegex = "([^\\/]*\\/)*[^\\/]+";

    const std::string textFileLineRegex = hashRegex + " \\*" + filepathRegex;

    const std::string xmlOpeningTagRegex = " *<([^<> ]+)> *";//first group
    const std::string xmlContentRegex = "([^<>]+)";//second group
    const std::string xmlClosingTagRegex = "<\\/\\1>";//backreference to the first group
    const std::string xmlOneLineElRegex = " *" + xmlOpeningTagRegex + xmlContentRegex + xmlClosingTagRegex + " *";
}

class HashStreamWriter : public VisitorWriter {
    virtual void parseLine(const std::string&, std::unordered_map<std::string, std::string>&) const = 0;

protected:
    const std::shared_ptr<ChecksumCalculator> calc;
    mutable std::ostringstream tempStream;
public:
    HashStreamWriter(std::ostream& os, const std::shared_ptr<ChecksumCalculator>& calc);

    virtual void visitRegularFile(const RegularFile&) const override = 0;

    //extract files and their checksums from a stream
    virtual std::unordered_map<std::string, std::string> parseFiles(std::istream& is) const;

    virtual void finalizeExport() const override;
};