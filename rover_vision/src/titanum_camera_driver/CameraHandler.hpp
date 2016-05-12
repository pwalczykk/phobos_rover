#ifndef GETIMAGE_HPP_
#define GETIMAGE_HPP_

#include <ros/ros.h>
#include <opencv2/opencv.hpp>

class CameraHandler{
    cv::VideoCapture cap;

protected:
    cv::Mat frame;
    int cam_num;

public:
    CameraHandler(int cam_num){
        this->cam_num = cam_num;
        cap = cv::VideoCapture();
    }

    void Init(){
        cap.open(cam_num);
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
