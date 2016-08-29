#ifndef CAMERA_HANDLER_HPP_
#define CAMERA_HANDLER_HPP_

#include <ros/ros.h>
#include <opencv2/opencv.hpp>

class CameraHandler{
    cv::VideoCapture cap;

protected:
    cv::Mat frame;
    std::string stream_addres;

public:
    CameraHandler(std::string stream_addres){
        this->stream_addres = stream_addres;
        cap = cv::VideoCapture();
    }

    void Init(){
        cap.open(stream_addres, cv::CAP_IMAGES);
        if(!cap.open(stream_addres)){
            ROS_ERROR_STREAM("CAN'T OPEN CAMERA STREAM FROM ADDRES:");
            ROS_ERROR_STREAM(stream_addres);
            exit(-1);
        }
    }

    void Release(){
        cap.release();
    }

    void Set(int res_x, int res_y, int fps){
        // cap.set(CV_CAP_PROP_FRAME_WIDTH,res_x);
        // cap.set(CV_CAP_PROP_FRAME_HEIGHT,res_y);
        // cap.set(CV_CAP_PROP_FPS,fps);
    }

    void Capture(){
        cap >> frame;
    }
};

#endif
