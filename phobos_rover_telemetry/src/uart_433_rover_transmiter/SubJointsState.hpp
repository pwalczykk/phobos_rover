#ifndef SUBJOINTSSTATE_HPP_
#define SUBJOINTSSTATE_HPP_

#include <ros/ros.h>
#include <phobos_shared/JointsState16.h>

class SubJointsState{
    ros::NodeHandle *nh;
    ros::Subscriber sub;
public:
    phobos_shared::JointsState16 msg;
public:
    SubJointsState(std::string topic, ros::NodeHandle *nh){
        this->nh = nh;
        this->sub = nh->subscribe(topic, 100, &SubJointsState::MsgInterrupt, this);
    }
    ~SubJointsState(){}

    void MsgInterrupt(const phobos_shared::JointsState16 msg){
        this->msg = msg;
    }
};

#endif
