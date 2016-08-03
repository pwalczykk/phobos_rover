#ifndef SUBPOSEORIENT_HPP_
#define SUBPOSEORIENT_HPP_

#include <ros/ros.h>
#include <geometry_msgs/Pose.h>

class SubPoseOrient{
    ros::NodeHandle *nh;
    ros::Subscriber sub;
public:
    geometry_msgs::Pose msg;
public:
    SubPoseOrient(std::string topic, ros::NodeHandle *nh){
        this->nh = nh;
        this->sub = nh->subscribe(topic, 100, &SubPoseOrient::MsgInterrupt, this);
    }
    ~SubPoseOrient(){}

    void MsgInterrupt(const geometry_msgs::Pose msg){
        this->msg = msg;
    }
};

#endif
