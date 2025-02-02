#include <catch2/catch_test_macros.hpp>

#include "Factories/HashStreamWriterFactory.hpp"
#include "Visitors/NormalTextStreamWriter.hpp"
#include "Visitors/XMLStreamWriter.hpp"
#include "Calculators/MD5Calculator.hpp"

TEST_CASE("TestHashStreamWriterFactoryGetWriter", "[HashStreamWriterFactory]") {
    std::shared_ptr<HashStreamWriter> writer;

    std::ostringstream os;
    std::shared_ptr<ChecksumCalculator> calc = std::make_shared<MD5Calculator>();

    REQUIRE_NOTHROW(writer = HashStreamWriterFactory::getWriter("text", os, calc));
    REQUIRE(std::dynamic_pointer_cast<NormalTextStreamWriter>(writer));

    REQUIRE_NOTHROW(writer = HashStreamWriterFactory::getWriter("xml", os, calc));
    REQUIRE(std::dynamic_pointer_cast<XMLStreamWriter>(writer));

    REQUIRE_THROWS_AS(HashStreamWriterFactory::getWriter("unsupported", os, calc), std::invalid_argument);
}