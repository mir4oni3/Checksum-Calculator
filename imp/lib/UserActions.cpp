#include "../inc/UserActions.hpp"
#include "../inc/CalculatorFactory.hpp"
#include "../inc/FileParserFactory.hpp"
#include "../inc/FileSystemBuilderFactory.hpp"
#include "../inc/FileIterator.hpp"

#include <memory>

void UserActions::start(const InputFacade& input) {
    std::shared_ptr<FileSystemBuilder> builder = FileSystemBuilderFactory::getBuilder(input.getTraverse());
    if (input.getBuildChecksums()) {
        builder->setAlgorithm(input.getAlgorithm());
    }

    std::shared_ptr<File> target = builder->build(input.getPath());
    std::shared_ptr<ChecksumCalculator> calc = ChecksumCalculatorFactory::getCalculator(input.getAlgorithm());
    std::shared_ptr<FileParser> parser = FileParserFactory::getParser(input.getFormat());
    

    if (input.getChecksums() == "") {
        viewChecksums(target, calc, parser);
        return;
    }

    //compare target with checksum file
    std::ifstream checksumFile(input.getChecksums());
    if (!checksumFile) {
        throw std::runtime_error("FileActions::viewChecksums - Error opening file");
    }
    std::unordered_map<std::string, std::string> checksums = parser->parseFiles(checksumFile);
    compareChecksums(target, calc, checksums);
}

void UserActions::viewChecksums(const std::shared_ptr<File>& target, const std::shared_ptr<ChecksumCalculator>& calc, const std::shared_ptr<FileParser>& parser) {
    if (!parser) {	
        throw std::invalid_argument("UserActions::viewChecksums - nullptr parser passed");
    }
    parser->exportFile(target, std::cout, calc);
}

void UserActions::compareChecksums(const std::shared_ptr<File>& target, const std::shared_ptr<ChecksumCalculator>& calc, std::unordered_map<std::string, std::string>& checksums) {
    if (!target || !calc) {
        throw std::invalid_argument("UserActions::compareChecksums - nullptr arg(s) passed");
    }
    
    FileIterator it(target);
    while (it.hasMore()) {
        std::shared_ptr<RegularFile> regFile = std::dynamic_pointer_cast<RegularFile>(it.next());
        if (!regFile) {
            continue;
        }

        std::string currentPath = regFile->getPath();
        std::string checksum = regFile->getChecksum(calc);

        if (checksums.find(currentPath) == checksums.end()) {
            std::cout << currentPath << ": NEW\n";
            continue;
        }
        if (checksum != checksums.at(currentPath)) {
            std::cout << currentPath << ": MODIFIED\n";
            checksums.erase(currentPath);
            continue;
        }
        if (checksum == checksums.at(currentPath)) {
            std::cout << currentPath << ": OK\n";
            checksums.erase(currentPath);
            continue;
        }
    }

    for (const auto& [path, checksum] : checksums) {
        std::cout << path << ": REMOVED\n";
    }
}