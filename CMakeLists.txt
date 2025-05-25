cmake_minimum_required(VERSION 3.30)
project(raylibTest C)

set(CMAKE_C_STANDARD 11)

set(INC_DIR ./include)
set(LINK_DIR ./lib)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})


add_executable(raylibTest main.c
        include/enemy.h
        include/bullet.h
        include/movement.h
        movement.c
        bullet.c
        enemy.c)
target_link_libraries(raylibTest libraylib.a)
target_link_libraries(raylibTest winmm)