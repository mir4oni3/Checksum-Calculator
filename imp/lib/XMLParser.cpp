#include "../inc/XMLParser.hpp"
#include "../inc/FileIterator.hpp"

#include <regex>
#include <sstream>

//simple parser to extract filepaths and their checksums from an XML file
//we assume the elements for extracting are on a single line, i.e. <tag>content</tag>
void XMLParser::parseLine(const std::string& line, std::unordered_map<std::string, std::string>& files) const {
    if (line == "") {
        return;
    }

    static const std::regex lineRegex(RegexConstants::xmlOneLineElRegex);
    static std::pair<std::string, std::string> toAdd;

    std::smatch match;
    if (std::regex_match(line, match, lineRegex)) {
        //current line is of the form <tag>content</tag>
        std::string tag = match[1];
        std::string content = match[2];
        if (tag == "path") {
            toAdd.first = content;
        }
        if (tag == "checksum") {
            toAdd.second = content;
        }
    }

    if (toAdd.first != "" && toAdd.second != "") {
        files.insert(toAdd);
        toAdd = {};
    }
}

void XMLParser::exportFile(
            const std::shared_ptr<File>& file,
            std::ostream& os,
            const std::shared_ptr<ChecksumCalculator>& calc) const
{
    if (!file || !os || !calc) {
        throw std::invalid_argument("NormalTextParser::exportFile - invalid arg(s)");
    }

    FileIterator it(file);

    os << "<checksums>\n";
    while (it.hasMore()) {
        std::shared_ptr<RegularFile> regFile = std::dynamic_pointer_cast<RegularFile>(it.next());
        if (!regFile) {
            continue;
        }

        os << "    <item>\n";
        os << "        <mode>binary</mode>\n";
        os << "        <checksum>" << regFile->getChecksum(calc) << "</checksum>\n";
        os << "        <path>" << regFile->getPath() << "</path>\n";
        os << "        <size>" << regFile->getSize() << "</size>\n";
        os << "    </item>\n";
    }
    os << "</checksums>\n";    
}