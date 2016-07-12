#ifndef IMAGEPUBLISHER_HPP_
#define IMAGEPUBLISHER_HPP_

#include "CameraHandler.hpp"

#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/CameraInfo.h>
#include <std_msgs/Header.h>

class ImagePublisher : public CameraHandler{
    ros::NodeHandle nh;
    image_transport::ImageTransport* it;
    image_transport::Publisher pub_img;

protected:
    sensor_msgs::ImagePtr img;
    std_msgs::Header header;

public:
    ImagePublisher(std::string stream_addres, std::string publisher_topic) : CameraHandler(stream_addres){
        it = new image_transport::ImageTransport(nh);
        pub_img = it->advertise(publisher_topic, 1);

        header.seq = 0;
        header.frame_id = "0";

    }

    void PublishImage(){
        header.seq++;
        header.stamp = ros::Time::now();

        CameraHandler::Capture();

        img = cv_bridge::CvImage(header, "bgr8", CameraHandler::frame).toImageMsg();

        pub_img.publish(img);
    }
};

#endif
