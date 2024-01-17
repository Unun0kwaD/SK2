cmake_minimum_required(VERSION 3.10)

project(FootballGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.x)
FetchContent_MakeAvailable(SFML)


add_subdirectory(box2d)

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(${PROJECT_NAME} main.cpp Player.cpp Walls.cpp Ball.cpp Gate.cpp)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/)


target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

#target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/box2d/)

target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics box2d )

install(TARGETS ${PROJECT_NAME})