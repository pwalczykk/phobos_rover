#ifndef ROS_UART_RX_IMU_HPP_
#define ROS_UART_RX_IMU_HPP_

#define ACCEL_SCALE_RANGE 2
#define GYRO_SCALE_RANGE 250
#define MAGNETOMETER_SCALE_RANGE 1.3

#define Z_ACCEL_OFFSET 1800
#define Y_ACCEL_OFFSET 130
#define X_ACCEL_OFFSET -800

#define Z_GYRO_OFFSET 0
#define Y_GYRO_OFFSET 0
#define X_GYRO_OFFSET 0

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/MagneticField.h>
#include <iostream>
#include <string.h>
#include "UART_Rx.hpp"

class ROS_UART_Rx : public UART_Rx{

    ros::NodeHandle *nh;

    ros::Publisher pubimu, pubmag;

	int error_counter;

	sensor_msgs::Imu msgimu;
	sensor_msgs::MagneticField msgmag;
	std::stringstream dataStream;
    float xlin, ylin, zlin, xang, yang, zang, xmag, ymag, zmag;

public:
    ROS_UART_Rx(std::string device_addres, std::string topicimu, std::string topicmag, ros::NodeHandle* nh) : UART_Rx((const char*) device_addres.c_str()){

        this->nh = nh;

        pubimu = nh->advertise<sensor_msgs::Imu>(topicimu, 10);
        pubmag = nh->advertise<sensor_msgs::MagneticField>(topicmag, 10);


        msgimu.header.frame_id = "odom";
        msgmag.header.frame_id = "odom";


        msgimu.linear_acceleration_covariance = {	1e-6,		0   ,	 0,
													0	 ,	 	1e-6,	 0,
													0	 ,		0	,	 1e-6	};

		msgimu.angular_velocity_covariance = 	{	1e-6,		0   ,	 0,
													0	 ,	 	1e-6,	 0,
													0	 ,		0	,	 1e-6	};

		msgimu.orientation_covariance = 		{	1e-9,		0   ,	 0,
													0	 ,	 	1e-9,	 0,
													0	 ,		0	,	 1e-9	};

		msgmag.magnetic_field_covariance = 		{	1e-6,		0   ,	 0,
													0	 ,	 	1e-6,	 0,
													0	 ,		0	,	 1e-6	};

	    error_counter = 0;
    }

    ~ROS_UART_Rx(){}

    void CheckReceiveBuffer(){
        if(UART_Rx::ReadBuffer()){

			//read data into stringstream
            dataStream.str(std::string(UART_Rx::rx_buffer));

            //extract data into numerical variables
            dataStream >> xmag >> zmag >> ymag >> xlin >> ylin >> zlin >> xang >> yang >> zang;

            /*

            //compose mag message
            msgmag.magnetic_field.x = xmag;//(xmag/2047)*MAGNETOMETER_SCALE_RANGE;
            msgmag.magnetic_field.y = ymag;//(ymag/2047)*MAGNETOMETER_SCALE_RANGE;
            msgmag.magnetic_field.z = zmag;//(zmag/2047)*MAGNETOMETER_SCALE_RANGE;

            //compose IMU message
            msgimu.linear_acceleration.x = xlin;//((xlin+(X_ACCEL_OFFSET))/32767)*ACCEL_SCALE_RANGE*9.81;
            msgimu.linear_acceleration.y = ylin;//((ylin+(Y_ACCEL_OFFSET))/32767)*ACCEL_SCALE_RANGE*9.81;
            msgimu.linear_acceleration.z = zlin;//((zlin+(Z_ACCEL_OFFSET))/32767)*ACCEL_SCALE_RANGE*9.81;
            msgimu.angular_velocity.x = xang;//((xang+(X_GYRO_OFFSET))/32767)*(GYRO_SCALE_RANGE)*(2*3.14159/360);
            msgimu.angular_velocity.y = yang;//((yang+(Y_GYRO_OFFSET))/32767)*(GYRO_SCALE_RANGE)*(2*3.14159/360);
            msgimu.angular_velocity.z = zang;//((zang+(Z_GYRO_OFFSET))/32767)*(GYRO_SCALE_RANGE)*(2*3.14159/360);
            */


            //compose mag message
            msgmag.magnetic_field.x = (ymag/2047)*MAGNETOMETER_SCALE_RANGE;
            msgmag.magnetic_field.y = (-xmag/2047)*MAGNETOMETER_SCALE_RANGE;
            msgmag.magnetic_field.z = (zmag/2047)*MAGNETOMETER_SCALE_RANGE;

            //compose IMU message
            msgimu.linear_acceleration.x = ((xlin+(X_ACCEL_OFFSET))/32767)*ACCEL_SCALE_RANGE*9.81;
            msgimu.linear_acceleration.y = ((ylin+(Y_ACCEL_OFFSET))/32767)*ACCEL_SCALE_RANGE*9.81;
            msgimu.linear_acceleration.z = ((zlin+(Z_ACCEL_OFFSET))/32767)*ACCEL_SCALE_RANGE*9.81;
            msgimu.angular_velocity.x = ((xang+(X_GYRO_OFFSET))/32767)*(GYRO_SCALE_RANGE)*(2*3.14159/360);
            msgimu.angular_velocity.y = ((yang+(Y_GYRO_OFFSET))/32767)*(GYRO_SCALE_RANGE)*(2*3.14159/360);
            msgimu.angular_velocity.z = ((zang+(Z_GYRO_OFFSET))/32767)*(GYRO_SCALE_RANGE)*(2*3.14159/360);



           // printf("Publikuje\n");
            //publish
            pubimu.publish(msgimu);
            pubmag.publish(msgmag);

            //zero out the buffer, as we don't need the data anymore
            bzero(UART_Rx::rx_buffer, sizeof(UART_Rx::rx_buffer));

        }
		else{
			ROS_FATAL("IMU ERROR - Cant read  buffer");
			error_counter++;
			if(error_counter > 5){
				exit(-1);
			}
		}
    }
};

#endif
