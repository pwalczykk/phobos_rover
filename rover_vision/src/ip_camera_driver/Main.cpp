#include "CameraHandler.hpp"
#include "ImagePublisher.hpp"

int main(int argc, char** argv){

    std::string stream_addres = argv[argc-2];
    std::string publisher_topic = argv[argc-1];

    ros::init(argc, argv, "aqqq");
    ros::NodeHandle nh;
    ros::Rate loop_rate(10);

    ImagePublisher cam1(stream_addres, publisher_topic);

    cam1.CameraHandler::Init();

    // cam1.CameraHandler::Set(320, 240, 30);

    while(ros::ok()){
        cam1.CameraHandler::Init();
        cam1.PublishImage();
        loop_rate.sleep();
    }

    cam1.CameraHandler::Release();

    return 0;
}
