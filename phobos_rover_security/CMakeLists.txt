cmake_minimum_required(VERSION 2.8.3)
project(phobos_rover_security)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")


find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  sensor_msgs
)


catkin_package(

  CATKIN_DEPENDS roscpp std_msgs

)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(security_controller
    src/Main.cpp
    src/AngleCalculator.cpp)
target_link_libraries(security_controller ${catkin_LIBRARIES})
