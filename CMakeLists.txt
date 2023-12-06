project(planetasrochosos)
add_executable(${PROJECT_NAME} main.cpp 
                                window.cpp 
                                solar.cpp 
                                planet.cpp
                                camera.cpp
                                space.cpp)
enable_abcg(${PROJECT_NAME})
