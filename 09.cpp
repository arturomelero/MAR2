// Arturo Melero Ortiz

/*
    EL CARPINTERO EBANISTO  -------------------------------------------------------

        --> Dado un tablon de longitud L, se desean realizar N cortes a longitudes l1,...,ln
            realizando un esfuerzo cuantificado como el doble de la longitud actual de la tabla.
            Se desea minimizar el esfuerzo. Similar al problema de matrices.

            tablon(i,j) = min_{i<k<j} {tablon[i,k] + tablon[k,j] + 2*cortes(j)-cortes(i)}

            El coste de la solución es O(n^3), siendo n el numero de cortes a realizar + 2 (inicio y fin de la tabla)
*/

#include <iostream>
#include <fstream>
#include <vector>

// Variable global para las comprobaciones. Solo se modifica en resuelveCaso
int max;

// Implementacion descendente
int cortar(int L, const std::vector<int>& cortes, std::vector<std::vector<int>>& tablon) {
    for (int d = 2; d < cortes.size(); ++d) {
        for (int i = 0; i < cortes.size() - d; ++i) {
            int j = i + d; 
            tablon[i][j] = max;
            for (int k = i + 1; k < j; ++k) {
                int temp = tablon[i][k] + tablon[k][j] + 2 * (cortes[j] - cortes[i]);
                if (temp < tablon[i][j]) { // es mejor cortar por k
                    tablon[i][j] = temp; 
                }
            }
        }
    }
    return tablon[0][cortes.size() - 1];
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // Datos de entrada
    int N, L;
    std::cin >> L >> N;
    if (L == 0 && N == 0)  // fin de la entrada
        return false;
    std::vector<int> cortes(N + 2);
    cortes[0] = 0; cortes[N + 1] = L;
    for (int i = 1; i <= N; ++i) {
        std::cin >> cortes[i];
    }
    std::vector<std::vector<int>> tablon(N+2, std::vector<int>(N+2, 0));
    max = 2 * N * L;
    std::cout << cortar(L, cortes, tablon) << "\n";

    return true;
}

int main() {
    // ajustes para que cin extraiga directamente de un fichero
#ifndef DOMJUDGE
    std::ifstream in("casos.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

    while (resuelveCaso());

    // para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif
    return 0;
}