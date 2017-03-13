#include <ros/ros.h>
#include "PubCamInfo.hpp"
#include "VoidImageSubscriber.hpp"

int main(int argc, char** argv){
    ros::init(argc, argv, "cam_info_publisher2");
    ros::NodeHandle nh("~");
    image_transport::ImageTransport it(nh);

    std::string camera_name1, camera_name2;
    nh.getParam("camera_name1", camera_name1);
    nh.getParam("camera_name2", camera_name2);

    PubCamInfo cam_info1(camera_name1, 320, 240, &nh);
    PubCamInfo cam_info2(camera_name2, 320, 240, &nh);

    VoidImageSubscriber stereo_info(&nh, &it, &cam_info1, "/rover/" + camera_name1 +"/image", &cam_info2, "/rover/" + camera_name2 + "/image");

    // nh.param("cam1_D0", cam_info1.msg.D[0], (double)0.0);
    // nh.param("cam1_D1", cam_info1.msg.D[1], (double)0.0);
    // nh.param("cam1_D2", cam_info1.msg.D[2], (double)0.0);
    // nh.param("cam1_D3", cam_info1.msg.D[3], (double)0.0);
    // nh.param("cam1_D4", cam_info1.msg.D[4], (double)0.0);
    // nh.param("cam1_D5", cam_info1.msg.D[5], (double)0.0);

    nh.param("cam1_K0", cam_info1.msg.K[0], (double)0.0);
    nh.param("cam1_K1", cam_info1.msg.K[1], (double)0.0);
    nh.param("cam1_K2", cam_info1.msg.K[2], (double)0.0);
    nh.param("cam1_K3", cam_info1.msg.K[3], (double)0.0);
    nh.param("cam1_K4", cam_info1.msg.K[4], (double)0.0);
    nh.param("cam1_K5", cam_info1.msg.K[5], (double)0.0);
    nh.param("cam1_K6", cam_info1.msg.K[6], (double)0.0);
    nh.param("cam1_K7", cam_info1.msg.K[7], (double)0.0);
    nh.param("cam1_K8", cam_info1.msg.K[8], (double)0.0);

    nh.param("cam1_R0", cam_info1.msg.R[0], (double)0.0);
    nh.param("cam1_R1", cam_info1.msg.R[1], (double)0.0);
    nh.param("cam1_R2", cam_info1.msg.R[2], (double)0.0);
    nh.param("cam1_R3", cam_info1.msg.R[3], (double)0.0);
    nh.param("cam1_R4", cam_info1.msg.R[4], (double)0.0);
    nh.param("cam1_R5", cam_info1.msg.R[5], (double)0.0);
    nh.param("cam1_R6", cam_info1.msg.R[6], (double)0.0);
    nh.param("cam1_R7", cam_info1.msg.R[7], (double)0.0);
    nh.param("cam1_R8", cam_info1.msg.R[8], (double)0.0);

    nh.param("cam1_P0", cam_info1.msg.P[0], (double)0.0);
    nh.param("cam1_P1", cam_info1.msg.P[1], (double)0.0);
    nh.param("cam1_P2", cam_info1.msg.P[2], (double)0.0);
    nh.param("cam1_P3", cam_info1.msg.P[3], (double)0.0);
    nh.param("cam1_P4", cam_info1.msg.P[4], (double)0.0);
    nh.param("cam1_P5", cam_info1.msg.P[5], (double)0.0);
    nh.param("cam1_P6", cam_info1.msg.P[6], (double)0.0);
    nh.param("cam1_P7", cam_info1.msg.P[7], (double)0.0);
    nh.param("cam1_P8", cam_info1.msg.P[8], (double)0.0);
    nh.param("cam1_P9", cam_info1.msg.P[9], (double)0.0);
    nh.param("cam1_P10", cam_info1.msg.P[10], (double)0.0);
    nh.param("cam1_P11", cam_info1.msg.P[11], (double)0.0);

    // nh.param("cam2_D0", cam_info2.msg.D[0], (double)0.0);
    // nh.param("cam2_D1", cam_info2.msg.D[1], (double)0.0);
    // nh.param("cam2_D2", cam_info2.msg.D[2], (double)0.0);
    // nh.param("cam2_D3", cam_info2.msg.D[3], (double)0.0);
    // nh.param("cam2_D4", cam_info2.msg.D[4], (double)0.0);
    // nh.param("cam2_D5", cam_info2.msg.D[5], (double)0.0);

    nh.param("cam2_K0", cam_info2.msg.K[0], (double)0.0);
    nh.param("cam2_K1", cam_info2.msg.K[1], (double)0.0);
    nh.param("cam2_K2", cam_info2.msg.K[2], (double)0.0);
    nh.param("cam2_K3", cam_info2.msg.K[3], (double)0.0);
    nh.param("cam2_K4", cam_info2.msg.K[4], (double)0.0);
    nh.param("cam2_K5", cam_info2.msg.K[5], (double)0.0);
    nh.param("cam2_K6", cam_info2.msg.K[6], (double)0.0);
    nh.param("cam2_K7", cam_info2.msg.K[7], (double)0.0);
    nh.param("cam2_K8", cam_info2.msg.K[8], (double)0.0);

    nh.param("cam2_R0", cam_info2.msg.R[0], (double)0.0);
    nh.param("cam2_R1", cam_info2.msg.R[1], (double)0.0);
    nh.param("cam2_R2", cam_info2.msg.R[2], (double)0.0);
    nh.param("cam2_R3", cam_info2.msg.R[3], (double)0.0);
    nh.param("cam2_R4", cam_info2.msg.R[4], (double)0.0);
    nh.param("cam2_R5", cam_info2.msg.R[5], (double)0.0);
    nh.param("cam2_R6", cam_info2.msg.R[6], (double)0.0);
    nh.param("cam2_R7", cam_info2.msg.R[7], (double)0.0);
    nh.param("cam2_R8", cam_info2.msg.R[8], (double)0.0);

    nh.param("cam2_P0", cam_info2.msg.P[0], (double)0.0);
    nh.param("cam2_P1", cam_info2.msg.P[1], (double)0.0);
    nh.param("cam2_P2", cam_info2.msg.P[2], (double)0.0);
    nh.param("cam2_P3", cam_info2.msg.P[3], (double)0.0);
    nh.param("cam2_P4", cam_info2.msg.P[4], (double)0.0);
    nh.param("cam2_P5", cam_info2.msg.P[5], (double)0.0);
    nh.param("cam2_P6", cam_info2.msg.P[6], (double)0.0);
    nh.param("cam2_P7", cam_info2.msg.P[7], (double)0.0);
    nh.param("cam2_P8", cam_info2.msg.P[8], (double)0.0);
    nh.param("cam2_P9", cam_info2.msg.P[9], (double)0.0);
    nh.param("cam2_P10", cam_info2.msg.P[10], (double)0.0);
    nh.param("cam2_P11", cam_info2.msg.P[11], (double)0.0);

    ros::spin();
}
