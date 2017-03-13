#ifndef VOIDIMAGESUBSCRIBER_HPP_
#define VOIDIMAGESUBSCRIBER_HPP_

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

#include "PubCamInfo.hpp"


class VoidImageSubscriber{
    ros::NodeHandle *nh;
    image_transport::ImageTransport *it;

    image_transport::Subscriber sub_cam1;
    image_transport::Subscriber sub_cam2;

    sensor_msgs::Image img1;
    sensor_msgs::Image img2;

    image_transport::Publisher pub_cam1;
    image_transport::Publisher pub_cam2;

    PubCamInfo *cam_info1;
    PubCamInfo *cam_info2;

    bool new_img1;
    bool new_img2;

public:
    VoidImageSubscriber(ros::NodeHandle *nh, image_transport::ImageTransport *it, PubCamInfo *cam_info1, std::string topic1, PubCamInfo *cam_info2, std::string topic2){
        this->nh = nh;
        this->it = it;
        sub_cam1 = it->subscribe(topic1, 1, &VoidImageSubscriber::MsgInterrupt1, this);
        sub_cam2 = it->subscribe(topic2, 1, &VoidImageSubscriber::MsgInterrupt2, this);

        pub_cam1 = it->advertise(topic1 + "2", 1);
        pub_cam2 = it->advertise(topic2 + "2", 1);

        this->cam_info1 = cam_info1;
        this->cam_info2 = cam_info2;

        new_img1 = false;
        new_img2 = false;
    }

    void MsgInterrupt1(const sensor_msgs::ImageConstPtr& msg){
        new_img1 = true;

        img1.header = cam_info1->msg.header;
        img1.height = msg->height;
        img1.width = msg->width;
        img1.encoding = msg->encoding;
        img1.is_bigendian = msg->is_bigendian;
        img1.step = msg->step;
        img1.data = msg->data;

        if(!new_img2){
            cam_info1->msg.header.stamp = ros::Time::now();
        }else{
            cam_info1->msg.header.stamp = cam_info2->msg.header.stamp;

            cam_info1->Publish(); new_img1 = false;
            cam_info2->Publish(); new_img2 = false;

            pub_cam1.publish(img1);
            pub_cam2.publish(img2);

        }
    }

    void MsgInterrupt2(const sensor_msgs::ImageConstPtr& msg){
        new_img2 = true;

        img2.header = cam_info2->msg.header;
        img2.height = msg->height;
        img2.width = msg->width;
        img2.encoding = msg->encoding;
        img2.is_bigendian = msg->is_bigendian;
        img2.step = msg->step;
        img2.data = msg->data;

        if(!new_img1){
            cam_info2->msg.header.stamp = ros::Time::now();
        }else{
            cam_info2->msg.header.stamp = cam_info1->msg.header.stamp;

            cam_info1->Publish(); new_img1 = false;
            cam_info2->Publish(); new_img2 = false;

            pub_cam1.publish(img1);
            pub_cam2.publish(img2);

        }

    }
};

#endif
