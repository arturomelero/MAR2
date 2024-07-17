#include<iostream>
#include"Matriz.h"
using namespace std;

// Daixiang Chen y Arturo Melero Ortiz

//Plantea y explica la recurrencia
	// Partiendo de la casilla final, recorremos la matriz de abajo a arriba y de derecha a izquierda, sumando en cada momento el número de formas de 
	// acceder a la final desde cualquiera de las casillas accesibles
//Definicion
	// salto(i,j) = número de formas de saltar de casilla (i,j) a casilla (n,m)
//Llamada inicial
	// salto(N,M)
//Caso/s base
	// salto(N,M) = 1
//Casos recursivos: La recurrencia suma las formas que hay de llegar desde cualquiera de las dos casillas a las que se pueda acceder desde la actual

	
// La complejidad tiene coste O(n*m), pues recorremos la matriz exactamente una vez
long long int numCaminos(Matriz<int> const& tablero, int N, int M) {
	Matriz<int> pd(N, M, 0);
	pd[N - 1][M - 1] = 1;
	for (int i = N-1; i >= 0; --i) {
		for (int j = M-1; j >= 0; --j) {
			if (i + tablero[i][j] < N && j + tablero[i][j] < M) {
				pd[i][j] = pd[i + tablero[i][j]][j] + pd[i][j + tablero[i][j]];
			}
			// Puedo ir solo abajo 
			else if (i + tablero[i][j] < N) {
				pd[i][j] = pd[i + tablero[i][j]][j];
			}
			// Puedo ir solo derecha
			else if (j + tablero[i][j] < M) {
				pd[i][j] = pd[i][j + tablero[i][j]];
			}
		}
	}
	return pd[0][0];
}

bool resuelveCaso() {
	int N, M;
	cin >> N >> M;
	if (!cin) {
		return false;
	}
	else {
		Matriz<int> tablero(N, M);
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < M; j++)
				cin >> tablero[i][j];
		}

		cout << numCaminos(tablero, N, M) << '\n';
		return true;
	}
}


int main() {
	while (resuelveCaso()) {
	}
	return 0;
}