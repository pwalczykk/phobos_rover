#ifndef PUBARMVEL_HPP_
#define PUBARMVEL_HPP_

#include <ros/ros.h>
#include <phobos_shared/ArmVel16.h>

class PubArmVel{
    ros::NodeHandle *nh;
    ros::Publisher pub;
public:
    phobos_shared::ArmVel16 msg;
public:
    PubArmVel(std::string topic, ros::NodeHandle *nh){
        this->nh = nh;
        this->pub = nh->advertise<phobos_shared::ArmVel16>(topic, 100);
    }
    ~PubArmVel(){}

    void Publish(){
        this->pub.publish(msg);
    }

    void Publish(int link_0, int link_1, int link_2, int link_3, int link_4, int gripper){
        this->msg.link_0 = link_0;
        this->msg.link_1 = link_1;
        this->msg.link_2 = link_2;
        this->msg.link_3 = link_3;
        this->msg.link_4 = link_4;
        this->msg.gripper = gripper;
        this->pub.publish(msg);
    }
};

#endif
