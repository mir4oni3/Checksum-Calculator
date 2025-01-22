#pragma once

#include "../../resources/tclap/CmdLine.h"

#include <iostream>
#include <string>

class InputFacade {
    std::string path;
    std::string checksums;
    bool traverse;
    std::string algorithm;
    std::string output;
    bool buildChecksums;

public:
    InputFacade(int argc, char** argv);

    std::string getPath() const;
    std::string getChecksums() const;
    bool getTraverse() const;
    std::string getAlgorithm() const;
    std::string getOutput() const;
    bool getBuildChecksums() const;
    
private:
    std::string getValidatedArg(TCLAP::ValueArg<std::string>&) const;
};