# Voxel_Engine/app/CMakeLists.txt

project(zeroengine)

# Create the executable
add_executable(${PROJECT_NAME} "zero_engine.cpp" "App.cpp")

# Link the static library from subproject1 using its alias sub::lib1
# Link the header only library from subproject2 using its alias sub::lib2
# This will cause the include directories for that target to be added to this project

target_include_directories( ${PROJECT_NAME}
    PUBLIC 
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    core::core
    glad
    glm
    glfw
    stb
)

add_custom_target(copy_assets ALL)
  file(COPY "Assets" DESTINATION "${CMAKE_BINARY_DIR}/App")
add_dependencies(${PROJECT_NAME} copy_assets)
