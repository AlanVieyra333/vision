#include <opencv2/core/mat.hpp>

void hu_moments(cv::Mat img, double huMoments[7]);
cv::Point2d extract_largest_component(cv::Mat src, cv::Mat& dst);
void get_img_border(cv::Mat src, cv::Mat& dst);
