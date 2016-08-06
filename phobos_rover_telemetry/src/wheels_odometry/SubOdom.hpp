#ifndef SUBODOM_HPP_
#define SUBODOM_HPP_

#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include "DataStructs.hpp"

class SubOdom{
    ros::NodeHandle *nh;
    ros::Subscriber sub;

public:
    bool new_msg;
    DataOdometry *odometry;

public:
    SubOdom(std::string topic, DataOdometry *odometry ,ros::NodeHandle *nh){
        this->nh = nh;
        this->odometry = odometry;
        this->sub = nh->subscribe(topic, 100, &SubOdom::MsgInterrupt, this);
    }
    ~SubOdom(){}

    void MsgInterrupt(const nav_msgs::Odometry msg){
        this->odometry->position_x = msg.pose.pose.position.x;
        this->odometry->position_y = msg.pose.pose.position.y;
        this->odometry->position_z = msg.pose.pose.position.z;

        this->odometry->orientation_x = msg.pose.pose.orientation.x;
        this->odometry->orientation_y = msg.pose.pose.orientation.y;
        this->odometry->orientation_z = msg.pose.pose.orientation.z;
        this->odometry->orientation_w = msg.pose.pose.orientation.w;

        this->new_msg = true;
    }

    void Reset(){
        this->new_msg = false;
    }
};

#endif
