#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

#include "image_process.hpp"
#include "utils.hpp"

using namespace cv;

int8_t img_classiier(Mat img) {
  Mat img_border;
  int8_t predict = -1;
  double best_hu_moments_c[7] = {1.594433, 0, 8.499806, 0, 0, 0, 0};
  double best_hu_moments_t[7] = {1.624184, 3.829316, 5.473052, 7.161877, 0, 0, 0};

  get_img_border(img, img_border);
  // imwrite("debug_img_border.png", img_border);

  extract_largest_component(img_border, img_border);
  // imwrite("out.png", largest_component);

  double huMoments[7];
  hu_moments(img_border, huMoments);

  double distance_c = distance_hu_moments(huMoments, best_hu_moments_c);
  double distance_t = distance_hu_moments(huMoments, best_hu_moments_t);

  printf("Distancias a los momentos de hu: %f y %f\n", distance_c, distance_t);

  if (distance_c <= 0.84 || distance_t <= 1.48) {
    draw_border(img, img_border);

    if (distance_c < distance_t) {
      predict = 0;
    } else {
      predict = 1;
    }
  }

  return predict;
}
