﻿# Voxel_Engine/app/CMakeLists.txt

project(zeroengine)

# Create the executable
add_executable(${PROJECT_NAME} "zero_engine.cpp" "App.cpp")
target_include_directories( ${PROJECT_NAME}
    PUBLIC 
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    core
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ZERO_CORE_LOGGER_ACTIVATE ZERO_ENABLE_ASSERTS)