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
    "${SOURCE_DIR}/Components/GameManagerComponent.cpp"     "${SOURCE_DIR}/Components/GameManagerComponent.h"
    "${SOURCE_DIR}/Components/HealthComponent.cpp"          "${SOURCE_DIR}/Components/HealthComponent.h"
    "${SOURCE_DIR}/Components/IngredientComponent.cpp"      "${SOURCE_DIR}/Components/IngredientComponent.h"
    "${SOURCE_DIR}/Components/IngredientTileComponent.cpp"  "${SOURCE_DIR}/Components/IngredientTileComponent.h"
    "${SOURCE_DIR}/Components/RotateComponent.cpp"          "${SOURCE_DIR}/Components/RotateComponent.h"
    "${SOURCE_DIR}/Components/ScoreComponent.cpp"           "${SOURCE_DIR}/Components/ScoreComponent.h"
    "${SOURCE_DIR}/Components/MovementComponent.cpp"        "${SOURCE_DIR}/Components/MovementComponent.h"
    "${SOURCE_DIR}/Components/LevelComponent.cpp"           "${SOURCE_DIR}/Components/LevelComponent.h"
    "${SOURCE_DIR}/Components/HealthUIManager.cpp"          "${SOURCE_DIR}/Components/HealthUIManager.h"
    "${SOURCE_DIR}/Components/EnemyAILogicComponent.cpp"    "${SOURCE_DIR}/Components/EnemyAILogicComponent.h"
    "${SOURCE_DIR}/Components/MenuComponent.cpp"            "${SOURCE_DIR}/Components/MenuComponent.h"
    "${SOURCE_DIR}/Components/SquashableComponent.cpp"      "${SOURCE_DIR}/Components/SquashableComponent.h"
    "${SOURCE_DIR}/Components/PepperComponent.cpp"          "${SOURCE_DIR}/Components/PepperComponent.h"
    "${SOURCE_DIR}/Components/StunnableComponent.cpp"       "${SOURCE_DIR}/Components/StunnableComponent.h"
    "${SOURCE_DIR}/Components/ThrowPepperComponent.cpp"     "${SOURCE_DIR}/Components/ThrowPepperComponent.h"
    "${SOURCE_DIR}/Components/RespawnComponent.cpp"         "${SOURCE_DIR}/Components/RespawnComponent.h"
    "${SOURCE_DIR}/Components/SelfDestroyComponent.cpp"     "${SOURCE_DIR}/Components/SelfDestroyComponent.h"

    # Commands
    "${SOURCE_DIR}/InputCommands/PlayingCommands.cpp"       "${SOURCE_DIR}/InputCommands/PlayingCommands.h"

    # Helpers
    "${SOURCE_DIR}/Helpers/LevelLoader.cpp"                 "${SOURCE_DIR}/Helpers/LevelLoader.h"

    # State
    "${SOURCE_DIR}/State/IGameState.h"
    "${SOURCE_DIR}/State/GamePlayingState.cpp"              "${SOURCE_DIR}/State/GamePlayingState.h"
    "${SOURCE_DIR}/State/GameMenuState.cpp"                 "${SOURCE_DIR}/State/GameMenuState.h"
    "${SOURCE_DIR}/State/GameLeaderboardState.cpp"          "${SOURCE_DIR}/State/GameLeaderboardState.h"
)

# Create the executable
add_executable(${TARGET_NAME} ${BURGER_TIME_SOURCES})
target_include_directories(${TARGET_NAME} PUBLIC
    "${SOURCE_DIR}/Components"
    "${SOURCE_DIR}/Helpers"
    "${SOURCE_DIR}/InputCommands"
    "${SOURCE_DIR}/State"
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
