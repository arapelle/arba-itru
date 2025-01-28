cmake_minimum_required(VERSION 3.26)

project(basic_cmake_project)

add_executable(${PROJECT_NAME} main.cpp)
# Find package arba-itru:
find_package(arba-itru 0.3.0 CONFIG REQUIRED)
# Use arba-itru release shared target:
target_link_libraries(${PROJECT_NAME} PRIVATE arba-itru)
