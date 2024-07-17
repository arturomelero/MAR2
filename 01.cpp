// Arturo Melero Ortiz

/*
    LA CUERDA DE LA COMETA -------------------------------------------------------

        --> Formar una cuerda de longitud L con n cordeles con coste y longitud
            de manera que se devuelva si es posible formar la cuerda, el nº de
            combinaciones posibles, min num de cordeles y min coste.

            El coste de la solución es O(n*l), coste espacial O(l).
*/

#include <iostream>
#include <fstream>
#include <vector>
#include "EnterosInf.h"

struct Cordel {
    int longitud;
    int precio;
};

struct tSol {
    long long numComb = 0;
    EntInf minCord = Infinito;
    EntInf minCost = Infinito;
};

/* ---- Ya no hace falta esta función ----
bool esPosible(const std::vector<Cordel> &cordeles, int l, int n) {
    std::vector<bool> cuerdas(l + 1, false);
    cuerdas[0] = true;
    for (int i = 0; i < n; ++i) {
        for (int j = l; j >= cordeles[i].longitud; --j) {
            cuerdas[j] = cuerdas[j] || cuerdas[j - cordeles[i].longitud];
        }
    }
    return cuerdas[l];
}
*/

tSol preguntas(const std::vector<Cordel>& cordeles, int l, int n) {
    std::vector<long long> numComb(l + 1, 0);
    std::vector<EntInf> minCord(l + 1, Infinito), minCost(l + 1, Infinito);
    numComb[0] = 1; // Hay una única forma de formar una cuerda de longitud 0: empleando 0 cordeles
    minCord[0] = 0; // Num minimo para cuerda de longitud 0 es 0 cordeles
    minCost[0] = 0; // Coste 0 para una cuerda de longitud 0
    for (int i = 0; i < n; ++i) {
        for (int j = l; j >= cordeles[i].longitud; --j) {
            numComb[j] += numComb[j - cordeles[i].longitud];
            if (minCord[j - cordeles[i].longitud] < Infinito) {
                minCord[j] = std::min(minCord[j], minCord[j - cordeles[i].longitud] + 1);
                minCost[j] = std::min(minCost[j], minCost[j - cordeles[i].longitud] + cordeles[i].precio);
            }
        }
    }
    tSol sol; sol.numComb = numComb[l]; sol.minCord = minCord[l]; sol.minCost = minCost[l];
    return sol;
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // Datos de entrada
    int N, L;
    std::cin >> N >> L;
    if (!std::cin)  // fin de la entrada
        return false;
    std::vector<Cordel> cordeles(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> cordeles[i].longitud >> cordeles[i].precio;
    }

    tSol sol = preguntas(cordeles, L, N);

    // escribir sol
    if (sol.numComb != 0)
        std::cout << "SI" << ' ' << sol.numComb << ' ' << sol.minCord << ' ' << sol.minCost << "\n";
    else std::cout << "NO\n";

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