add_library(stb_image STATIC src/stb_image.cpp)

# Aseg�rate de incluir los directorios de encabezados de stb_image
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/stb)