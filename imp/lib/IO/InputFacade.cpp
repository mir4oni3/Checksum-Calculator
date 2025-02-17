#include "IO/InputFacade.hpp"

#include <cstdlib>

std::string InputFacade::getValidatedArg(TCLAP::ValueArg<std::string>& arg, bool isRegular, fs::perms perms = fs::perms::owner_read) const {
    std::string value = arg.getValue();
    if (!arg.isSet()) {
        return value;
    }
    
    if (!fs::exists(value)) {
        throw std::invalid_argument("Argument file does not exist");
    }
   
    if (isRegular && !fs::is_regular_file(value)) {
       throw std::invalid_argument("Error: Argument is not a regular file but it should be: " + arg.getValue());
    }

    if (!isRegular && !fs::is_directory(value)) {
        throw std::invalid_argument("Error: Argument is not a directory but it should be: " + arg.getValue());
    }

    if ((fs::status(value).permissions() & perms) != perms) {
       throw std::invalid_argument("Error: Incorrect permissions for argument file: " + arg.getValue());
    }

    return value;
}

InputFacade::InputFacade(int argc, char** argv) {
    TCLAP::CmdLine cmd("Checksum Calculator");

    TCLAP::ValueArg<std::string> pathArg("p", "path", "Path to target", false, ".", "string", cmd);
    TCLAP::ValueArg<std::string> checksumsArg("c", "checksums", "Checksums to compare", false, "", "string", cmd);
    TCLAP::SwitchArg noTraverseArg("", "no-traverse", "Don't traverse symlinks", cmd, false);
    TCLAP::ValueArg<std::string> algorithmArg("a", "algorithm", "Checksum algorithm", false, "md5", "string", cmd);
    TCLAP::ValueArg<std::string> formatArg("f", "format", "Output format", false, "text", "string", cmd);
    TCLAP::SwitchArg buildChecksumsArg("", "build-checksums", "Calculate checksums when building the files", cmd, false);
    TCLAP::ValueArg<std::string> saveToArg("s", "save-to", "Save checksums to file", false, "", "string", cmd);

    try {
        cmd.parse(argc, argv);
    } catch (TCLAP::ArgException &e) {
        throw std::invalid_argument(e.what());
    }

    //validated here
    this->checksums = getValidatedArg(checksumsArg, true);
    this->path = getValidatedArg(pathArg, false);
    this->traverse = !noTraverseArg.getValue();
    this->buildChecksums = buildChecksumsArg.getValue();
    this->saveTo = getValidatedArg(saveToArg, true, fs::perms::owner_write);

    //validated later
    this->format = formatArg.getValue();
    this->algorithm = algorithmArg.getValue();
}

std::string InputFacade::getPath() const {
    return this->path;
}

std::string InputFacade::getChecksums() const {
    return this->checksums;
}

bool InputFacade::getTraverse() const {
    return this->traverse;
}

std::string InputFacade::getAlgorithm() const {
    return this->algorithm;
}

std::string InputFacade::getFormat() const {
    return this->format;
}

bool InputFacade::getBuildChecksums() const {
    return this->buildChecksums;
}

std::string InputFacade::getSaveTo() const {
    return this->saveTo;
}