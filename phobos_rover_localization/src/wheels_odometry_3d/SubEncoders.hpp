#ifndef SUBENCODERS_HPP_
#define SUBENCODERS_HPP_

#include <ros/ros.h>
#include <phobos_shared/EncodersWheels.h>
#include "DataStructs.hpp"

class SubEncoders{
    ros::NodeHandle *nh;
    ros::Subscriber sub;

public:
    bool new_msg;
    DataEncoders *encoders;

public:
    SubEncoders(std::string topic, DataEncoders *encoders,ros::NodeHandle *nh){
        this->nh = nh;
        this->encoders = encoders;
        this->sub = nh->subscribe(topic, 100, &SubEncoders::MsgInterrupt, this);
    }
    ~SubEncoders(){}

    void MsgInterrupt(const phobos_shared::EncodersWheels msg){
        this->encoders->wheel_imps_fl += msg.wheel_vel_fl;
        this->encoders->wheel_imps_fr += msg.wheel_vel_fr;
        this->encoders->wheel_imps_ml += msg.wheel_vel_ml;
        this->encoders->wheel_imps_mr += msg.wheel_vel_mr;
        this->encoders->wheel_imps_bl += msg.wheel_vel_bl;
        this->encoders->wheel_imps_br += msg.wheel_vel_br;
        this->new_msg = true;
    }

    void Reset(){
        this->encoders->wheel_imps_fl = 0;
        this->encoders->wheel_imps_fr = 0;
        this->encoders->wheel_imps_ml = 0;
        this->encoders->wheel_imps_mr = 0;
        this->encoders->wheel_imps_bl = 0;
        this->encoders->wheel_imps_br = 0;
        this->new_msg = false;
    }
};
#endif
