# Voxel_Engine/core/external/CMakeLists.txt


project(core_external)

add_subdirectory(glfw-3.3.9)
# A�ade la carpeta "glad"
add_subdirectory(glad)
add_subdirectory(glm-master)
add_subdirectory(stb)
add_subdirectory(entt)
# Aseg�rate de incluir los directorios de los encabezados de GLFW
add_library(${PROJECT_NAME} external.cpp)

target_include_directories( ${PROJECT_NAME}
    PUBLIC 
    ${PROJECT_SOURCE_DIR}glad/include
    ${PROJECT_SOURCE_DIR}glm-master/glm
    ${PROJECT_SOURCE_DIR}glfw-3.3.9/include
    ${PROJECT_SOURCE_DIR}stb/include
    ${PROJECT_SOURCE_DIR}entt/include
)


