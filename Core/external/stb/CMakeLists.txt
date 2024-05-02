add_library(stb STATIC src/stb_image.cpp)

# Aseg�rate de incluir los directorios de encabezados de stb_image
target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)