#include "IO/InputFacade.hpp"
#include "IO/UserActions.hpp"

#include <iostream>

int main(int argc, char** argv) {
    InputFacade input(argc, argv);
    try {
        UserActions::start(input);
    } catch(const std::exception& e) {
        std::cerr << "Something went wrong:" << std::endl << e.what() << std::endl;
        return 1;
    }
}