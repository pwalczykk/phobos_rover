cmake_minimum_required(VERSION 2.8.3)
project(phobos_rover_localization)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

find_package(catkin REQUIRED COMPONENTS
    roscpp
    phobos_shared
)

catkin_package()

include_directories(${catkin_INCLUDE_DIRS})

add_executable(wheels_odometry_3d
    src/wheels_odometry_3d/Main.cpp)
target_link_libraries(wheels_odometry_3d ${catkin_LIBRARIES})
