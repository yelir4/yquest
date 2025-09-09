cmake_minimum_required(VERSION 4.1.0) 
project(yquest)

#use C++17 features
set(CMAKE_CXX_STANDARD 17)

#where is SDL
set(CMAKE_PREFIX_PATH "C:/Tools/SDL3-3.2.20")
find_package(SDL3 REQUIRED)

#SDL true text
set(SDL3_TTF_PATH "C:/Tools/SDL3_ttf-3.2.2")
find_package(SDL3_ttf REQUIRED)


#yquest executable (place sources here)
add_executable(yquest src/main.cpp src/player.cpp src/gamepad.cpp)

#link SDL3 libraries
target_link_libraries(yquest PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)

#copy SDL3.dll next to executable
add_custom_command(TARGET yquest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_PREFIX_PATH}/x86_64-w64-mingw32/bin/SDL3.dll"
    $<TARGET_FILE_DIR:yquest>
)

#copy SDL3_ttf.dll next to executable
add_custom_command(TARGET yquest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL3_TTF_PATH}/x86_64-w64-mingw32/bin/SDL3_ttf.dll"
    $<TARGET_FILE_DIR:yquest>
)