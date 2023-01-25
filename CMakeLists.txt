cmake_minimum_required(VERSION 3.24)
project(i_wanna_be_the_dino C)

set(CMAKE_C_STANDARD 11)

add_executable(iwbtd "iwbtd.c" "iwbtd.h" "iwbtd.rc" "common.h" "defs.h" "structs.h" "init.c" "init.h"
        "game.c" "game.h" "dino.c" "dino.h" "sprites.c" "sprites.h")


target_link_libraries(iwbtd SDL2main)
target_link_libraries(iwbtd SDL2)
target_link_libraries(iwbtd SDL2_image)
target_link_libraries(iwbtd SDL2_ttf)
target_link_libraries(iwbtd SDL2_mixer)