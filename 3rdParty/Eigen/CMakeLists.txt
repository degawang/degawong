
set(PROJECT_NAME Eigen)
add_library(${PROJECT_NAME} STATIC Eigen.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})