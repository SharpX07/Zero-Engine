# Voxel_Engine/core/external/CMakeLists.txt


project(core_external)

add_subdirectory(glfw-3.3.9)
# A�ade la carpeta "glad"
add_subdirectory(glad)
add_subdirectory(glm-master)
add_subdirectory(stb)
add_subdirectory(entt)
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "" FORCE)
configure_file("${PROJECT_SOURCE_DIR}/assimp/include/assimp/config.h.in" "${PROJECT_SOURCE_DIR}/assimp/include/assimp/config.h")
configure_file("${PROJECT_SOURCE_DIR}/assimp/contrib/zlib/zconf.h.cmakein" "vendor/assimp/contrib/zlib/zconf.h")
add_subdirectory(assimp)

# Aseg�rate de incluir los directorios de los encabezados de GLFW
add_library(${PROJECT_NAME} external.cpp)

target_include_directories( ${PROJECT_NAME}
    PUBLIC 
    ${PROJECT_SOURCE_DIR}glad/include
    ${PROJECT_SOURCE_DIR}glm-master/glm
    ${PROJECT_SOURCE_DIR}glfw-3.3.9/include
    ${PROJECT_SOURCE_DIR}stb/include
    ${PROJECT_SOURCE_DIR}entt/include
    ${PROJECT_SOURCE_DIR}assimp/include
)


