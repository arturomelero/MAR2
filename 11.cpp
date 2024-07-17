// Arturo Melero Ortiz

/*
    MEJOR NO LLEVAR MUCHAS MONEDAS  -------------------------------------------------------

        --> Dadas N monedas, con valor y cantidad concretos, se desea pagar un precio P
            utilizando el menor numero de monedas posibles.

            cartera(i,j) = min {0 <= k <= min{cantidad[i], j/valor[i]}} {k + cartera(i, j-k*valor[i])}

            El coste de la solución es O(n*P*min(c, P/v)). Coste en espacio O(P), con P precio
*/

#include <iostream>
#include <fstream>
#include <vector>
#include "EnterosInf.h"

struct tMoneda {
    int valor, cantidad;
};



// Implementacion descendente
EntInf minMonedas(int P, const std::vector<tMoneda>& monedas) {
    int n = monedas.size();
    std::vector<EntInf> cartera(P+1, Infinito);
    cartera[0] = 0;
    for (int i = 1; i < n + 1; ++i) {
        for (int j = P; j >= 0; --j) {
            int max = std::min(monedas[i - 1].cantidad, j / monedas[i-1].valor);
            EntInf mejor = cartera[j];
            for (int k = 0; k <= max; ++k) {
                if (mejor > cartera[j - k * monedas[i - 1].valor] + k) mejor = cartera[j - k * monedas[i - 1].valor] + k;
            }
            cartera[j] = mejor;
        }
    }
    return cartera[P];
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // Datos de entrada
    int N; std::cin >> N;
    if (!std::cin)  // fin de la entrada
        return false;
    std::vector<tMoneda> monedas(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> monedas[i].valor;
    }
    for (int i = 0; i < N; ++i) {
        std::cin >> monedas[i].cantidad;
    }
    int P; std::cin >> P;

    EntInf sol = minMonedas(P, monedas);
    sol == Infinito ? std::cout << "NO\n" : std::cout << "SI " << sol << '\n';

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