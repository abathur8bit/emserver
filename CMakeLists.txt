cmake_minimum_required(VERSION 3.12)
project(EMServer)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        .
        /usr/local/include
)

link_directories(
        /usr/local/lib
)

add_executable(emserver
    area.cpp
    clienthandler.cpp
    configparser.cpp
    emscript.cpp
    emserver.cpp
    exparse.cpp
    mudobject.cpp
    player.cpp
    room.cpp
    smallbasic.cpp
    world.cpp
    stringtokenizer.cpp
    main.cpp)
target_link_libraries(emserver PRIVATE ssobjects pthread)

add_executable(hello hello.cpp)