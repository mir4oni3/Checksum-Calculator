#pragma once

#include "InputFacade.hpp"
#include "ChecksumCalculator.hpp"
#include "File.hpp"
#include "HashStreamWriter.hpp"

#include <memory>

class UserActions {
public:
    static void start(const InputFacade& input);
    static void viewChecksums(const std::shared_ptr<File>& target, const std::shared_ptr<HashStreamWriter>& writer);
    static void compareChecksums(const std::shared_ptr<File>& target, const std::shared_ptr<ChecksumCalculator>& calc, std::unordered_map<std::string, std::string>& checksums);
};