#include "CameraHandler.hpp"
#include "ImagePublisher.hpp"

int main(int argc, char** argv){

    int cam_num1 = atoi(argv[1]);
    int cam_num2 = atoi(argv[2]);

    if(!(cam_num1 >= 0 && cam_num1 < 9)){
        ROS_ERROR("Bad camera1 number");
        exit(-1);
    }

    if(!(cam_num2 >= 0 && cam_num2 < 9)){
        ROS_ERROR("Bad camera2 number");
        exit(-1);
    }

    ros::init(argc, argv, "titanum_camera_driver");
    ros::NodeHandle nh;
    ros::Rate loop_rate(30);

    ImagePublisher cam1(cam_num1);
    ImagePublisher cam2(cam_num2);

    cam1.CameraHandler::Init();
    cam1.CameraHandler::Init();


    cam1.CameraHandler::Set(320, 240, 30);
    cam2.CameraHandler::Set(320, 240, 30);

    while(ros::ok()){
        cam1.CameraHandler::Init();
        cam1.PublishImage();
        cam1.CameraHandler::Release();

        cam2.CameraHandler::Init();
        cam2.PublishImage();
        cam2.CameraHandler::Release();

        loop_rate.sleep();
    }

    return 0;
}
