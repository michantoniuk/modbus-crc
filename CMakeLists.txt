cmake_minimum_required(VERSION 3.16)
project(ModbusCrc VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Enable release optimizations for non-MSVC compilers
if(NOT MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()

# Find Qt packages
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

# Windows-specific settings
if(WIN32)
    # Enable optimizations for MSVC
    add_compile_options(/O2 /GL /Gy /Oi)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    # Always create Windows executable (not console app) in all configurations
    set(CMAKE_WIN32_EXECUTABLE TRUE)
endif()

# Find threading library
find_package(Threads REQUIRED)

# Add resources file
set(RESOURCES
        resources/resources.qrc
)

# Set source files
set(SOURCES
        src/main.cpp
        src/main_window.cpp
        src/crc_calculator.cpp
        src/hex_parser.cpp
)

set(HEADERS
        src/main_window.h
        src/crc_calculator.h
        src/hex_parser.h
)

# Create executable
add_executable(ModbusCrc
        ${SOURCES}
        ${HEADERS}
        ${RESOURCES}
)

# Add version info for Windows only if app_icon.ico exists
if(WIN32 AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/app_icon.ico" AND
        EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/win32/app.rc.in")
    configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/win32/app.rc.in
            ${CMAKE_CURRENT_BINARY_DIR}/app.rc
            @ONLY
    )
    target_sources(ModbusCrc PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/app.rc)
endif()

# Link libraries
target_link_libraries(ModbusCrc PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Threads::Threads
)

# Create a standalone executable package
if(WIN32)
    # Get Qt deployment tool
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    # Create a custom target for packaging
    add_custom_target(standalone_package
            # First, make a clean deployment directory
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/standalone"
            COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/standalone"

            # Copy our executable
            COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE:ModbusCrc>"
            "${CMAKE_BINARY_DIR}/standalone/"

            # Run windeployqt to copy all dependencies
            COMMAND ${WINDEPLOYQT_EXECUTABLE}
            --release
            --no-translations
            --no-system-d3d-compiler
            --no-opengl-sw
            --no-virtualkeyboard
            "${CMAKE_BINARY_DIR}/standalone/ModbusCrc.exe"

            # Create a simple README file
            COMMAND ${CMAKE_COMMAND} -E echo "ModbusCrc - Samodzielna aplikacja do obliczania CRC-16 Modbus" > "${CMAKE_BINARY_DIR}/standalone/README.txt"

            DEPENDS ModbusCrc
            COMMENT "Creating standalone package"
    )

    # Install standalone package
    install(DIRECTORY "${CMAKE_BINARY_DIR}/standalone/"
            DESTINATION .
            COMPONENT standalone)
else()
    # On Linux, just install the executable
    install(TARGETS ModbusCrc
            RUNTIME DESTINATION bin
            COMPONENT application)
endif()