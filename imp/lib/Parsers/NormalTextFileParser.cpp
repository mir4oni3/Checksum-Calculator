#include "Parsers/NormalTextFileParser.hpp"

#include <regex>
#include <sstream>

void NormalTextFileParser::parseLine(const std::string& line, std::unordered_map<std::string, std::string>& files) const {
    if (line == "") {
        return;
    }

    static const std::regex lineRegex(RegexConstants::textFileLineRegex);

    if (!std::regex_match(line, lineRegex)) {
        throw std::runtime_error(
            "NormalTextStreamWriter::parseFiles - Invalid file line format for: "
            + line +
            "\n File format should be in the plain text format "
            "used in the *sum family of applications in Linux"
        );
    }

    std::istringstream iss(line);
    std::string checksum;
    std::string path;

    iss >> checksum >> path;

    if (iss.bad()) {
        throw std::runtime_error("Error parsing line: " + line);
    }

    //remove the * from the path
    path = path.substr(1);

    files.insert({path, checksum});
}
