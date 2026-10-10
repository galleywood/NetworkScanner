cmake_minimum_required(VERSION 3.15)
project(NetworkScanner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add new .cpp files here as the project grows
add_executable(NetworkScanner
        src/main.cpp
        src/network_utils.cpp
        src/arp.cpp
        src/hostname.cpp
        src/ping.cpp
        src/scanner.cpp
        src/printer.cpp
        src/vendor.cpp
)

target_include_directories(NetworkScanner PRIVATE src)

if(WIN32)
    target_compile_definitions(NetworkScanner PRIVATE
            _WINSOCK_DEPRECATED_NO_WARNINGS
            WIN32_LEAN_AND_MEAN
    )
    target_link_libraries(NetworkScanner PRIVATE ws2_32 iphlpapi)
else()
    message(FATAL_ERROR "This scanner uses the Windows API (SendARP, IcmpSendEcho) and only builds on Windows.")
endif()

# Copy the IEEE vendor list next to the .exe (put it in data/oui.csv, then reload CMake)
if(EXISTS ${CMAKE_SOURCE_DIR}/data/oui.csv)
    add_custom_command(TARGET NetworkScanner POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/data/oui.csv
            $<TARGET_FILE_DIR:NetworkScanner>)
endif()

if(MSVC)
    target_compile_options(NetworkScanner PRIVATE /W4)
else()
    target_compile_options(NetworkScanner PRIVATE -Wall -Wextra)
endif()