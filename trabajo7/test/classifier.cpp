#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

#include "image_process.hpp"
#include "image_classifier.hpp"

using namespace cv;

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Use: ./Trabajo7 img_name\n");
    return EXIT_FAILURE;
  }

  const char* img_name = argv[1];
  Mat img = imread(img_name);  // Load an image

  // Check if image is loaded fine
  if (img.empty()) {
    printf("Error opening image: %s\n", img_name);
    return EXIT_FAILURE;
  }

  int8_t predict = img_classiier(img);
  if (predict != -1) {
    printf("Figura reconocida: %s\n", target[predict]);
  } else {
    printf("Figura no reconocida\n");
  }

  return EXIT_SUCCESS;
}