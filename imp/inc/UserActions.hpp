#pragma once

#include "../inc/InputFacade.hpp"
#include "../inc/ChecksumCalculator.hpp"
#include "../inc/File.hpp"

#include <memory>

class UserActions {
public:
    void start(const InputFacade& input);
    void viewChecksums(const std::shared_ptr<File>& target, const std::shared_ptr<ChecksumCalculator>& calc);
    void compareChecksums(const std::shared_ptr<File>& target, const std::shared_ptr<ChecksumCalculator>& calc, const std::unordered_map<std::string, std::string>& checksums);
};