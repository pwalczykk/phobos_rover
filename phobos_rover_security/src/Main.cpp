#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include "../include/AngleCalculator.hpp"

AngleCalculator cal;


void RespondImu(const sensor_msgs::Imu::ConstPtr& msg){

	cal.CalculateAngle(msg->orientation.x, msg->orientation.y, msg->orientation.z, msg->orientation.w);
	cal.CheckForSpike(msg->linear_acceleration.x,msg->linear_acceleration.y,msg->linear_acceleration.z);

}

int main(int argc, char **argv){

	ros::init(argc, argv, "security_controller");
	ros::NodeHandle nh;
	ros::Rate lr(10);

	cal.Init("/rover/security/error_code", &nh, 30, 5);

	ros::Subscriber subimu;

	subimu = nh.subscribe("/imu/data", 10, RespondImu);


	ros::spin();
	lr.sleep();

	return 0;
}
