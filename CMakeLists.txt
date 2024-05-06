cmake_minimum_required (VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
project (Zero_engine)
file(COPY "${PROJECT_SOURCE_DIR}/App/Assets" DESTINATION "${CMAKE_BINARY_DIR}/App")
# Add sub directories
add_subdirectory(Core)
add_subdirectory(App)