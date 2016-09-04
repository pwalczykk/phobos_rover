#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <unistd.h>

int main(int argc, char** argv){
    ros::init(argc, argv, "synchronizer");
    ros::NodeHandle nh("~");
    ros::Publisher pub = nh.advertise<std_msgs::Bool>("/rover/camera/synchronization", 100);

    float sync_delay;
    nh.param("sync_delay", sync_delay, (float)4.0);
    int sync_delay_us = (int) (sync_delay * 1000000);

    usleep(sync_delay_us);

    std_msgs::Bool msg;
    msg.data = true;

    pub.publish(msg);
    ROS_INFO("cameras synchronization");

    return 0;
}
