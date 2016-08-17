#ifndef SUBERROR_HPP_
#define SUBERROR_HPP_

#include <ros/ros.h>
#include <std_msgs/Int16.h>

class SubError{
    ros::NodeHandle *nh;
    ros::Subscriber sub;
public:
    std_msgs::Int16 msg;
public:
    SubError(std::string topic, ros::NodeHandle *nh){
        this->nh = nh;
        this->sub = nh->subscribe(topic, 100, &SubError::MsgInterrupt, this);
    }
    ~SubError(){}

    void MsgInterrupt(const std_msgs::Int16 msg){
        this->msg = msg;
    }
};

#endif
