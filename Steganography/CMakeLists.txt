cmake_minimum_required(VERSION 3.28)
project(Steganography C)

set(CMAKE_C_STANDARD 11)

find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)

add_executable(Steganography main.c
        utils.h
        steganography.h
        steganography.c)

target_link_libraries(Steganography PNG::PNG ZLIB::ZLIB)