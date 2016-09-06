#include <ros/ros.h>
#include "../include/AngleCalculator.hpp"
#include "../include/ErrorCodes.h"
#include <std_msgs/Int16.h>

AngleCalculator::AngleCalculator(const char* topic, ros::NodeHandle *nh, float maxang, float maxj)
{

	Init(topic,nh,maxang,maxj);

}

void AngleCalculator::CalculateAngle(float x, float y, float z , float w)
{

	float g[3] = {0, 0, 0};
	float angle = 1;

	g[0] = 1*(2 * (x * z - w * y));
	g[1] = 1*(2 * (w * x + y * z));
	g[2] = 1*(w * w - x * x - y * y + z * z);

	angle = asin( (fabs(g[2])) / (sqrt( pow(g[0],2) + pow(g[1],2) + pow(g[2],2) )) );
	angle = 1.5708 - angle;
	angle = angle*180/3.1416;

	printf("%f %f %f\n", g[0],g[1],g[2]);
	printf("%f\n", angle);

	this->msg.data = ABOUT_TO_FALL_OVER;
	if(angle >= maxangle) pub.publish(this->msg);

}

void AngleCalculator::CheckForSpike(float accx, float accy, float accz){

	float jerkx = accx - accxlast;
	float jerky = accy - accylast;
	float jerkz = accz - acczlast;

	accxlast = accx;
	accylast = accy;
	acczlast = accz;

	float jerklength = sqrt( pow(jerkx,2)+pow(jerky,2)+pow(jerkz,2) );

	printf("Jerk: %f\n", jerklength);

	this->msg.data = COLLISION_DETECTED;
	if(jerklength >= maxjerk) pub.publish(this->msg);

	// if(jerklength >= maxjerk) pub.publish(COLLISION_DETECTED);	// Nie można publikować bezpośrednio inta, tylko messaga

}

void AngleCalculator::Init(const char* topic, ros::NodeHandle* nh, float maxang, float maxj)
{
    pub = nh->advertise<std_msgs::Int16>(topic, 100);
    this->maxangle = maxang;
    this->maxjerk = maxj;
}
