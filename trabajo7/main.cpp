#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

void binarize(Mat src, Mat& dst, uint8_t u) {
  int rows, cols;
  Mat img_binarize;

  rows = src.rows;
  cols = src.cols;

  img_binarize = Mat::ones(rows, cols, CV_8UC1);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (src.at<uchar>(i, j) >= u) {
        img_binarize.at<uchar>(i, j) = 255;
      } else {
        img_binarize.at<uchar>(i, j) = 0;
      }
    }
  }

  dst = img_binarize;
}

Mat get_img_border(Mat img) {
  Mat img_blur, img_gray, img_dil, img_sobel, img_binary;
  uint8_t u;

  // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
  GaussianBlur(img, img_blur, Size(7, 7), 0, 0, BORDER_DEFAULT);
  medianBlur(img_blur, img_blur, 7);
  // imwrite("debug_blur.png", img_blur);

  cvtColor(img_blur, img_gray, COLOR_BGR2GRAY);
  // imwrite("debug_gray.png", img_gray);

  u = 255 * 0.25;
  binarize(img_gray, img_gray, u);
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

  u = 255 * 0.1;
  binarize(img_erode, img_binary, u);

  return img_binary;
}

void print_points(Mat img) {
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

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Use: ./Trabajo5 img_name.png");
    return EXIT_FAILURE;
  }

  Mat image, img_border;
  String imageName = argv[1];

  // As usual we load our source image (src)
  image = imread(imageName);  // Load an image

  // Check if image is loaded fine
  if (image.empty()) {
    printf("Error opening image: %s\n", imageName.c_str());
    return EXIT_FAILURE;
  }

  img_border = get_img_border(image);

  imwrite("img_border.png", img_border);

  print_points(img_border);

  return EXIT_SUCCESS;
}