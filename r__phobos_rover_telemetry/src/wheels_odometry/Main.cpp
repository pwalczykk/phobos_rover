#include <ros/ros.h>

#include <phobos_shared/EncodersWheels.h>

#include "DataStructs.hpp"
#include "OdometryCalculator.hpp"
#include "SubEncoders.hpp"
#include "SubOdom.hpp"
#include "PubOdom.hpp"


int main(int argc, char** argv){
    ros::init(argc, argv, "wheels_odometry");
    ros::NodeHandle nh;

    float wheel_circumference, wheels_spacing;
    int impulses_per_rotation;
    nh.param("/wheel_circumference", wheel_circumference, (float)1.0);
    nh.param("/wheels_spacing", wheels_spacing, (float)1.0);
    nh.param("/impulses_per_rotation", impulses_per_rotation, 1000);

    DataOdometry odometry;
    DataEncoders encoders;

    SubOdom sub_odom("/rover/localization/odom_ekf",&odometry ,&nh);
    SubEncoders sub_encoders("/rover/encoders/wheels_relative",&encoders, &nh);

    PubOdom pub_odom("/rover/localization/wo",&odometry ,&nh);

    OdometryCalculcator odometry_calculator(&odometry, &encoders);
    odometry_calculator.wheel_circumference = wheel_circumference;
    odometry_calculator.wheels_spacing = wheels_spacing;
    odometry_calculator.impulses_per_rotation = impulses_per_rotation;

    ros::Rate loop_rate(10);

    while(ros::ok()){
        ros::spinOnce();

        if(sub_odom.new_msg && sub_encoders.new_msg){
            odometry_calculator.Update();
            pub_odom.Publish();
            sub_encoders.Reset();
            sub_odom.Reset();
        }

        loop_rate.sleep();
    }
    return 0;
}
