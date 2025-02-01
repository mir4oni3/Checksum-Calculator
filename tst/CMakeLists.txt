include(FetchContent)
FetchContent_Declare(
   Catch2
   GIT_REPOSITORY https://github.com/catchorg/Catch2.git
   GIT_TAG v3.7.1
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../tst/Test*.cpp)

add_executable(runTests ${TEST_SOURCES})

target_link_libraries(runTests PRIVATE Catch2::Catch2WithMain ChecksumsLibrary)

include(CTest)
include(Catch)
catch_discover_tests(runTests)