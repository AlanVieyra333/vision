
/*
 * calcula las funciones necesarias para calcular las matrices
 * K  R y t que del modelo de la cámara obscura, dadas las esquinas de
 * un cuadrado, indicadas en una imágen.
 * */

void normalizar(double S[][2], double E[][2], double vd[], int n);
void CalculaR(double A[][3], double R[][3]);
int parametros_de_H(double H[3][3], double MK[16], double MRT[16], double Hi,
                    double Wi);
int homografia(double P[][2], double p[][2], double MK[16], double MRT[16],
               double height, double width);
