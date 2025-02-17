#include <catch2/catch_test_macros.hpp>

#include "Files/FileSystemBuilder.hpp"
#include "TestResources/HelperFunctions.hpp"

TEST_CASE("TestFileSystemBuilderBuild", "[fs_builder]") {
    FileSystemBuilder builder;
    builder.saveFullPaths(true);
    builder.setSortFiles(true);

    std::string path = HelperTestFunctions::getResourceFilePath("sampleTestDirectory");
    std::unique_ptr<File> file;
    REQUIRE_NOTHROW(file = builder.build(path));
    REQUIRE(file != nullptr);

    REQUIRE(file->getPath() == HelperTestFunctions::getResourceFilePath("sampleTestDirectory"));
    REQUIRE(file->getSize() == 7168);

    //check if the files were built correctly
    Directory* dir = dynamic_cast<Directory*>(file.get());
    REQUIRE(dir);
    const std::vector<std::unique_ptr<File>>& files = dir->getFiles();
    REQUIRE(files.size() == 5);

    std::filesystem::path resourcePath = HelperTestFunctions::getResourceFile("sampleTestDirectory");
    std::filesystem::path expected;
    //check the gibberish files
    for (int i = 0;i < 3;i++) {
        expected = resourcePath / ("gibberishFile" + std::to_string(i + 1) + ".txt");
        REQUIRE(files[i]->getPath() == expected.string());
        REQUIRE(files[i]->getSize() == 1024);
    }

    //check sampleDirs
    expected = resourcePath / "sampleDir1";
    REQUIRE(files[3]->getPath() == expected.string());
    REQUIRE(files[3]->getSize() == 2048);

    expected = resourcePath / "sampleDir2";
    REQUIRE(files[4]->getPath() == expected.string());
    REQUIRE(files[4]->getSize() == 2048);

    //check the sampleDir1 files
    dir = dynamic_cast<Directory*>(files[3].get());
    REQUIRE(dir);
    const std::vector<std::unique_ptr<File>>& sampleDir1Files = dir->getFiles();
    REQUIRE(sampleDir1Files.size() == 2);

    expected = resourcePath / "sampleDir1" / "gibberishFile4.txt";
    REQUIRE(sampleDir1Files[0]->getPath() == expected.string());
    REQUIRE(sampleDir1Files[0]->getSize() == 1024);
    expected = resourcePath / "sampleDir1" / "gibberishFile5.txt";
    REQUIRE(sampleDir1Files[1]->getPath() == expected.string());
    REQUIRE(sampleDir1Files[1]->getSize() == 1024);

    //check the sampleDir2 files
    dir = dynamic_cast<Directory*>(files[4].get());
    REQUIRE(dir);
    const std::vector<std::unique_ptr<File>>& sampleDir2Files = dir->getFiles();
    REQUIRE(sampleDir2Files.size() == 2);

    expected = resourcePath / "sampleDir2" / "gibberishFile6.txt";
    REQUIRE(sampleDir2Files[0]->getPath() == expected.string());
    REQUIRE(sampleDir2Files[0]->getSize() == 1024);
    expected = resourcePath / "sampleDir2" / "gibberishFile7.txt";
    REQUIRE(sampleDir2Files[1]->getPath() == expected.string());
    REQUIRE(sampleDir1Files[1]->getSize() == 1024);
}