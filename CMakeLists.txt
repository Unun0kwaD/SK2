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

# add_executable(${PROJECT_NAME} main.cpp Player.cpp Walls.cpp Ball.cpp Gate.cpp game_state.cpp)
add_executable(Client client.cpp Player.cpp Walls.cpp Ball.cpp Gate.cpp game_state.cpp client_handler.cpp)
add_executable(Server server.cpp Player.cpp Walls.cpp Ball.cpp Gate.cpp game_state.cpp room.cpp)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources/)


target_include_directories(Client PRIVATE ${INCLUDE_DIRS})
target_include_directories(Server PRIVATE ${INCLUDE_DIRS})

#target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/box2d/)

target_link_libraries(Client sfml-system sfml-window sfml-graphics box2d )
target_link_libraries(Server sfml-system sfml-window sfml-graphics box2d )

install(TARGETS Client)
install(TARGETS Server)