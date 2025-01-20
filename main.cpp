#include "imp/inc/InputFacade.hpp"

#include <iostream>

int main(int argc, char** argv) {
    InputFacade input(argc, argv);
    std::cout << "Algorithm: " << input.getAlgorithm() << std::endl;
    std::cout << "Checksums: " << input.getChecksums() << std::endl;
    std::cout << "Path: " << input.getPath() << std::endl;
    return 0;
}