#pragma once

#include "File.hpp"
#include "IO/InputFacade.hpp"

class FileSystemBuilder {
protected:
    std::string algorithm;
    bool saveFullPath = false;
public:
    virtual ~FileSystemBuilder() = default;

    virtual std::unique_ptr<File> build(const std::string&) const = 0;
    
    void setAlgorithm(const std::string& algorithm);
    void saveFullPaths(bool save);
};

class IgnoreSymlinkFileSystemBuilder : public FileSystemBuilder {
public:
    std::unique_ptr<File> build(const std::string&) const override;
};

class TraverseSymlinkFileSystemBuilder : public FileSystemBuilder {
public:
    std::unique_ptr<File> build(const std::string&) const override;
};