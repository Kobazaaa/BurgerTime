#--------------------------------------------------
#    Burger Time CMake
#--------------------------------------------------

# Setup
cmake_minimum_required(VERSION 3.26)
project(BurgerTime)
add_subdirectory(Kobengine)

# Set variables
set(TARGET_NAME BurgerTime)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/game)

# Set source files for the game
list(APPEND BURGER_TIME_SOURCES
    # Main
	"${SOURCE_DIR}/BurgerTime.cpp"

    # Components
    "${SOURCE_DIR}/Components/HealthComponent.cpp"      "${SOURCE_DIR}/Components/HealthComponent.h"
    "${SOURCE_DIR}/Components/RotateComponent.cpp"      "${SOURCE_DIR}/Components/RotateComponent.h"
    "${SOURCE_DIR}/Components/ScoreComponent.cpp"       "${SOURCE_DIR}/Components/ScoreComponent.h"

    # Commands
    "${SOURCE_DIR}/InputCommands/DamageCommand.cpp"     "${SOURCE_DIR}/InputCommands/DamageCommand.h"
    "${SOURCE_DIR}/InputCommands/MoveCommands.cpp"      "${SOURCE_DIR}/InputCommands/MoveCommands.h"
    "${SOURCE_DIR}/InputCommands/ScoreCommand.cpp"      "${SOURCE_DIR}/InputCommands/ScoreCommand.h"
)

# Create the executable
add_executable(${TARGET_NAME} ${BURGER_TIME_SOURCES})
target_include_directories(${TARGET_NAME} PUBLIC
    "${SOURCE_DIR}/Components"
    "${SOURCE_DIR}/InputCommands"
)

# Enable c++20 features
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)
set_property(TARGET ${TARGET_NAME} PROPERTY
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
)
# Max warning level & treat warnings as errors
target_compile_options(${TARGET_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

if(WIN32)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
    set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")
endif()

# Link Kobengine
target_link_libraries(${TARGET_NAME} PRIVATE kobengine)
