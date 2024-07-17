// Arturo Melero Ortiz

/*
    AIBOFOBIA -------------------------------------------------------

        --> Dadas dos palabras, se debe reconstruir cualquier subsecuencia comun mas larga válida

            secuencia (i, j) = tam secuencia max formada con los primeros i caracteres de p1 y los primeros j de p2

            secuencia (i, j) = | secuencia (i-1, j-1) + 1 si p1[i] = p2[j];
                               | max(secuencia (i-1, j), secuencia (i, j-1) oc

            El coste de la solución es O(), siendo
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Implementacion descendente
void subsecuencia(const std::string& p1, const std::string& p2, std::vector<std::vector<int>>& secuencia) {
    for (int i = 1; i <= p1.size(); ++i) {
        for (int j = 1; j <= p2.size(); ++j) {
            if (p1[i - 1] == p2[j - 1]) secuencia[i][j] = 1 + secuencia[i - 1][j - 1];
            else secuencia[i][j] = std::max(secuencia[i - 1][j], secuencia[i][j - 1]);
        }
    }
}

void solucion(std::string& sol, int i, int j, const std::string& p1, const std::string& p2, std::vector<std::vector<int>>& secuencia) {
    if (i == 0 || j == 0) return;
    else if (p1[i - 1] == p2[j - 1]) {
        solucion(sol, i - 1, j - 1, p1, p2, secuencia);
        sol.push_back(p1[i - 1]);
    }
    else if (secuencia[i - 1][j] > secuencia[i][j - 1]) {
        solucion(sol, i - 1, j, p1, p2, secuencia);
    }
    else solucion(sol, i, j - 1, p1, p2, secuencia);
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // Datos de entrada
    std::string p1, p2;
    std::cin >> p1 >> p2;
    if (!std::cin)  // fin de la entrada
        return false;
    std::vector<std::vector<int>> secuencia(p1.size() + 1, std::vector<int>(p2.size() + 1, 0));

    // escribir sol
    std::string sol;
    subsecuencia(p1, p2, secuencia); solucion(sol, p1.size(), p2.size(), p1, p2, secuencia);
    std::cout << sol << "\n";

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