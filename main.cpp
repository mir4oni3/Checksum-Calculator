#include "IO/InputFacade.hpp"
#include "IO/UserActions.hpp"

#include <iostream>

int main(int argc, char** argv) {
    InputFacade input(argc, argv);
    UserActions::start(input);
}