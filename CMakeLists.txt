cmake_minimum_required(VERSION 3.24)
project(pigeon_war_client)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(pigeon_war_client src/main.cpp src/window.cpp src/texture.cpp src/game.cpp src/texture_loader.cpp
  src/entity.cpp src/board.cpp src/animation.cpp)
target_include_directories(pigeon_war_client PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(pigeon_war_client ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE src)
