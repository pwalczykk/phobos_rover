#ifndef ANGLE_CALCULATOR_HPP_
#define ANGLE_CALCULATOR_HPP_

#include <ros/ros.h>
#include <std_msgs/Int16.h>

class AngleCalculator{

	ros::Publisher pub;
	ros::NodeHandle *nh;
	float maxangle, maxjerk, accxlast, accylast, acczlast, accelx, accely, accelz;

	std_msgs::Int16 msg;

	public:

	AngleCalculator(){};
	AngleCalculator(const char* topic, ros::NodeHandle *nh, float maxang, float maxj);

	void CalculateAngle(float x, float y, float z , float w);
	void Init(const char* topic, ros::NodeHandle* nh, float maxang,float maxj);
	void CheckForSpike(float accx, float accy, float accz);

};

#endif
