#include <opencv2/opencv.hpp>
#include <iostream>

int main() 
{
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;
    cv::Mat img(400, 400, CV_8UC3, cv::Scalar(0, 0, 255));
    cv::imshow("Test", img);
    cv::waitKey(0);
    return 0;
}