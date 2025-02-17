#include <catch2/catch_test_macros.hpp>

#include "IO/InputFacade.hpp"
#include "TestResources/HelperFunctions.hpp"

TEST_CASE("TestInputFacadeConstructor", "[inputFacade]") {
    std::vector<std::string> args = { "test", "-p", ".", "-c", __FILE__, "-a", "SHA256", "-f", "xml", "-s", __FILE__ };

    std::vector<char*> argv; 
    for (auto& arg : args) {
        argv.push_back(arg.data());
    }
    int argc = argv.size();

    InputFacade input(argc, argv.data());

    REQUIRE(input.getPath() == ".");
    REQUIRE(input.getChecksums() == __FILE__);
    REQUIRE(input.getAlgorithm() == "SHA256");
    REQUIRE(input.getFormat() == "xml");
    REQUIRE(input.getSaveTo() == __FILE__);
    REQUIRE(input.getBuildChecksums() == false);
    REQUIRE(input.getTraverse() == true);
}

static char** getArgs(std::vector<std::string>& args) {
    static std::vector<char*> argv;
    argv.clear(); 
    for (auto& arg : args) {
        argv.push_back(arg.data());
    }
    return argv.data();
}

TEST_CASE("TestInputFacadeConstructorInvalidArgs", "[inputFacade]") {
    std::vector<std::string> args = {"test", "-p", "."};
    int argc = args.size();

    REQUIRE_NOTHROW(InputFacade(argc, getArgs(args)));

    //file doesn't exist
    args[2] = "Non-existant-file";
    REQUIRE_THROWS_AS(InputFacade(argc, getArgs(args)), std::invalid_argument);

    //file exists but directory is expected
    args[2] = __FILE__;
    REQUIRE_THROWS_AS(InputFacade(argc, getArgs(args)), std::invalid_argument);

    //file exists but regular file is expected
    args[1] = "-c";
    args[2] = ".";
    REQUIRE_THROWS_AS(InputFacade(argc, getArgs(args)), std::invalid_argument);

    //file exists and is regular, but no write permissions
    args[1] = "-s";
    args[2] = (HelperTestFunctions::getResourceFile("sampleTestDirectory") / "noPermFile.txt").string();
    REQUIRE_THROWS_AS(InputFacade(argc, getArgs(args)), std::invalid_argument);
}