cmake_minimum_required(VERSION 3.25)
project(snake)

set(CMAKE_CXX_STANDARD 14)

add_executable(snake src/main.cpp src/window.cpp src/Game.cpp src/Game.h src/Apple.cpp src/Apple.h src/SnakeTile.cpp src/SnakeTile.h src/Camera.cpp src/Camera.h)
target_include_directories(snake PRIVATE C:/SDL2/include)
target_link_directories(snake PRIVATE C:/SDL2/lib)
target_link_libraries(snake PRIVATE -static-libgcc -static-libstdc++ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer)
