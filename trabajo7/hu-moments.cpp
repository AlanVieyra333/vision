#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

#include "image_process.hpp"
#include "utils.hpp"

using namespace std;
using namespace cv;

void get_best_hu_moments(String img_folder_name, double best_hu_moments[7],
                         uint8_t best_hu_moments_len) {
  Mat img, img_border, largest_component;
  vector<String> img_names;
  double huMoments[7];

  glob(img_folder_name + "/*.png", img_names, false);
  int img_len = img_names.size();

  double all_huMoments[7][img_len];

  for (int i = 0; i < img_len; i++) {
    img = imread(img_names[i]);  // Load an image

    // Check if image is loaded fine
    if (img.empty()) {
      printf("Error opening image: %s\n", img_names[i].c_str());
      exit(EXIT_FAILURE);
    }

    get_img_border(img, img_border);
    // imwrite("debug_img_border.png", img_border);

    extract_largest_component(img_border, largest_component);
    // imwrite("out.png", largest_component);

    hu_moments(largest_component, huMoments);

    // Agregar los momentos de hu de la imagen actual.
    for (uint8_t j = 0; j < 7; j++) {
      all_huMoments[j][i] = huMoments[j];
      // printf("huMoment[%d]: %f\n", (j+1), huMoments[j]);
    }
  }

  // Calcular la desviacion estandar de los momentos de hu.
  vector<double> std_huMoments;
  double aux_std_huMoments[7];

  for (int i = 0; i < 7; i++) {
    double std = standard_deviation(all_huMoments[i], img_len);
    std_huMoments.push_back(std);
    best_hu_moments[i] = 0;
    aux_std_huMoments[i] = std;
    printf("std [%d]: %f\n",  (i+1), std);
  }
  sort(std_huMoments.begin(), std_huMoments.end());

  // Calcular la media de los n momentos de hu con desviasion estandar mas
  // baja.
  for (uint8_t i = 0; i < best_hu_moments_len; i++) {
    for (uint8_t j = 0; j < 7; j++) {
      if (std_huMoments[i] == aux_std_huMoments[j]) {
        best_hu_moments[j] = mean(all_huMoments[j], img_len);
        break;
      }
    }
  }
}

int main(int argc, char** argv) {
  if (argc != 3) {
    printf("Use: ./hu-moments.o ./folder_name num_best_hu_moments\n");
    return EXIT_FAILURE;
  }

  const char* folder_name = argv[1];
  uint8_t hu_moments_len = atoi(argv[2]);

  double best_hu_moments[7];
  get_best_hu_moments(folder_name, best_hu_moments, hu_moments_len);
  for (uint8_t i = 0; i < 7; i++) {
    printf("%f\n", best_hu_moments[i]);
  }

  return EXIT_SUCCESS;
}