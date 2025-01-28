#pragma once

#include "tclap/CmdLine.h"

#include <iostream>
#include <string>

class InputFacade {
    std::string path;
    std::string checksums;
    bool traverse;
    std::string algorithm;
    std::string format;
    bool buildChecksums;
    std::string saveTo;

public:
    InputFacade(int argc, char** argv);

    std::string getPath() const;
    std::string getChecksums() const;
    bool getTraverse() const;
    std::string getAlgorithm() const;
    std::string getFormat() const;
    bool getBuildChecksums() const;
    std::string getSaveTo() const;
    
private:
    std::string getValidatedArg(TCLAP::ValueArg<std::string>&) const;
};