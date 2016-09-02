#ifndef IMAGE_WINDOW_HPP_
#define IMAGE_WINDOW_HPP_

#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

class ImageWindow{
    ros::NodeHandle *nh;
    image_transport::ImageTransport *it;
    image_transport::Subscriber sub;

    std::string window;

public:
    ImageWindow(ros::NodeHandle *nh, image_transport::ImageTransport *it, std::string topic, std::string window){
        this->nh = nh;
        this->it = it;
        this->window = window;
        sub = it->subscribe(topic, 1, &ImageWindow::MsgInterrupt, this);
        cv::namedWindow(this->window, CV_WINDOW_NORMAL);
        cv::setWindowProperty(this->window, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    }

    ~ImageWindow(){
        cv::destroyWindow(this->window);
    }

    void ResizeWindow(int width, int length){
        cv::resizeWindow(this->window, width, length);
    }

    void MoveWindow(int x, int y){
        cv::moveWindow(this->window, x, y);
    }

    void MsgInterrupt(const sensor_msgs::ImageConstPtr& msg){
        try{
            cv::imshow(this->window, cv_bridge::toCvShare(msg, "bgr8")->image);
            cv::waitKey(10);
        }
        catch (cv_bridge::Exception& e){
             ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
        }
    }
};

#endif
