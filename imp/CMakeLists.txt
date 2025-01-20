file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../imp/lib/*.cpp)

add_library(ChecksumsLibrary STATIC ${SOURCES})
add_executable(ChecksumsApp ${CMAKE_CURRENT_SOURCE_DIR}/../main.cpp)

target_include_directories(ChecksumsLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_include_directories(ChecksumsLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../tclap)

target_link_libraries(ChecksumsApp ChecksumsLibrary)