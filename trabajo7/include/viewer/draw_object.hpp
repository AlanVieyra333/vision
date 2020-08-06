#include <stddef.h>

void set_homography(double Points[5][2], size_t rows, size_t cols);
void draw_square();
void draw_triangle(float rotate_y);
void animation_tetraedro(float angle);
void init_tetraedro(float v[][3], float l);
void draw_tetraedro(float Lv[][3]);
