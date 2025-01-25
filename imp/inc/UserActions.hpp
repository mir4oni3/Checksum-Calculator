#pragma once

#include "../inc/InputFacade.hpp"
#include "../inc/ChecksumCalculator.hpp"
#include "../inc/File.hpp"
#include "../inc/FileParser.hpp"

#include <memory>

class UserActions {
public:
    void start(const InputFacade& input) const;
    void viewChecksums(const std::shared_ptr<File>& target, const std::shared_ptr<ChecksumCalculator>& calc, const std::shared_ptr<FileParser>& parser) const;
    void compareChecksums(const std::shared_ptr<File>& target, const std::shared_ptr<ChecksumCalculator>& calc, std::unordered_map<std::string, std::string>& checksums) const;
};