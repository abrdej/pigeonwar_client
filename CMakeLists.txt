cmake_minimum_required(VERSION 3.21)
project(pigeon_war_client)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Boost COMPONENTS log log_setup system REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
if (NOT EMSCRIPTEN)
  find_package(SDL2TTF REQUIRED)
endif ()

configure_file(src/config.h.in ${CMAKE_SOURCE_DIR}/src/config.h @ONLY)

set(SOURCES
  src/animation.cpp
  src/board.cpp
  src/button.cpp
  src/entities_collection.cpp
  src/entity.cpp
  src/game.cpp
  src/game_state.cpp
  src/main.cpp
  src/message_processor.cpp
  src/panel.cpp
  src/text.cpp
  src/texture.cpp
  src/texture_loader.cpp
  src/timer.cpp
  src/window.cpp
)

if (EMSCRIPTEN)
  list(APPEND SOURCES src/websocket_client_em.cpp)
else ()
  list(APPEND SOURCES src/websocket_client_beast.cpp)
endif ()

add_executable(pigeon_war_client ${SOURCES})
if (NOT EMSCRIPTEN)
  target_include_directories(
    pigeon_war_client PUBLIC ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIR})
  target_link_libraries(
    pigeon_war_client ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY} Boost::log Boost::log_setup)
endif ()

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE src)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE external/json/include)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE external/lynx/include)

if (EMSCRIPTEN)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

  target_compile_options(pigeon_war_client PUBLIC -sUSE_SDL=2)
  target_compile_options(pigeon_war_client PUBLIC -sUSE_SDL_IMAGE=2)
  target_compile_options(pigeon_war_client PUBLIC -sUSE_SDL_TTF=2)
  target_compile_options(pigeon_war_client PUBLIC -sUSE_ZLIB=1)
  target_compile_options(pigeon_war_client PUBLIC -sUSE_LIBPNG=1)
  target_compile_options(pigeon_war_client PUBLIC -sUSE_LIBJPEG=1)
  target_compile_options(pigeon_war_client PUBLIC -sSDL2_IMAGE_FORMATS=["bmp","png"])
  target_compile_options(pigeon_war_client PUBLIC -O2)
#  target_compile_options(pigeon_war_client PUBLIC -sNO_DISABLE_EXCEPTION_CATCHING)

  target_link_options(pigeon_war_client PUBLIC -sUSE_SDL=2)
  target_link_options(pigeon_war_client PUBLIC -sUSE_SDL_IMAGE=2)
  target_link_options(pigeon_war_client PUBLIC -sUSE_SDL_TTF=2)
  target_link_options(pigeon_war_client PUBLIC -sUSE_ZLIB=1)
  target_link_options(pigeon_war_client PUBLIC -sUSE_LIBPNG=1)
  target_link_options(pigeon_war_client PUBLIC -sUSE_LIBJPEG=1)
  target_link_options(pigeon_war_client PUBLIC -sSDL2_IMAGE_FORMATS=["bmp","png"])
  target_link_options(pigeon_war_client PUBLIC --emrun)
  target_link_options(pigeon_war_client PUBLIC --preload-file ../res/)
  target_link_options(pigeon_war_client PUBLIC --use-preload-plugins)
  target_link_options(pigeon_war_client PUBLIC -lwebsocket.js)
#  target_link_options(pigeon_war_client PUBLIC -sNO_DISABLE_EXCEPTION_CATCHING)
endif ()
