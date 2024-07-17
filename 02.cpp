
// Arturo Melero Ortiz

/*
    CONSTRUYENDO DIANAS -------------------------------------------------------

        --> Para una puntuación n y s secciones de puntuaciones s1, ..., sn, se 
            desea obtener el menor numero de dardos que hay que lanzar para obtener
            la puntuación y la lista de secciones que se debe obtener.

            El coste de la solución es O(n*s), coste espacial O(n*s).
*/

#include <iostream>
#include <fstream>
#include <vector>
#include "EnterosInf.h"

struct tSol {
    EntInf minDardos;
    std::vector<EntInf> puntuaciones;
};

tSol resolver(int n, std::vector<int> secciones) {
    std::vector<std::vector<EntInf>> dianas(secciones.size() + 1, std::vector<EntInf>(n + 1, Infinito));
    dianas[0][0] = 0;
    for (int i = 1; i <= secciones.size(); ++i) {
        dianas[i][0] = 0;
        for (int j = 1; j <= n; ++j) {
            if (secciones[i - 1] > j) dianas[i][j] = dianas[i - 1][j];
            else dianas[i][j] = std::min(dianas[i - 1][j], dianas[i][j - secciones[i - 1]] + 1);
        }
    }
    tSol sol;
    sol.minDardos = dianas[secciones.size()][n];
    if (sol.minDardos != Infinito) {
        int i = secciones.size(), j = n;
        while (j > 0) {
            if (secciones[i - 1] <= j && dianas[i][j] != dianas[i - 1][j]) {
                sol.puntuaciones.push_back(secciones[i - 1]);
                j = j - secciones[i - 1];
            }
            else --i;
        }
    }

    return sol;
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {

    // leer los datos de la entrada
    int N, S;
    std::cin >> N >> S;
    if (!std::cin)  // fin de la entrada
        return false;

    std::vector<int> secciones(S);
    for (int i = 0; i < S; ++i) std::cin >> secciones[i];

    tSol sol = resolver(N, secciones);

    // escribir sol
    if (sol.minDardos == Infinito) std::cout << "Imposible\n";
    else {
        std::cout << sol.minDardos << ":";
        int i = 0;
        while ((EntInf(i)) != sol.minDardos) {
            std::cout << " " << sol.puntuaciones[i];
            ++i;
        }
        std::cout << "\n";
    }

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
