# Voxel_Engine/core/external/entt/CMakeLists.txt

add_library(entt STATIC src/entt.cpp)

# Asegúrate de incluir los directorios de encabezados de entt
target_include_directories(entt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)