#ifndef SUBENCODERS_HPP_
#define SUBENCODERS_HPP_

template <typename ros_Message>
class SubEncoders{
    ros::NodeHandle *nh;
    ros::Subscriber sub;
public:
    ros_Message msg;
public:
    SubEncoders(std::string topic, ros::NodeHandle *nh){
        this->nh = nh;
        this->sub = nh->subscribe(topic, 100, &SubEncoders::MsgInterrupt, this);
    }
    ~SubEncoders(){}

    void MsgInterrupt(const ros_Message msg){
        this->msg = msg;
    }
};
#endif
