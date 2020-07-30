#include <opencv2/core/mat.hpp>

#include "triangles.h"

const char* target[] = {"CIRCULO", "TRIANGULO"};

int8_t img_classiier(cv::Mat img);
bool marker_recognition(cv::Mat _img, DPOINT points[5]);
