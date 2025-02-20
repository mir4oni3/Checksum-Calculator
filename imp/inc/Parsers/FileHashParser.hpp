#pragma once

#include <unordered_map>
#include <string>
#include <istream>

namespace RegexConstants {
    const std::string hashRegex = "[a-fA-F0-9]+"; //hash sanity check
    const std::string filepathRegex = "([^\\/]*\\/)*[^\\/]+";

    const std::string textFileLineRegex = hashRegex + " \\*" + filepathRegex;

    const std::string xmlOpeningTagRegex = " *<([^<> ]+)> *";//first group
    const std::string xmlContentRegex = "([^<>]+)";//second group
    const std::string xmlClosingTagRegex = "<\\/\\1>";//backreference to the first group
    const std::string xmlOneLineElRegex = " *" + xmlOpeningTagRegex + xmlContentRegex + xmlClosingTagRegex + " *";
}

class FileHashParser {
protected:
    virtual void parseLine(const std::string&, std::unordered_map<std::string, std::string>&) const = 0;
public:
    //extract files and their checksums from a stream
    virtual std::unordered_map<std::string, std::string> parseFiles(std::istream& is) const;
};