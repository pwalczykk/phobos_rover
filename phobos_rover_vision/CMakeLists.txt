cmake_minimum_required(VERSION 2.8.3)
project(phobos_rover_vision)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    tf
    phobos_shared
)

catkin_package()

include_directories(${catkin_INCLUDE_DIRS})

add_executable(tf_broadcaster
    src/tf_broadcaster/Main.cpp)
target_link_libraries(tf_broadcaster ${catkin_LIBRARIES})
