#include <catch2/catch_test_macros.hpp>

#include "Files/File.hpp"
#include "Calculators/MD5Calculator.hpp"

class FileStub : public File {
public:
    FileStub(const std::string& path) : File(path) {}
    void accept(const VisitorWriter& writer) const override {}
};

class RegularFileStub : public RegularFile {
public:
    RegularFileStub(const std::string& path) : RegularFile(path) {}
    void accept(const VisitorWriter& writer) const override {}
    void setChecksum(const std::string& checksum) {
        this->checksum = checksum;
    }
};

TEST_CASE("TestFileConstructorAndGetPath", "[File]") {
    REQUIRE_THROWS_AS(FileStub(""), std::invalid_argument);

    std::shared_ptr<FileStub> file;
    REQUIRE_NOTHROW(file = std::make_shared<FileStub>(__FILE__));
    REQUIRE(file->getPath() == __FILE__);
}

TEST_CASE("TestFileGetSetSize", "[File]") {
    FileStub file(__FILE__);
    REQUIRE_THROWS_AS(file.getSize(), std::logic_error);
    REQUIRE_NOTHROW(file.setSize(10));
    
    size_t filesize;
    REQUIRE_NOTHROW(filesize = file.getSize());
    REQUIRE(filesize == 10);
}

TEST_CASE("TestRegularFileConstructor", "[RegularFile]") {
    REQUIRE_THROWS_AS(RegularFile(""), std::invalid_argument);
    REQUIRE_NOTHROW(RegularFile(__FILE__));

    //file exists but is not regular file
    REQUIRE_THROWS_AS(RegularFile("."), std::invalid_argument);
}

TEST_CASE("TestRegularFileGetChecksum", "[RegularFile]") {
    RegularFileStub file(__FILE__);
    std::shared_ptr<MD5Calculator> calc = std::make_shared<MD5Calculator>();
    std::string checksum;
    std::ifstream fileStream(__FILE__, std::ios::binary);

    //nullptr arg
    REQUIRE_THROWS_AS(file.getChecksum(nullptr), std::invalid_argument);

    //checksum is already set, no new calculations
    file.setChecksum("test");
    REQUIRE_NOTHROW(checksum = file.getChecksum(calc));
    REQUIRE(checksum == "test");

    //checksum is not set, calculate
    file.setChecksum("");
    REQUIRE_NOTHROW(checksum = file.getChecksum(calc));
    REQUIRE(checksum == calc->calculate(fileStream));
}

TEST_CASE("TestDirectoryConstructor", "[Directory]") {
    REQUIRE_THROWS_AS(Directory(""), std::invalid_argument);
    REQUIRE_NOTHROW(Directory("."));

    //file exists but is not directory
    REQUIRE_THROWS_AS(Directory(__FILE__), std::invalid_argument);
}

TEST_CASE("TestDirectoryAddGetFiles", "[Directory]") {
    Directory dir(".");
    std::shared_ptr<File> file;
    
    //nullptr arg
    file = nullptr;
    REQUIRE_THROWS_AS(dir.addFile(file), std::invalid_argument);

    file = std::make_shared<RegularFile>(__FILE__);

    REQUIRE_NOTHROW(dir.addFile(file));
    std::vector<std::shared_ptr<File>> files;
    REQUIRE_NOTHROW(files = dir.getFiles());
    REQUIRE(files.size() == 1);
    REQUIRE(files[0] == file);
}