add_library(animations SHARED
  animation.cpp
)
target_link_libraries(animations PUBLIC
  pigeon_war_lib
)

add_library(common_animations SHARED
  common/common.cpp
)
target_link_libraries(common_animations PRIVATE
  animations
)

add_library(shooter_animations SHARED
  shooter/shooter.cpp
)
target_link_libraries(shooter_animations PRIVATE
  animations
)

add_library(destroyer_animations SHARED
  destroyer/destroyer.cpp
)
target_link_libraries(destroyer_animations PRIVATE
  animations
)

add_library(native_animations SHARED
  native/native.cpp
)
target_link_libraries(native_animations PRIVATE
  animations
)

add_library(sniper_animations SHARED
  sniper/sniper.cpp
)
target_link_libraries(sniper_animations PRIVATE
  animations
)

add_library(poisoner_animations SHARED
  poisoner/poisoner.cpp
)
target_link_libraries(poisoner_animations PRIVATE
  animations
)
