project(ZeroCore)
include(FetchContent)

# Libraries
file(GLOB_RECURSE LIBRARIES_SOURCES 
    "libraries/*.cpp"
)

# Find external libraries
find_package(glfw3 CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED) 
add_subdirectory(external/nativefiledialog-extended)
add_subdirectory(external/glad)
add_subdirectory(external/stb)


add_library(${PROJECT_NAME} core.cpp ${LIBRARIES_SOURCES})
add_library(core ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/libraries
    ${PROJECT_SOURCE_DIR}/external/glad/include
    ${PROJECT_SOURCE_DIR}/external/stb/include
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC 
    glad
    stb_image
    glm::glm
    glfw
    spdlog::spdlog
    assimp::assimp
    EnTT::EnTT
    imgui::imgui
    nfd
    imguizmo::imguizmo
)   
target_compile_definitions(${PROJECT_NAME} PRIVATE ZERO_CORE_LOGGER_ACTIVATE)

