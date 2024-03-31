cmake_minimum_required(VERSION 3.15)  # CMake version check
project(caeos_project)               # Create project "simple_example"

set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard

# Set linker script for machine target
set(LINKER_SCRIPT "${PROJECT_SOURCE_DIR}/linker.ld")
include(lm3s6965evb.cmake)

set(RTOS_LIBS
        RTOSLibs/AsyncIO/async_io.c
)
set(RTOS_LIBS_INCLUDE
        RTOSLibs/AsyncIO
)

set(GAMES
        games/pong/pong.c
)
set(GAMES_INCLUDE
        games/pong
)

set(SOURCE_FILES
        main.c
        ${RTOS_LIBS}
        ${GAMES}
)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(firmware ${SOURCE_FILES})
target_include_directories(
        firmware
        PUBLIC
        include
        LuminaryMicro
        ${RTOS_LIBS_INCLUDE}
        ${GAMES_INCLUDE}
)

# ARM CMSIS
add_subdirectory(CMSIS)
target_link_libraries(firmware cmsis)

# LuminaryMicro Board Support Package
add_subdirectory(LuminaryMicro)
target_link_libraries(firmware drivers)

# FreeRTOS
set(FREERTOS_CONFIG_FILE_DIRECTORY ${PROJECT_SOURCE_DIR})
set(FREERTOS_PORT GCC_ARM_CM3)
add_subdirectory(FreeRTOS/Source)
target_link_libraries(firmware freertos_kernel)
