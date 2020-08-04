
/*
 * calcula las funciones necesarias para calcular las matrices 
 * K  R y t que del modelo de la cámara obscura, dadas las esquinas de 
 * un cuadrado, indicadas en una imágen.
 * */
#include <math.h>
#include "homography/homografia.h"
#include "homography/eigen.h"
#include "homography/matrices.h"

/*
 * Normaliza los datos de entrada
 * */
void normalizar( double S[][2], double E[][2], double vd[], int n )
{
  double sum1, sum2, prom, var, sig;
  int i;

  // normalizamos x
  sum1 = 0.0;
  sum2 = 0.0;
  i = 0;
  while (i < n){
    sum1 += E[i][0];
    sum2 += E[i][0]*E[i][0];
    i++;
  }
  prom = sum1/n;
  var = sum2/n - prom*prom;
  sig = sqrt(var);
  vd[0] = prom;
  vd[1] = sig;

  i = 0;
  while (i < n){
    S[i][0] = (E[i][0]-prom)/sig;
    i++;
  }

  // normalizamos y
  sum1 = 0.0;
  sum2 = 0.0;
  i = 0;
  while (i < n){
    sum1 += E[i][1];
    sum2 += E[i][1]*E[i][1];
    i++;
  }

  prom = sum1/n;
  var = sum2/n - prom*prom;
  sig = sqrt(var);
  vd[2] = prom;
  vd[3] = sig;

  i = 0;
  while (i < n){
    S[i][1] = (E[i][1]-prom)/sig;
    i++;
  }
}

/*
 * Calcula la matriz de rotación haciendo la descomposición 
 *  SVD de la matríz de entrada (a través de la eigendescomposición)
 * */
void CalculaR( double A[][3], double R[][3]  )
{
    int i , j;
    double S[3][3];
	double U[3][3], V[3][3];
    double roots[3];

    multiplicaATB( A, A, S );
    dsyevv3( S, V, roots );
    for( i=0; i<3; i++ ) 
		roots[i] = 1.0/sqrt(roots[i]);

    multiplicaAB( A, V, U );
	for( i=0; i<3; i++ ) 
		for( j=0; j<3; j++ ) 
			U[i][j] *= roots[j]; 

    multiplicaABT( U, V, R );
}



/*
 * Calcula la matríz R y t dada una matríz H
 * */
int parametros_de_H( double H[3][3], double MK[16], double MRT[16], double Hi, double Wi)
{
    double u0,v0,w33,f2,f;
    double l1, l2 ,l;
    double A[3][3]; 
    double A2[3][3];
    double Rp[3][3];
    double R[3][3];
	
    double RT[3][3];  
    double RN[3][3];      
	double Kinv[3][3];	
    double vt[3], vc[3];
	double theta1, theta2, theta3;

	int i;

    
    u0 = -Wi/2.0;
    v0 = -Hi/2.0;
    //printf("u0 = %f, v0 = %f\n", u0, v0);
    
    w33 = (  -H[0][1]*( H[0][0] + H[2][0]*u0 )
                - H[1][1]*( H[1][0] + H[2][0]*v0 )
                - H[2][1]*H[0][0]*u0
                - H[2][1]*H[1][0]*v0  )/(H[2][1]*H[2][0]);

    f2 = w33 - u0*u0 - v0*v0;

    f = 0;
    if (f2 <  0){
        return -1;
    } else{
        f = sqrt( f2 );
    }

    // double K[3][3] ={{f,0.0,u0},{0.0,f,v0},{0.0,0.0,-1.0}};
    // Kinv = numpy.matrix( [[1.0/f, 0.0, u0/f], [0.0, 1.0/f, v0/f], [0.0, 0.0, -1.0]] )
	Kinv[0][0] = 1.0/f; Kinv[0][1] = 0.0;   Kinv[0][2] = u0/f;
	Kinv[1][0] = 0.0;   Kinv[1][1] = 1.0/f; Kinv[1][2] = v0/f;
	Kinv[2][0] = 0.0;   Kinv[2][1] = 0.0;   Kinv[2][2] = -1.0;

    multiplicaAB(Kinv, H, A);
    
    l1 = Norma(A[0][0], A[1][0], A[2][0]);
    l2 = Norma(A[0][1], A[1][1], A[2][1]);
    l = (l1+l2)/2.0;

    Mult_Escalar((1.0/l),A,A2);
    vt[0] = A2[0][2];
    vt[1] = A2[1][2];
    vt[2] = A2[2][2];
    CopiaMatriz(A2,Rp);
    
    Rp[0][2] = Rp[1][0]*Rp[2][1] - Rp[1][1]*Rp[2][0];
    Rp[1][2] = Rp[2][0]*Rp[0][1] - Rp[2][1]*Rp[0][0];
    Rp[2][2] = Rp[0][0]*Rp[1][1] - Rp[0][1]*Rp[1][0];
    
    CalculaR(Rp, R);
	// Cálculo de theta3, theta2, theta1
	theta1 = atan2( R[2][1], -R[2][0] );
	theta1 *= 180.0/M_PI;

	theta2 = acos( R[2][2] );
	theta2 *= 180.0/M_PI;

	theta3 = atan2( R[1][2], R[0][2] );
	theta3 *= 180.0/M_PI;

	// printf( "(theta3, theta2, theta1) = %lf, %lf, %lf\n", theta3, theta2, theta1 );

    TranspuestaMatriz(R,RT);
    Mult_Escalar(-1, RT, RN);
    Mult_Mat_Vector(RN, vt, vc);
    
    //Inicialización
    for (i = 0; i < 16; i++){
        MK[i] = 0.0;
        MRT[i] = 0.0;
    }
    
    
    MK[0]  = f;
    MK[5]  = f;
    MK[8]  = u0;
    MK[9]  = v0;
    MK[10] = 51.0; //**********************************************************//
    MK[11] = -1;
    MK[14] = 20.0; //**********************************************************// 
    
    
    MRT[0 ] = R[0][0]; MRT[1 ] = R[1][0]; MRT[2 ] = R[2][0]; MRT[3 ] = 0.0 ;
    MRT[4 ] = R[0][1]; MRT[5 ] = R[1][1]; MRT[6 ] = R[2][1]; MRT[7 ] = 0.0;
    MRT[8 ] = R[0][2]; MRT[9 ] = R[1][2]; MRT[10] = R[2][2]; MRT[11] = 0.0;
    MRT[12] = vt[0];   MRT[13] = vt[1];   MRT[14] = vt[2];   MRT[15] = 1.0 ;
    return 1;
}

/*
 * Calcula la matríz H (homografía) dado un conjunto de 4 
 * puntos de entrada, teniendo como salida la matriz K, R y t.
 * */
int homografia(double P[][2], double p[][2], double MK[16], double MRT[16],  double height , double width)
{
	#define N 5
    int i,j;
    
    /*Valores en el modelo */
    double pnor[N][2];
    double Pnor[N][2];
    double vdp[N];
    double vdP[N];
    double R[10][9] ;
    double A[10][9] = {{0}};
    double suma;
    double vh[10] = {0};
    double Hp[3][3]= { {0} };
    double H[3][3]= { {0} };
    double mx, s_x, my, s_y;
    double mu, s_u,mv, s_v;

    normalizar(pnor, p, vdp, N);
    normalizar(Pnor, P, vdP, N);
    i = 0;
    
    while (i < N){
        j = 2*i;
        A[j][0] = Pnor[i][0];
        A[j][1] = Pnor[i][1];
        A[j][2] = 1.0;
        A[j][6] = -pnor[i][0]*Pnor[i][0];
        A[j][7] = -pnor[i][0]*Pnor[i][1];
        A[j][8] = pnor[i][0];

        j++;
        A[j][3] = Pnor[i][0];
        A[j][4] = Pnor[i][1];
        A[j][5] = 1.0;
        A[j][6] = -pnor[i][1]*Pnor[i][0];
        A[j][7] = -pnor[i][1]*Pnor[i][1];
        A[j][8] = pnor[i][1];

        i++;
    }

	// Tengo que hacer la descomposición QR como en el paper: 
    QR_noQ( A, R );
  
	/* Backsustitution */ 
    vh[7] = R[7][8]/R[7][7];

    for( i=6; i>=0; i-- ) {
        suma = 0.0;
        for( j=i+1; j<8; j++ ) 
            suma += R[i][j] * vh[j];
        
        vh[i] = ( R[i][8] - suma )/R[i][i];
    }

    //Aquí se tiene H normalizada 
    
    Hp[0][0] = vh[0];
    Hp[0][1] = vh[1];
    Hp[0][2] = vh[2];
    Hp[1][0] = vh[3];
    Hp[1][1] = vh[4];
    Hp[1][2] = vh[5];
    Hp[2][0] = vh[6];
    Hp[2][1] = vh[7];
    Hp[2][2] = 1.0;

    mx = vdP[0];
    s_x = vdP[1];
    my = vdP[2];
    s_y = vdP[3];
    i=0;

    while (i < 3){
        H[i][0] = Hp[i][0]/s_x;
        H[i][1] = Hp[i][1]/s_y;
        H[i][2] = Hp[i][2] - mx*Hp[i][0]/s_x - my*Hp[i][1]/s_y;
        i ++;
    }

    mu  = vdp[0];
    s_u = vdp[1];
    mv  = vdp[2];
    s_v = vdp[3];
    Hp[0][0] = (H[0][0] * s_u) + (mu * H[2][0]);
    Hp[1][0] = (H[1][0] * s_v) + (mv * H[2][0]);

    Hp[0][1] = (H[0][1] * s_u) + (mu * H[2][1]);
    Hp[1][1] = (H[1][1] * s_v) + (mv * H[2][1]);

    Hp[0][2] = (H[0][2] * s_u) + (mu * H[2][2]);
    Hp[1][2] = (H[1][2] * s_v) + (mv * H[2][2]);
    
    
    i = 0;
    while (i < 3){
        H[0][i] = Hp[0][i];
        H[1][i] = Hp[1][i];
        i ++;
    }
    
    i = parametros_de_H(H, MK, MRT, height, width);

	return i;
}
