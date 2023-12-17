##########################################################################

cmake_minimum_required(VERSION 3.10)

##########################################################################

project(game_console)

##########################################################################

# Source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")


# Include directories
include_directories("${LIB_DIR}")
include_directories("${LIB_DIR}/ArduinoCore-API/api")
include_directories("${LIB_DIR}/ArduinoCore-API/test/include")
include_directories("${LIB_DIR}/adafruit_driver")
include_directories("${LIB_DIR}/adafruit_driver/Fonts")
include_directories("${SRC_DIR}")
include_directories("${SRC_DIR}/games")
include_directories("${SRC_DIR}/menu")
include_directories("${SRC_DIR}/x86")
include_directories("${SRC_DIR}/wifi")

##########################################################################

set(CMAKE_BUILD_TYPE mingw32-make)
set(CMAKE_MAKE_PROGRAM mingw32-make)
set(CMAKE_C_COMPILER g++)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DARDUINO=101 -D__cplusplus1 -DBUILD_FOR_X86")


set(CPP_SRCS
    "${SRC_DIR}/Buttons.cpp"
    "${SRC_DIR}/game_execution.cpp"
    "${SRC_DIR}/main.cpp"
    "${SRC_DIR}/main_menu.cpp"
    "${SRC_DIR}/menu/analog_voltage_demo.cpp"
    "${SRC_DIR}/menu/keyboard.cpp"
    "${SRC_DIR}/menu/menu_item.cpp"
    "${SRC_DIR}/wifi/remote_sw_update.cpp"
    "${SRC_DIR}/wifi/wifi_features.cpp"
    # Libraries and mocks
    "${SRC_DIR}/x86/ArduinoCore-x86.cpp"
    "${LIB_DIR}/ArduinoCore-API/test/src/dtostrf.cpp"
    "${LIB_DIR}/ArduinoCore-API/test/src/itoa.cpp"
    "${LIB_DIR}/ArduinoCore-API/test/src/MillisFake.cpp"
    "${LIB_DIR}/adafruit_driver/Adafruit_GFX.cpp"
    "${LIB_DIR}/adafruit_driver/Adafruit_PCD8544.cpp"
    "${LIB_DIR}/ArduinoCore-API/api/Common.cpp"
    "${LIB_DIR}/ArduinoCore-API/api/IPAddress.cpp"
    "${LIB_DIR}/ArduinoCore-API/api/Print.cpp"
    "${LIB_DIR}/ArduinoCore-API/api/Stream.cpp"
    "${LIB_DIR}/ArduinoCore-API/api/String.cpp"
    # Add other source files as needed
    )

set(C_SRCS
    # Add other source files as needed
)

# Library directories
link_directories("${LIB_DIR}")

# Executable
add_executable(game_console ${CPP_SRCS} ${C_SRCS})

# Link libraries
target_link_libraries(game_console libzmq-v141-mt-4_3_4)

# Output directories
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")

# Additional settings (if needed)
# ...

# Print information (optional)
message(STATUS "C++ Source Files: ${CPP_SRCS}")
message(STATUS "C Source Files: ${C_SRCS}")
message(STATUS "Include Directories: ${LIB_DIR} ${SRC_DIR}")
message(STATUS "Library Directories: ${LIB_DIR}")