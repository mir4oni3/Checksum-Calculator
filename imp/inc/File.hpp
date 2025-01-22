#pragma once

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
    virtual size_t getSize() const;
    std::string getPath() const;
};

class RegularFile : public File {
public:
    RegularFile(const std::string& path);
};

class Directory : public File {
    std::vector<std::shared_ptr<File>> files;
public:
    Directory(const std::string& path);
    void addFile(std::shared_ptr<File>& file);
};