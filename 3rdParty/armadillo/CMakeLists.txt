set(PROJECT_NAME armadillo)
add_library(${PROJECT_NAME} STATIC armadillo.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)