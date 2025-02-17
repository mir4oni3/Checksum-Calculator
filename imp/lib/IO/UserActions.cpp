#include "IO/UserActions.hpp"
#include "Factories/CalculatorFactory.hpp"
#include "Factories/HashStreamWriterFactory.hpp"
#include "Factories/FileSystemBuilderFactory.hpp"
#include "Factories/ParserFactory.hpp"
#include "Files/FileIterator.hpp"
#include "Visitors/CompareWriter.hpp"

#include <memory>

void UserActions::start(const InputFacade& input) {
    //initialize builder
    std::unique_ptr<FileSystemBuilder> builder = FileSystemBuilderFactory::getBuilder(input.getTraverse());

    //set builder mode to calculate checksums when building filesystem if requested
    if (input.getBuildChecksums()) {
        builder->setAlgorithm(input.getAlgorithm());
    }

    //initialize other objects
    std::unique_ptr<File> target = builder->build(input.getPath());
    std::unique_ptr<ChecksumCalculator> calc = ChecksumCalculatorFactory::getCalculator(input.getAlgorithm());
    std::unique_ptr<ProgressReporter> reporter = std::make_unique<ProgressReporter>(std::cout);

    //perform actions
    if (input.getChecksums() == "") { 
        std::unique_ptr<HashStreamWriter> writer = HashStreamWriterFactory::getWriter(input.getFormat(), std::cout, *calc);
        if (!input.getBuildChecksums()) {
            //observe progress only in the no-build mode, since in build mode
            //the checksums are already calculated during the builder->build function.
            writer->addObserver(*reporter);
        }
        writer->exportFile(*target);
    }
    else {
        compareChecksums(*target, *calc, input.getChecksums(), input.getFormat(), *reporter);
    }

    //save file if user requested
    if (input.getSaveTo() != "") {
        std::ofstream ofs(input.getSaveTo());
        if (!ofs) {
            throw std::runtime_error("UserActions::start - Error opening file");
        }
        HashStreamWriterFactory::getWriter(input.getFormat(), ofs, *calc)->exportFile(*target);
    }
}

void UserActions::compareChecksums(File& target, ChecksumCalculator& calc, const std::string& checksumFile,
                                   const std::string& format, ProgressReporter& reporter) {
    //parse checksum file to hashmap
    std::ifstream checksumIfs(checksumFile);
    if (!checksumIfs) {
        throw std::runtime_error("FileActions::start - Error opening checksum file");
    }
    std::unique_ptr<FileHashParser> parser = ParserFactory::getParser(checksumIfs);
    std::unordered_map<std::string, std::string> checksums = parser->parseFiles(checksumIfs);

    std::unique_ptr<CompareWriter> compareWriter = std::make_unique<CompareWriter>(std::cout, calc, checksums);
    compareWriter->addObserver(reporter);
    compareWriter->exportFile(target);
}