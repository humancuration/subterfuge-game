cmake_minimum_required(VERSION 3.12)
project(GameAI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GDExtension setup
find_package(godot-cpp CONFIG REQUIRED)

# Our source files
set(SOURCES
    src/AI/register_types.cpp
    src/AI/AIController.cpp
    src/AI/NPCController.cpp
    src/AI/EmergentBehavior/EmergentBehaviorManager.cpp
    # ... other source files
)

# Create the GDExtension library
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE godot::cpp) 