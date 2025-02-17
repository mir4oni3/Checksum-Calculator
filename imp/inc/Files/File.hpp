#pragma once

#include "Calculators/ChecksumCalculator.hpp"

#include <vector>
#include <memory>
#include <string>

class VisitorWriter;

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

    virtual void accept(const VisitorWriter& writer) const = 0;
};

class RegularFile : public File {
protected:
    mutable std::string checksum;
public:
    RegularFile(const std::string& path);

    std::string getChecksum(const ChecksumCalculator&) const;
    void accept(const VisitorWriter& writer) const override;
};

class Directory : public File {
    std::vector<std::unique_ptr<File>> files;
public:
    Directory(const std::string& path);

    void addFile(std::unique_ptr<File>&& file);
    const std::vector<std::unique_ptr<File>>& getFiles() const;
    void sortFiles();
    
    void accept(const VisitorWriter& writer) const override;
};