#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

#include "image_process.hpp"
#include "marker/reconoce.h"
#include "opencv2/imgcodecs.hpp"
#include "utils.hpp"

using namespace cv;

int8_t img_classiier(Mat img) {
  Mat img_border;
  int8_t predict = -1;
  double best_hu_moments_c[7] = {1.594433, 0, 8.499806, 0, 0, 0, 0};
  double best_hu_moments_t[7] = {1.624184, 3.829316, 5.473052, 7.161877,
                                 0,        0,        0};

  get_img_border(img, img_border);
  // imwrite("debug_img_border.png", img_border);

  extract_largest_component(img_border, img_border);
  // imwrite("debug_largest_component.png", img_border);

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

bool marker_recognition(Mat _img, double points[5][2]) {
  Mat Image;

  GaussianBlur(_img, Image, Size(7, 7), 1.5, 1.5);  // Smooth filter
  threshold(Image, Image, 105, 255, THRESH_BINARY);

  bool marker_found = false;
  int v[4];
  uchar *pout;
  int *pin;
  Mat Image2 = Mat::zeros(Image.rows, Image.cols, CV_8UC1);

  Mat Etiquetas, Estadisticas, Centroides;

  Image = ~Image;

  /** Los objetos deben ser blancos **/
  int n = connectedComponentsWithStats(Image, Etiquetas, Estadisticas,
                                       Centroides, 8, CV_32S);
  // printf( "n = %d\n", n );

  /** Para todas las componentes conectadas **/
  for (int i = 1; i < n && !marker_found; i++) {
    if (Estadisticas.at<int>(i, CC_STAT_AREA) < 50) continue;

    v[0] = Estadisticas.at<int>(i, CC_STAT_LEFT) - 1;
    if (v[0] < 0) v[0] = 0;
    v[1] = Estadisticas.at<int>(i, CC_STAT_TOP) - 1;
    if (v[1] < 0) v[1] = 0;
    v[2] = v[0] + Estadisticas.at<int>(i, CC_STAT_WIDTH) + 1;
    if (v[2] > Image.cols - 1) v[2] = Image.cols - 1;
    v[3] = v[1] + Estadisticas.at<int>(i, CC_STAT_HEIGHT) + 1;
    if (v[3] > Image.rows - 1) v[3] = Image.rows - 1;

    if (v[0] == 0 || v[1] == 0 || v[2] == Image.cols - 1 ||
        v[3] == Image.rows - 1)
      continue;

    for (int j = v[1]; j <= v[3]; j++) {
      pin = Etiquetas.ptr<int>(j) + v[0];
      pout = Image2.ptr<uchar>(j) + v[0];
      for (int k = v[0]; k <= v[2]; k++) {
        if (*pin == i) {
          *pout = 255;
        }
        pin++;
        pout++;
      }
    }

    if (reconoce(Image2, i, v, points) == 0) {
      marker_found = true;
    }

    for (int j = v[1]; j <= v[3]; j++) {
      pout = Image2.ptr<uchar>(j) + v[0];
      for (int k = v[0]; k <= v[2]; k++) {
        *pout++ = 0;
      }
    }
  }

  return marker_found;
}