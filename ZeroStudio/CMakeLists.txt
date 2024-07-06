# Voxel_Engine/app/CMakeLists.txt

project(ZeroStudio)

# Libraries
file(GLOB_RECURSE PANELS_SOURCES 
    "Panels/*.cpp"
)
# Create the executable
add_executable(${PROJECT_NAME} "src/ZeroStudio.cpp" "src/EditorLayer.h" "src/EditorLayer.cpp" ${PANELS_SOURCES})
target_include_directories( ${PROJECT_NAME}
    PUBLIC 
    ${PROJECT_SOURCE_DIR}
    "Components/*.h"
)

target_link_libraries(${PROJECT_NAME}

    core
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ZERO_CORE_LOGGER_ACTIVATE ZERO_ENABLE_ASSERTS)