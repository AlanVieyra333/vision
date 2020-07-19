#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

#include "image_process.hpp"
#include "utils.hpp"

using namespace std;
using namespace cv;

const char* target[] = {"CIRCULO", "TRIANGULO"};

int8_t classiier(Mat img) {
  int8_t predict = -1;
  double best_hu_moments_c[7] = {1.594433, 0, 8.499806, 0, 0, 0, 0};
  double best_hu_moments_t[7] = {1.624184, 0, 5.473052, 0, 0, 0, 0};

  get_img_border(img, img);
  // imwrite("debug_img_border.png", img_border);

  extract_largest_component(img, img);
  // imwrite("out.png", largest_component);

  double huMoments[7];
  hu_moments(img, huMoments);

  double distance_c = distance_hu_moments(huMoments, best_hu_moments_c);
  double distance_t = distance_hu_moments(huMoments, best_hu_moments_t);

  if (distance_c < distance_t) {
    predict = 0;
  } else {
    predict = 1;
  }

  // printf("distance_c: %f\n", distance_c);
  // printf("distance_t: %f\n", distance_t);

  return predict;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Use: ./Trabajo7 img_name");
    return EXIT_FAILURE;
  }

  const char* img_name = argv[1];
  Mat img = imread(img_name);  // Load an image

  // Check if image is loaded fine
  if (img.empty()) {
    printf("Error opening image: %s\n", img_name);
    return EXIT_FAILURE;
  }

  int8_t predict = classiier(img);
  if (predict != -1) {
    printf("Figura reconocida: %s\n", target[predict]);
  } else {
    printf("Figura no reconocida\n");
  }

  return EXIT_SUCCESS;
}