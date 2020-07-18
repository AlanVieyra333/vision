#include "utils.hpp"

#include <math.h>

double standard_deviation(double samples[], size_t n) {
  return sqrt(variance(samples, n));
}

double variance(double samples[], size_t n) {
  double variance = 0;
  double t = samples[0];

  for (int i = 1; i < n; i++) {
    t += samples[i];
    double diff = ((i + 1) * samples[i]) - t;
    variance += (diff * diff) / ((i + 1.0) * i);
  }

  return variance / (n - 1);
}