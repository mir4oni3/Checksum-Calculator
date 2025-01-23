#pragma once

#include "ChecksumCalculator.hpp"

#include <vector>
#include <memory>
#include <string>

//NOTE: Intended to be built by the FileSystemBuilder
class File {
    std::string path;
    size_t size = 0;
    bool sizeKnown = false;
public:
    File(const std::string& path);
    virtual ~File() = default;

    void setSize(size_t size);
    size_t getSize() const;
    std::string getPath() const;
};

class RegularFile : public File {
    mutable std::string checksum;
public:
    RegularFile(const std::string& path);
    std::string getChecksum(const std::shared_ptr<ChecksumCalculator>&) const;
};

class Directory : public File {
    std::vector<std::shared_ptr<File>> files;
public:
    Directory(const std::string& path);

    void addFile(std::shared_ptr<File>& file);
    const std::vector<std::shared_ptr<File>>& getFiles() const;
};