#pragma once

#include "File.hpp"
#include "IO/InputFacade.hpp"

#include <unordered_set>

class FileSystemBuilder {
protected:
    std::string algorithm;
    bool saveFullPath = false;
    bool sortFiles = false;
public:
    virtual ~FileSystemBuilder() = default;

    std::unique_ptr<File> build(const std::string&) const;
    virtual std::unique_ptr<File> buildCommon(const std::string& path, const std::unique_ptr<ChecksumCalculator>& calc) const;
    std::unique_ptr<File> handleDirectory(const std::string& path, const std::unique_ptr<ChecksumCalculator>& calc) const;

    void setAlgorithm(const std::string& algorithm);
    void setSortFiles(bool sort);
    void saveFullPaths(bool save);
};

//NOTE: this implementation will traverse UNIX symlinks only
//Windows .lnk files(or any other files) won't be treated as symlinks
class TraverseSymlinkFileSystemBuilder : public FileSystemBuilder {
    mutable std::unordered_set<std::string> visited;
public:
    std::unique_ptr<File> buildCommon(const std::string&path, const std::unique_ptr<ChecksumCalculator>& calc) const override;
};