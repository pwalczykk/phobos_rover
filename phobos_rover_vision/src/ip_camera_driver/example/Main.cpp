#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>

int main(int argc, char** argv){
    cv::VideoCapture cap;
    cv::Mat frame;

    // const std::string stream_adress ="http://admin:admin@10.42.0.95/tmpfs/auto.jpg";
    const std::string stream_adress ="rtsp://192.168.1.10:554/user=admin&password=&channel=1&stream=0.sdp?real_stream--rtp-caching=100";
    if(!argc == 2){
        exit(-1);
    }
    // const std::string stream_adress = "http://admin:admin@" + std::string(argv[1]) + "/tmpfs/auto.jpg";



    cv::namedWindow("Win", 1);

    int i = 0;
    while(1){
        if(!cap.open(stream_adress)){
            std::cout << stream_adress << std::endl;
            std::cout << "Cant open stream!" << std::endl;
            exit(-1);
        }
        cap.read(frame);
        cv::imshow("Win", frame);
        cv::waitKey(10);
    }

    return 0;
}
