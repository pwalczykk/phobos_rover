#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>



void ImageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try{
    cv_bridge::CvImageConstPtr TemporaryImagePtr = cv_bridge::toCvShare(msg, "bgr8");
    cv::imshow("view", TemporaryImagePtr->image);
    cv::waitKey(30);
  }
  catch(cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  cv::namedWindow("view");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("/test/image_stream", 100, ImageCallback);
  ros::spin();
  cv::destroyWindow("view");
}
