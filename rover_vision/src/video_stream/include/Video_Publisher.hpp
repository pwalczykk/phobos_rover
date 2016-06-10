#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/CameraInfo.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream>

class Video_Publisher
{
    int cam_num;

    // ROS
    ros::NodeHandle nh;
    ros::Rate* loop_rate;

    image_transport::ImageTransport* it;
    image_transport::Publisher pub;

    ros::Publisher pub_info;

    sensor_msgs::ImagePtr img;
    sensor_msgs::CameraInfo cam_info;

    //OpenCV
    cv::Mat frame, frame_edited;
    cv::VideoCapture cap;
public:


    Video_Publisher(int cam_num);
    ~Video_Publisher();

    void Camera_init();

    void Capture_frame();
    void Edit_frame();
    void Publish_frame();
    void Publish_frame(cv::Mat frame);

    void Rate_sleep();
};
