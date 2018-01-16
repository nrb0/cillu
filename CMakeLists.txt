project(cillu)

set(MY_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(modules/cabl)
add_subdirectory(modules/oscpack)

add_subdirectory(src)
