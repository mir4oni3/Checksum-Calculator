file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../imp/lib/*.*)
add_library(ChecksumsLibrary STATIC ${SOURCES})
target_include_directories(ChecksumsLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/)
target_include_directories(ChecksumsLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../resources)

file(GLOB RHASH_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../resources/librhash/*.c)
add_library(rhash STATIC ${RHASH_SOURCES})
target_include_directories(rhash PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../resources/librhash)

target_link_libraries(ChecksumsLibrary PUBLIC rhash)

add_executable(ChecksumsApp ${CMAKE_CURRENT_SOURCE_DIR}/../main.cpp)

target_link_libraries(ChecksumsApp PUBLIC ChecksumsLibrary)
