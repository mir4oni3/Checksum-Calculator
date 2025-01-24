#pragma once

#include "File.hpp"
#include "InputFacade.hpp"

class FileSystemBuilder {
protected:
    std::string algorithm;
public:
    virtual ~FileSystemBuilder() = default;

    virtual std::shared_ptr<File> build(const std::string&) const = 0;
    
    void setAlgorithm(const std::string& algorithm);
};

class IgnoreSymlinkFileSystemBuilder : public FileSystemBuilder {
public:
    std::shared_ptr<File> build(const std::string&) const override;
};

class TraverseSymlinkFileSystemBuilder : public FileSystemBuilder {
public:
    std::shared_ptr<File> build(const std::string&) const override;
};