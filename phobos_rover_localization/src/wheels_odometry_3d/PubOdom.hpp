#ifndef PUBODOM_HPP_
#define PUBODOM_HPP_

#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include "DataStructs.hpp"

class PubOdom{
    ros::NodeHandle *nh;
    ros::Publisher pub;

public:
    nav_msgs::Odometry msg;
    DataOdometry *odometry;

public:
    PubOdom(std::string topic, DataOdometry *odometry ,ros::NodeHandle *nh){
        this->nh = nh;
        this->odometry = odometry;
        this->pub = nh->advertise<nav_msgs::Odometry>(topic, 100);

        msg.header.seq = 0;
        msg.header.stamp = ros::Time::now();
        msg.header.frame_id = "odom";
        msg.child_frame_id = "wo";
    }
    ~PubOdom(){}

    void Publish(){
        msg.header.seq++;
        msg.header.stamp = ros::Time::now();

        msg.pose.pose.position.x = odometry->position_x;
        msg.pose.pose.position.y = odometry->position_y;
        msg.pose.pose.position.z = odometry->position_z;

        msg.pose.pose.orientation.x = odometry->orientation_x;
        msg.pose.pose.orientation.y = odometry->orientation_y;
        msg.pose.pose.orientation.z = odometry->orientation_z;
        msg.pose.pose.orientation.w = odometry->orientation_w;

        pub.publish(msg);
    }
};

#endif
