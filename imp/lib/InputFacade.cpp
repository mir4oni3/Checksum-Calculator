#include "InputFacade.hpp"

#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

std::string InputFacade::getValidatedArg(TCLAP::ValueArg<std::string>& arg) const {
    std::string value = arg.getValue();
   if (arg.isSet() && !fs::exists(value)) {
        std::cerr << "Argument file does not exist" << std::endl;
        std::cerr << "Please rerun the program with correct arguments" << std::endl;
        exit(2);
    }

    if (arg.isSet() && !fs::is_regular_file(value) && !fs::is_directory(value)) {
        std::cerr << "Error: Argument is neither a file nor a directory: " << arg.getValue() << std::endl;
        std::cerr << "Please rerun the program with correct arguments" << std::endl;
        exit(3);
    }

    if (arg.isSet() && (fs::status(value).permissions() & fs::perms::owner_read) == fs::perms::none) {
        std::cerr << "Error: No read permissions for argument file: " << value << std::endl;
        std::cerr << "Please rerun the program with correct arguments" << std::endl;
        exit(4);
    }
    return value;
}

InputFacade::InputFacade(int argc, char** argv) {
    TCLAP::CmdLine cmd("Checksum Calculator");

    TCLAP::ValueArg<std::string> pathArg("p", "path", "Path to target", false, ".", "string", cmd);
    TCLAP::ValueArg<std::string> algorithmArg("a", "algorithm", "Checksum algorithm", false, "md5", "string", cmd);
    TCLAP::ValueArg<std::string> checksumsArg("c", "checksums", "Checksums to compare", false, "", "string", cmd);

    try {
        cmd.parse(argc, argv);
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        std::cerr << "Please rerun the program with correct arguments" << std::endl;
        exit(1);
    }

    this->checksums = getValidatedArg(checksumsArg);
    this->path = getValidatedArg(pathArg);
    this->algorithm = algorithmArg.getValue();
}

std::string InputFacade::getAlgorithm() const {
    return this->algorithm;
}

std::string InputFacade::getChecksums() const {
    return this->checksums;
}

std::string InputFacade::getPath() const {
    return this->path;
}