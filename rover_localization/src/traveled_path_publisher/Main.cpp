#include <ros/ros.h>
#include "TraveledPathPublisher.hpp"

int main(int argc, char** argv){
    ros::init(argc, argv, "traveled_path_publisher");
    ros::NodeHandle nh;

    TraveledPathPublisher traveled_path_publisher("/rover/odometry/filtered", "/rover/path");

    ros::spin();

    return 0;
}
