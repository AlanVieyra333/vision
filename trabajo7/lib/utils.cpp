#include "utils.hpp"

#include <inttypes.h>
#include <math.h>

#include <numeric>
#include <vector>

double distance_hu_moments(double huMoments_a[7], double huMoments_b[7]) {
  double distance = 0;

  for (uint8_t i = 0; i < 7; i++) {
    if (huMoments_a[i] != 0 && huMoments_b[i] != 0) {
      distance += (huMoments_a[i] - huMoments_b[i]) * (huMoments_a[i] - huMoments_b[i]);
    }
  }

  return sqrt(distance);
}

double standard_deviation(double samples[], size_t n) {
  return sqrt(variance(samples, n));
}

double variance(double samples[], size_t n) {
  double variance = 0;
  double t = samples[0];

  for (size_t i = 1; i < n; i++) {
    t += samples[i];
    double diff = ((i + 1) * samples[i]) - t;
    variance += (diff * diff) / ((i + 1.0) * i);
  }

  return variance / (n - 1);
}

double mean(double numbers[], size_t len) {
  double sum = 0;
  for (size_t i = 0; i < len; i++) {
    sum += numbers[i];
  }

  return sum / len;
}