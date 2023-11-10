add_library(animations SHARED
  animation.cpp
)
target_link_libraries(animations PUBLIC
  pigeon_war_lib
)

include(${CMAKE_SOURCE_DIR}/cmake/animations.cmake)

add_animations(common SHARED)
add_animations(shooter SHARED)
add_animations(destroyer SHARED)
add_animations(native SHARED)
add_animations(sniper SHARED)
add_animations(poisoner SHARED)
add_animations(samurai SHARED)
add_animations(saberhand SHARED)
add_animations(monk SHARED)
