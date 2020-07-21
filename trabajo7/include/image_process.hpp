#include <opencv2/core/mat.hpp>

void draw_border(cv::Mat img, cv::Mat border);
void hu_moments(cv::Mat img, double huMoments[7]);
cv::Point2d extract_largest_component(cv::Mat src, cv::Mat& dst);
void get_img_border(cv::Mat src, cv::Mat& dst);
void print_points_img_bin(cv::Mat img);
