project (core_folder)

file(GLOB SOURCES 
    "libraries/*.cpp" 
    "libraries/Debug/*.cpp"
    "libraries/Buffers/*.cpp"
    "libraries/GL_graphics/*.cpp"
    "libraries/ECS/*.cpp"
    "libraries/core/*.cpp"
    "libraries/ResourceManagement/*.cpp"
    "libraries/Modules/*.cpp"
)
add_subdirectory(external)

add_library(${PROJECT_NAME} core.cpp ${SOURCES})
add_library(core::core ALIAS ${PROJECT_NAME})


target_include_directories( ${PROJECT_NAME}
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/libraries
    ${PROJECT_SOURCE_DIR}/external/glad/include
    ${PROJECT_SOURCE_DIR}/external/glm-master/glm
    ${PROJECT_SOURCE_DIR}/external/glfw-3.3.9/include
    ${PROJECT_SOURCE_DIR}/external/stb/include
)

add_custom_target(copy_core_assets ALL)
  file(COPY "Assets" DESTINATION "${CMAKE_BINARY_DIR}/App/Core")
add_dependencies(${PROJECT_NAME} copy_core_assets)