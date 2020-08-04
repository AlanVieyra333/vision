#include <opencv2/core/mat.hpp>

const char* target[] = {"CIRCULO", "TRIANGULO"};

int8_t img_classiier(cv::Mat img);
bool marker_recognition(cv::Mat _img, double points[5][2]);
