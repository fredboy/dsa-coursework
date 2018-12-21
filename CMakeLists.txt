cmake_minimum_required(VERSION 3.12)
project(dsa_coursework C)

set(CMAKE_C_STANDARD 11)

add_executable(dsa_coursework src/main.c src/record.c src/record.h src/quick_sort_records.c src/quick_sort_records.h src/record_utils.c src/record_utils.h src/getch.c src/getch.h src/avl_tree.c src/avl_tree.h src/shannon.c src/shannon.h)
target_link_libraries(dsa_coursework m)