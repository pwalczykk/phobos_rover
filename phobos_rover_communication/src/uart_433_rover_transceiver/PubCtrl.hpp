#ifndef PUBCTRL_HPP_
#define PUBCTRL_HPP_

#include <ros/ros.h>
#include <std_msgs/Int16.h>

class PubCtrl{
    ros::NodeHandle *nh;
    ros::Publisher pub;
public:
    std_msgs::Int16 msg;
public:
    PubCtrl(std::string topic, ros::NodeHandle *nh){
        this->nh = nh;
        this->pub = nh->advertise<std_msgs::Int16>(topic, 100);
    }
    ~PubCtrl(){}

    void Publish(){
        this->pub.publish(msg);
    }

    void Publish(int ctrl){
        this->msg.data = ctrl;
    }
};

#endif
