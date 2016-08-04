#ifndef SUBODOM_HPP_
#define SUBODOM_HPP_

#include <ros/ros.h>
#include <geometry_msgs/Pose.h>

class SubOdom{
    ros::NodeHandle *nh;
    ros::Subscriber sub;
public:
    geometry_msgs::Pose msg;
public:
    SubOdom(std::string topic, ros::NodeHandle *nh){
        this->nh = nh;
        this->sub = nh->subscribe(topic, 100, &SubOdom::MsgInterrupt, this);
    }
    ~SubOdom(){}

    void MsgInterrupt(const geometry_msgs::Pose msg){
        this->msg = msg;
    }
};

#endif
