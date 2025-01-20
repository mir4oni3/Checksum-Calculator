#include "../../tclap/CmdLine.h"

#include <iostream>
#include <string>

class InputFacade {
    std::string algorithm;
    std::string checksums;
    std::string path;
public:
    InputFacade(int argc, char** argv);
    std::string getAlgorithm() const;
    std::string getChecksums() const;
    std::string getPath() const;
private:
    std::string getValidatedArg(TCLAP::ValueArg<std::string>&) const;
};