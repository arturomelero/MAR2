
// Arturo Melero Ortiz

/*
    JUEGO DE TABLERO -------------------------------------------------------

        --> Para un tablero NxN, se desea obtener el número máximo obtenible 
            recorriendo el tablero y la columna desde la cual empezar en la 
            última fila. --> Construir de arriba a abajo la solución.

            El coste de la solución es O(n^2), con n la dimensión del tablero
*/

#include <iostream>
#include <fstream>
#include <vector>
#include "EnterosInf.h"


std::pair<int, int> resolver(const std::vector<std::vector<int>>& tablero) {
    unsigned int n = tablero[0].size();
    std::vector<std::vector<int>> juego(n, std::vector<int>(n, 0));
    // Inicializamos sumas de la última fila
    for (int j = 0; j < n; ++j)
        juego[0][j] = tablero[0][j];
    // Calculamos la sumas máximas de la fila N-2 hacia arriba
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            juego[i][j] = tablero[i][j] + juego[i - 1][j];
            if (j > 0) 
                juego[i][j] = std::max(juego[i][j], tablero[i][j] + juego[i - 1][j - 1]);
            if (j < n - 1) 
                juego[i][j] = std::max(juego[i][j], tablero[i][j] + juego[i - 1][j + 1]);
            
        }
    }
    // Encontrar la celda de la última fila desde donde hay que comenzar el recorrido
    int max_valor = juego[n-1][0];
    int columna_inicio = 1;
    for (int j = 1; j < n; ++j) {
        if (juego[n-1][j] > max_valor) {
            max_valor = juego[n-1][j];
            columna_inicio = j + 1;
        }
    }

    std::pair<int, int> sol = { max_valor, columna_inicio };
    return sol;
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {

    // leer los datos de la entrada
    int n;
    std::cin >> n;
    if (!std::cin)  // fin de la entrada
        return false;
    std::vector<std::vector<int>> tablero(n, std::vector<int> (n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            std::cin >> tablero[i][j];
    }

    std::pair<int, int> sol = resolver(tablero);

    // escribir sol
    std::cout << sol.first << " " << sol.second << "\n";

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
