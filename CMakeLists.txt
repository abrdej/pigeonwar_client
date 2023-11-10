cmake_minimum_required(VERSION 3.21)
project(pigeon_war_client)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-fPIC)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Boost COMPONENTS log log_setup system REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
if (NOT EMSCRIPTEN)
  find_package(SDL2TTF REQUIRED)
endif ()

configure_file(src/config.h.in ${CMAKE_SOURCE_DIR}/src/config.h @ONLY)

set(LIB_SOURCES
  src/board.cpp
  src/button.cpp
  src/entities_collection.cpp
  src/entity.cpp
  src/game.cpp
  src/game_state.cpp
  src/load_resources.cpp
  src/message_processor.cpp
  src/panel.cpp
  src/text.cpp
  src/texture.cpp
  src/texture_loader.cpp
  src/timer.cpp
  src/window.cpp
  src/animation_factory.cpp
)

if (EMSCRIPTEN)
  list(APPEND LIB_SOURCES src/websocket_client_em.cpp)
else ()
  list(APPEND LIB_SOURCES src/websocket_client_beast.cpp)
endif ()

add_library(pigeon_war_lib
  ${LIB_SOURCES}
)

target_include_directories(pigeon_war_lib PUBLIC src)
target_include_directories(pigeon_war_lib PUBLIC external/json/include)
target_include_directories(pigeon_war_lib PUBLIC external/lynx/include)

if (NOT EMSCRIPTEN)
  target_include_directories(
    pigeon_war_lib PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR})
  target_link_libraries(
    pigeon_war_lib ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY} Boost::log Boost::log_setup dl)
endif ()

# TODO: add emscripten version of DL
if (EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

  target_compile_options(pigeon_war_lib PUBLIC -sUSE_SDL=2)
  target_compile_options(pigeon_war_lib PUBLIC -sUSE_SDL_IMAGE=2)
  target_compile_options(pigeon_war_lib PUBLIC -sUSE_SDL_TTF=2)
  target_compile_options(pigeon_war_lib PUBLIC -sUSE_ZLIB=1)
  target_compile_options(pigeon_war_lib PUBLIC -sUSE_LIBPNG=1)
  target_compile_options(pigeon_war_lib PUBLIC -sUSE_LIBJPEG=1)
  target_compile_options(pigeon_war_lib PUBLIC -sSDL2_IMAGE_FORMATS=["bmp","png"])
  target_compile_options(pigeon_war_lib PUBLIC -O2)
#  target_compile_options(pigeon_war_lib PUBLIC -sNO_DISABLE_EXCEPTION_CATCHING)

  target_link_options(pigeon_war_lib PUBLIC -sUSE_SDL=2)
  target_link_options(pigeon_war_lib PUBLIC -sUSE_SDL_IMAGE=2)
  target_link_options(pigeon_war_lib PUBLIC -sUSE_SDL_TTF=2)
  target_link_options(pigeon_war_lib PUBLIC -sUSE_ZLIB=1)
  target_link_options(pigeon_war_lib PUBLIC -sUSE_LIBPNG=1)
  target_link_options(pigeon_war_lib PUBLIC -sUSE_LIBJPEG=1)
  target_link_options(pigeon_war_lib PUBLIC -sSDL2_IMAGE_FORMATS=["bmp","png"])
  target_link_options(pigeon_war_lib PUBLIC --emrun)
  target_link_options(pigeon_war_lib PUBLIC --preload-file ../res/)
  target_link_options(pigeon_war_lib PUBLIC --use-preload-plugins)
  target_link_options(pigeon_war_lib PUBLIC -lwebsocket.js)
#  target_link_options(pigeon_war_lib PUBLIC -sNO_DISABLE_EXCEPTION_CATCHING)
endif ()

add_subdirectory(src/animations)

add_executable(pigeon_war_client
  src/main.cpp
)
target_link_libraries(pigeon_war_client PRIVATE
  pigeon_war_lib
)
