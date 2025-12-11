cmake_minimum_required(VERSION 3.10)
project(Platformer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Game.h
    src/Player.h
    src/Player.cpp
    src/Croco.cpp
    src/Croco.h
    src/Worm.cpp
    src/Worm.h
    src/Swamp.cpp
    src/Swamp.h
    src/Wizard.cpp
    src/Wizard.h
    src/pugixml.cpp
    src/pugixml.hpp
    src/pugiconfig.hpp
    src/SwampLevel.cpp
    src/SwampLevel.h
)

include_directories(${CMAKE_SOURCE_DIR}/test_kursovaya/include)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
    SFML::Graphics 
    SFML::Window 
    SFML::System
)
