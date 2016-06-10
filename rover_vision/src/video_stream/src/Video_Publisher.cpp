#include "../include/Video_Publisher.hpp"

Video_Publisher::Video_Publisher(int cam_num)
{
    this->cam_num = cam_num;
    std::ostringstream oss_image, oss_info;
    oss_image << "camera" << cam_num << "/image";
    oss_info << "camera" << cam_num <<"/camera_info";

    loop_rate = new ros::Rate(10);
    it = new image_transport::ImageTransport(nh);
    pub = it->advertise(oss_image.str(), 1);

    pub_info = nh.advertise<sensor_msgs::CameraInfo>(oss_info.str(), 1);


}

Video_Publisher::~Video_Publisher()
{}

void Video_Publisher::Camera_init()
{
    cap = cv::VideoCapture(cam_num);
    cap.set(CV_CAP_PROP_FRAME_WIDTH,320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,240);
    cap.set(CV_CAP_PROP_FPS,10);
}

void Video_Publisher::Capture_frame()
{
    //cap >> frame;
}

void Video_Publisher::Edit_frame()
{
//    cv::cvtColor(frame,frame,CV_BGR2HSV);
}

void Video_Publisher::Publish_frame()
{
    if(!frame.empty())
    {
        img = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
        cam_info.header = img->header;
        cam_info.height = img->height;
        cam_info.width = img->width;

        pub.publish(img);
        pub_info.publish(cam_info);

        cv::waitKey(1);
    }
}

void Video_Publisher::Publish_frame(cv::Mat frame)
{
    if(!frame.empty())
    {
        img = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
        cam_info.header = img->header;
        cam_info.height = img->height;
        cam_info.width = img->width;

        pub.publish(img);
        pub_info.publish(cam_info);

        cv::waitKey(1);
    }
}

void Video_Publisher::Rate_sleep()
{
    ros::spinOnce();
    loop_rate->sleep();
}
