#ifndef TRAVELEDPATHPUBLISHER_HPP_
#define TRAVELEDPATHPUBLISHER_HPP_

#define MIN_DIST_SQR 1

#include <ros/ros.h>
#include "nav_msgs/Odometry.h"
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"

class TraveledPathPublisher{
    ros::NodeHandle nh;
    ros::Subscriber sub_odom;
    ros::Publisher pub_path;

protected:
    nav_msgs::Path traveled_path;
    geometry_msgs::PoseStamped current_pose;

public:
    TraveledPathPublisher(std::string topic_odom, std::string topic_path){
        sub_odom = nh.subscribe(topic_odom, 100, &TraveledPathPublisher::Callback, this);
        pub_path = nh.advertise<nav_msgs::Path>(topic_path, 100);
    }

    void Callback(const nav_msgs::Odometry msg_odom){
// Normal behavior for not-first step
        if(traveled_path.poses.size()){
            double dx = msg_odom.pose.pose.position.x - traveled_path.poses.back().pose.position.x;
            double dy = msg_odom.pose.pose.position.y - traveled_path.poses.back().pose.position.y;
            double dz = msg_odom.pose.pose.position.z - traveled_path.poses.back().pose.position.z;

            if(dx*dx + dy*dy + dz*dz > MIN_DIST_SQR){
                current_pose.header = msg_odom.header;
                current_pose.pose = msg_odom.pose.pose;

                traveled_path.header = msg_odom.header;
                traveled_path.poses.push_back(current_pose);

                Update();
            }
 // Behavior for first step
        }else{
                current_pose.header = msg_odom.header;
                current_pose.pose = msg_odom.pose.pose;

                traveled_path.header = msg_odom.header;
                traveled_path.poses.push_back(current_pose);

                Update();
        }
    }

    void Update(){
        pub_path.publish(traveled_path);
    }
};

#endif
