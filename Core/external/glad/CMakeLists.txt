# Voxel_Engine/core/external/glad/CMakeLists.txt

add_library(glad STATIC src/glad.c)

# Asegúrate de incluir los directorios de encabezados de glad
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)