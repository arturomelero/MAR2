// Arturo Melero Ortiz

/*
    LUCES PARA UNA FIESTA -------------------------------------------------------

        --> Para una serie de tipos de bombillas, con coste y potencia, dada una
            potencia máxima y una potencia mínima debemos minimizar el coste de la
            instalación.

            El coste de la solución es O(n*pMax), siendo n el num de bombillas y pMax la potencia máxima
*/

#include <iostream>
#include <fstream>
#include <vector>
#include "EnterosInf.h"

struct Bombilla {
    int potencia;
    int precio;
};

std::pair<EntInf, EntInf> resolver(int pMin, int pMax, const std::vector<Bombilla>& bombillas) {
    unsigned int n = bombillas.size();
    std::vector<EntInf> instalacion(pMax + 1, Infinito);
    // Caso base: con 0 bombillas, el coste mínimo es 0 para cualquier potencia
    instalacion[0] = 0;
    // Llenar la tabla de programación dinámica
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= pMax; ++j) {
            if(bombillas[i-1].potencia <= j)
                instalacion[j] = std::min(instalacion[j], instalacion[j - bombillas[i - 1].potencia] + bombillas[i - 1].precio);
        }
    }

    // Encontrar coste mínimo y su potencia
    EntInf potenciaOptima = Infinito, minPrecio = Infinito;
    for (int p = pMin; p <= pMax; ++p) {
        if (instalacion[p] < minPrecio) {
            minPrecio = instalacion[p];
            potenciaOptima = p;
        }
    }
    return { minPrecio, potenciaOptima };
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {

    // leer los datos de la entrada
    int n, pMax, pMin;
    std::cin >> n >> pMax >> pMin;
    if (!std::cin)  // fin de la entrada
        return false;
    std::vector<Bombilla> bombillas(n);
    for (int i = 0; i < n; ++i)
        std::cin >> bombillas[i].potencia;
    for (int i = 0; i < n; ++i)
        std::cin >> bombillas[i].precio;

    std::pair<EntInf, EntInf> sol = resolver(pMin, pMax, bombillas);

    // escribir sol
    if (sol.first == Infinito || sol.second == Infinito)  std::cout << "IMPOSIBLE\n";
    else std::cout << sol.first << " " << sol.second << "\n";


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