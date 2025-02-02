#include <catch2/catch_test_macros.hpp>

#include "Factories/FileSystemBuilderFactory.hpp"

TEST_CASE("TestFileSystemBuilderFactoryGetBuilder", "[FileSystemBuilderFactory]") {
    std::shared_ptr<FileSystemBuilder> builder;

    REQUIRE_NOTHROW(builder = FileSystemBuilderFactory::getBuilder(true));
    REQUIRE(std::dynamic_pointer_cast<TraverseSymlinkFileSystemBuilder>(builder));

    REQUIRE_NOTHROW(builder = FileSystemBuilderFactory::getBuilder(false));
    REQUIRE(std::dynamic_pointer_cast<IgnoreSymlinkFileSystemBuilder>(builder));
}