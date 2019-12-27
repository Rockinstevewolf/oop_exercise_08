cmake_minimum_required(VERSION 2.9)

project(oop_exercise_8)

find_package(Threads)

include_directories(BEFORE
         ${PROJECT_SOURCE_DIR}/include  
)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})