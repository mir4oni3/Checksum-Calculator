#include "../inc/UserActions.hpp"
#include "../inc/CalculatorFactory.hpp"
#include "../inc/FileParserFactory.hpp"
#include "../inc/FileSystemBuilderFactory.hpp"

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
        viewChecksums(target, calc);
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