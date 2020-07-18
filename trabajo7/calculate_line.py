import numpy as np
from numpy import genfromtxt
import matplotlib.pyplot as plt

coords = genfromtxt('coords.csv', delimiter=',')
# print(coords.shape)


def calc_line(coords):
    n = coords.shape[0]
    suma_xi = 0
    suma_xi_2 = 0
    suma_yi = 0
    suma_xi_yi = 0

    for i in range(n):
        suma_xi += coords[i, 0]
        suma_xi_2 += coords[i, 0] * coords[i, 0]
        suma_yi += coords[i, 1]
        suma_xi_yi += coords[i, 0] * coords[i, 1]

    #   Delta   = | suma(xi^2)  suma(xi) |
    #             | suma(xi)       n     |
    #   Delta m = | suma(xi yi)   suma(xi)|
    #             | suma(yi)         n    |
    #   Delta b = | suma(xi^2)  suma(xi yi) |
    #             | suma(xi)     suma(yi)   |

    Delta = np.linalg.det([[suma_xi_2, suma_xi],
                           [suma_xi, n]])

    Delta_m = np.linalg.det([[suma_xi_yi, suma_xi],
                             [suma_yi, n]])

    Delta_b = np.linalg.det([[suma_xi_2, suma_xi_yi],
                             [suma_xi, suma_yi]])

    #   m = Delta m /Delta
    #   b = Delta b /Delta

    m = Delta_m / Delta
    b = Delta_b / Delta

    return m, b


m, b = calc_line(coords)
print("y = xm + b\n", "f(x) = x*", m, "+", b)


def f(x):
    return x*m + b


x = np.arange(0, 640, 1.0)

plt.plot(x, f(x))
plt.xlabel('x')
plt.ylabel('f(x)')
plt.title('Linea de borde')
plt.axis([0, 640, 0, 480])
plt.grid(True)
plt.show()
