#ifndef SUBODOM_HPP_
#define SUBODOM_HPP_

#include <ros/ros.h>
#include <nav_msgs/Odometry.h>

class SubOdom{
    ros::NodeHandle *nh;
    ros::Subscriber sub;
public:
    nav_msgs::Odometry msg;
public:
    SubOdom(std::string topic, ros::NodeHandle *nh){
        this->nh = nh;
        this->sub = nh->subscribe(topic, 100, &SubOdom::MsgInterrupt, this);
    }
    ~SubOdom(){}

    void MsgInterrupt(const nav_msgs::Odometry msg){
        this->msg = msg;
    }
};

#endif
