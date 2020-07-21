#include "image_process.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

void draw_border(Mat img, Mat border) {
  for (size_t r = 0; r < img.rows; r++) {
    for (size_t c = 0; c < img.cols; c++) {
      if (border.at<uchar>(r, c) == 255) {
        img.at<uchar>(r, c) = 255;
      }
    }
  }
}

void hu_moments(Mat img, double huMoments[7]) {
  // Calculate Moments
  Moments _moments = moments(img, false);

  // Calculate Hu Moments
  HuMoments(_moments, huMoments);

  // Log scale hu moments
  for (int i = 0; i < 7; i++) {
    huMoments[i] = -1 * copysign(1.0, huMoments[i]) * log10(abs(huMoments[i]));
    // printf("Hu moment [%d]: %f\n", (1 + 1), huMoments[i]);
  }
}

/**
 * Funcion que extrae el componente mas grande de una imagen binaria
 * y devuelve su centroide.
 */
Point2d extract_largest_component(Mat src, Mat& dst) {
  Mat labelImage(src.size(), CV_32S);
  Mat stats, centroids;
  int nLabels =
      connectedComponentsWithStats(src, labelImage, stats, centroids, 8);
  Mat img_largest_component = Mat::zeros(src.rows, src.cols, CV_8UC1);
  int max_area = 0, max_label = 0, area;

  // Obtener la etiqueta del componente con area mayor.
  for (int i = 1; i < nLabels; i++) {
    area = stats.at<int>(i, CC_STAT_AREA);

    if (area > max_area) {
      max_area = area;
      max_label = i;
    }
  }

  // Mostrar solo el componente con area mayor.
  if (max_label != 0) {
    for (int r = 0; r < labelImage.rows; ++r) {
      for (int c = 0; c < labelImage.cols; ++c) {
        int label = labelImage.at<int>(r, c);

        if (label == max_label) {
          img_largest_component.at<uchar>(r, c) = 255;
        }
      }
    }
  }

  // imwrite("debug_largest_comp.png", src);
  dst = img_largest_component;

  return centroids.at<Point2d>(max_label);
}

/**
 * Funcion que obtiene los bordes de una imagen.
 */
void get_img_border(Mat src, Mat& dst) {
  Mat img_blur, img_gray, img_dil, img_sobel;
  uint8_t u;

  // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
  GaussianBlur(src, img_blur, Size(7, 7), 0, 0, BORDER_DEFAULT);
  medianBlur(img_blur, img_blur, 7);
  // imwrite("debug_blur.png", img_blur);

  // printf("channels: %d\n", src.channels());
  if (src.channels() > 1) {
    cvtColor(img_blur, img_gray, COLOR_BGR2GRAY);
    // imwrite("debug_gray.png", img_gray);
  } else {
    img_gray = img_blur;
  }

  // binarize
  u = 255 * 0.25;
  threshold(img_gray, img_gray, u, 255, THRESH_BINARY);
  // imwrite("debug_gray_bin.png", img_gray);

  Mat elementD = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
  dilate(img_gray, img_dil, elementD, Point(-1, -1), 1);
  // imwrite("debug_dilate.png", img_dil);

  // Sobel
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;
  int ddepth = CV_16S;  // CV_16S;
  int ksize = 5;

  Sobel(img_dil, grad_x, ddepth, 1, 0, ksize, 1, 0, BORDER_DEFAULT);
  convertScaleAbs(grad_x, abs_grad_x);
  Sobel(img_dil, grad_y, ddepth, 0, 1, ksize, 1, 0, BORDER_DEFAULT);
  convertScaleAbs(grad_y, abs_grad_y);
  addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, img_sobel);

  // imwrite("imSobel_x.png", img_sobel_x);
  // imwrite("imSobel_y.png", img_sobel_y);
  // imwrite("debug_sobel.png", img_sobel);

  // Morph shapes : MORPH_RECT, MORPH_CROSS, MORPH_ELLIPSE
  Mat img_erode;
  Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
  erode(img_sobel, img_erode, element, Point(-1, -1), 1);
  // erode(img_sobel, img_erode, Mat(), Point(0, 0), ksize * 2);
  // imwrite("debug_erode.png", img_erode);

  // binarize
  u = 255 * 0.1;
  threshold(img_erode, dst, u, 255, THRESH_BINARY);
}

void print_points_img_bin(Mat img) {
  int rows, cols;
  FILE* file = fopen("coords.csv", "w");

  rows = img.rows;
  cols = img.cols;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (img.at<uchar>(i, j) > 0) {
        fprintf(file, "%d,%d\n", j, rows - i);
      }
    }
  }

  fclose(file);
}
