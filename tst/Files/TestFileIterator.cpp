#include <catch2/catch_test_macros.hpp>

#include "Files/FileIterator.hpp"
#include "Files/FileSystemBuilder.hpp"
#include "TestResources/HelperFunctions.hpp"

TEST_CASE("TestFileIteratorConstructor", "[fileIterator]") {
    std::shared_ptr<File> file = std::make_shared<RegularFile>(__FILE__);
    std::shared_ptr<FileIterator> fileIterator;
    REQUIRE_NOTHROW(fileIterator = std::make_shared<FileIterator>(*file));
}

TEST_CASE("TestFileIteratorNextRegularFile", "[fileIterator]") {
    RegularFile file(__FILE__);
    FileIterator fileIterator(file);
    const File* nextFile;

    REQUIRE(fileIterator.hasNext());
    REQUIRE_NOTHROW(nextFile = &fileIterator.next());
    REQUIRE(nextFile == &file);

    REQUIRE(!fileIterator.hasNext());
    REQUIRE_THROWS_AS(fileIterator.next(), std::invalid_argument);
}

//these files with random content were created with the following bash command:
// for i in {1..7}; do     dd if=/dev/urandom of=gibberishFile${i}.txt bs=1K count=1; done
TEST_CASE("TestFileIteratorNextDirectory", "[fileIterator]") {
    FileSystemBuilder builder;
    builder.saveFullPaths(true);
    builder.setSortFiles(true);
    std::string path = HelperTestFunctions::getResourceFilePath("sampleTestDirectory");
    std::unique_ptr<File> file = builder.build(path);
    FileIterator fileIterator(*file);
    const File* nextFile;
    std::string expected;

    //Note: Builder builds files in alphabetical order, so
    //the process is deterministic and can be tested this way

    //first file is the directory itself
    std::filesystem::path sampleTestDirectory = std::filesystem::absolute(HelperTestFunctions::getResourceFile("sampleTestDirectory"));

    REQUIRE(fileIterator.hasNext());
    REQUIRE_NOTHROW(nextFile = &fileIterator.next());
    expected = sampleTestDirectory.string();
    REQUIRE(nextFile != nullptr);
    REQUIRE(nextFile->getPath() == expected);

    //next files are the gibberish files
    
    REQUIRE(fileIterator.hasNext());
    REQUIRE_NOTHROW(nextFile = &fileIterator.next());
    expected = (sampleTestDirectory / "gibberishFile1.txt").string();
    REQUIRE(nextFile != nullptr);
    REQUIRE(nextFile->getPath() == expected);

    REQUIRE(fileIterator.hasNext());
    REQUIRE_NOTHROW(nextFile = &fileIterator.next());
    expected = (sampleTestDirectory / "gibberishFile2.txt").string();
    REQUIRE(nextFile != nullptr);
    REQUIRE(nextFile->getPath() == expected);

    REQUIRE(fileIterator.hasNext());
    REQUIRE_NOTHROW(nextFile = &fileIterator.next());
    expected = (sampleTestDirectory / "gibberishFile3.txt").string();
    REQUIRE(nextFile != nullptr);
    REQUIRE(nextFile->getPath() == expected);

    //next is the sampleDir1 and its subfiles
    std::filesystem::path sampleDir1 = sampleTestDirectory / "sampleDir1";

    REQUIRE(fileIterator.hasNext());
    REQUIRE_NOTHROW(nextFile = &fileIterator.next());
    expected = sampleDir1.string();
    REQUIRE(nextFile != nullptr);
    REQUIRE(nextFile->getPath() == expected);

    REQUIRE(fileIterator.hasNext());
    REQUIRE_NOTHROW(nextFile = &fileIterator.next());
    expected = (sampleDir1 / "gibberishFile4.txt").string();
    REQUIRE(nextFile != nullptr);
    REQUIRE(nextFile->getPath() == expected);

    REQUIRE(fileIterator.hasNext());
    REQUIRE_NOTHROW(nextFile = &fileIterator.next());
    expected = (sampleDir1 / "gibberishFile5.txt").string();
    REQUIRE(nextFile != nullptr);
    REQUIRE(nextFile->getPath() == expected);

    //next is the sampleDir2 and its subfiles
    std::filesystem::path sampleDir2 = sampleTestDirectory / "sampleDir2";

    REQUIRE(fileIterator.hasNext());
    REQUIRE_NOTHROW(nextFile = &fileIterator.next());
    expected = sampleDir2.string();
    REQUIRE(nextFile != nullptr);
    REQUIRE(nextFile->getPath() == expected);

    REQUIRE(fileIterator.hasNext());
    REQUIRE_NOTHROW(nextFile = &fileIterator.next());
    expected = (sampleDir2 / "gibberishFile6.txt").string();
    REQUIRE(nextFile != nullptr);
    REQUIRE(nextFile->getPath() == expected);

    REQUIRE(fileIterator.hasNext());
    REQUIRE_NOTHROW(nextFile = &fileIterator.next());
    expected = (sampleDir2 / "gibberishFile7.txt").string();
    REQUIRE(nextFile != nullptr);
    REQUIRE(nextFile->getPath() == expected);

    //we traversed the files, now nullptr should be returned
    REQUIRE(!fileIterator.hasNext());
    REQUIRE_THROWS_AS(fileIterator.next(), std::invalid_argument);
}