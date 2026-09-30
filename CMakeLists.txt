cmake_minimum_required(VERSION 3.20)

project(bxf
    VERSION 0.2.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Library

add_library(bxf
    src/writer.c
    src/reader.c
)

target_include_directories(bxf
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(bxf
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

# Examples

add_executable(bxf_writer_example
    examples/writer_example.c
)

target_link_libraries(bxf_writer_example
    PRIVATE
        bxf
)

add_executable(bxf_reader_example
    examples/reader_example.c
)

target_link_libraries(bxf_reader_example
    PRIVATE
        bxf
)

add_executable(bxf_values_example
    examples/values_example.c
)

target_link_libraries(bxf_values_example
    PRIVATE
        bxf
)

add_executable(bxf_error_example
    examples/error_example.c
)

target_link_libraries(bxf_error_example
    PRIVATE
        bxf
)

add_executable(bxf_roundtrip_example
    examples/roundtrip_example.c
)

target_link_libraries(bxf_roundtrip_example
    PRIVATE
        bxf
)
