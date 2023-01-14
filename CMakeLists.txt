cmake_minimum_required(VERSION 3.21)
project(pigeon_war_client)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(pigeon_war_client src/main.cpp src/window.cpp src/texture.cpp src/game.cpp src/texture_loader.cpp
  src/entity.cpp src/board.cpp src/animation.cpp)
if (NOT EMSCRIPTEN)
  target_include_directories(pigeon_war_client PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
  target_link_libraries(pigeon_war_client ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
endif ()

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE src)

if (EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

  target_compile_options(pigeon_war_client PUBLIC -sUSE_SDL=2)
  target_compile_options(pigeon_war_client PUBLIC -sUSE_SDL_IMAGE=2)
  target_compile_options(pigeon_war_client PUBLIC -sUSE_ZLIB=1)
  target_compile_options(pigeon_war_client PUBLIC -sUSE_LIBPNG=1)
  target_compile_options(pigeon_war_client PUBLIC -sUSE_LIBJPEG=1)
  target_compile_options(pigeon_war_client PUBLIC -sSDL2_IMAGE_FORMATS=["bmp","png"])
  target_compile_options(pigeon_war_client PUBLIC -sNO_DISABLE_EXCEPTION_CATCHING)

  target_link_options(pigeon_war_client PUBLIC -sUSE_SDL=2)
  target_link_options(pigeon_war_client PUBLIC -sUSE_SDL_IMAGE=2)
  target_link_options(pigeon_war_client PUBLIC -sUSE_ZLIB=1)
  target_link_options(pigeon_war_client PUBLIC -sUSE_LIBPNG=1)
  target_link_options(pigeon_war_client PUBLIC -sUSE_LIBJPEG=1)
  target_link_options(pigeon_war_client PUBLIC -sSDL2_IMAGE_FORMATS=["bmp","png"])
  target_link_options(pigeon_war_client PUBLIC --emrun)
  target_link_options(pigeon_war_client PUBLIC --preload-file ../res)
endif ()
