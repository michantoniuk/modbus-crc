cmake_minimum_required(VERSION 3.16)
project(ModbusCrc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Enable optimizations for Release builds
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

if(WIN32)
    # Windows specific flags
    add_compile_options(/O2 /GL /Gy /Oi)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Add threading support
find_package(Threads REQUIRED)

# Define source files
set(SOURCES
        src/main.cpp
        src/main_window.cpp
        src/crc_calculator.cpp
        src/hex_parser.cpp
)

# Define header files
set(HEADERS
        src/main_window.h
        src/crc_calculator.h
        src/hex_parser.h
)

add_executable(ModbusCrcCalculator
        ${SOURCES}
        ${HEADERS}
)

target_link_libraries(ModbusCrcCalculator PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Threads::Threads
)

if(WIN32)
    # On Windows, set to use console for debug, GUI for release
    set_target_properties(ModbusCrcCalculator PROPERTIES
            WIN32_EXECUTABLE $<CONFIG:Release>
    )
endif()