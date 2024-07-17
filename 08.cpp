// Arturo Melero Ortiz

/*
    SEIS GRADOS DE SEPARACION  -------------------------------------------------------

        --> Dadas P personas y R relaciones entre ellas, se desea calcular el grado de separacion
            entre ellas, entendido como el numero minimo de relaciones que hay que atravesar para
            conectarlas. Es similar al algoritmo de Floyd.

            separacion(i,j) = nº minimo de nodos a atravesar para ir de i a j pasando por k.

            El coste de la solución es O(P^3), siendo P el numero de personas.
            El coste en espacio es O(P^2).
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "EnterosInf.h"

// Variable global para las comprobaciones. Solo se modifica en resuelveCaso
int max;

// Implementacion descendente
EntInf gradoSeparacion(int P, const std::vector<std::vector<EntInf>>& G) {
    // Inicializacion
    std::vector<std::vector<EntInf>> separacion (P, std::vector<EntInf>(P, Infinito));
    
    // Diagonal a cero y rellenamos con valores del grafo
    for (int i = 0; i < P; ++i) {
        for (int j = 0; j < P; ++j) {
            if (i == j) separacion[i][j] = 0;
            else if (G[i][j] != 0) separacion[i][j] = G[i][j];
          
        }
    }

    // Aplicamos el algoritmo de Floyd
    for (int k = 0; k < P; ++k) {
        for (int i = 0; i < P; ++i) {
            for (int j = 0; j < P; ++j) {
                if (separacion[i][k] != Infinito && separacion[k][j] != Infinito && separacion[i][k] + separacion[k][j] < separacion[i][j]) {
                    separacion[i][j] = separacion[i][k] + separacion[k][j];
                }
            }
        }
    }

    // Buscamos el máximo valor en la matriz resultado
    EntInf maxGrado = 0;
    for (int i = 0; i < P; ++i) {
        for (int j = 0; j < P; ++j) {
            if (separacion[i][j] == Infinito) {
                return Infinito; // Si hay nodos desconectados, devolvemos -1
            }
            maxGrado = std::max(maxGrado, separacion[i][j]);
        }
    }

    return maxGrado;
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // Datos de entrada
    int P, R;
    std::cin >> P >> R;
    if (!std::cin)  // fin de la entrada
        return false;
    
    // Creamos un mapa para asignar un índice único a cada persona
    std::unordered_map<std::string, int> personIndex;
    std::vector<std::vector<EntInf>> grafo(P, std::vector<EntInf>(P, Infinito));

    // Llenamos la matriz de adyacencia con las relaciones directas
    for (int i = 0; i < R; ++i) {
        std::string p1, p2;
        std::cin >> p1 >> p2;

        if (personIndex.find(p1) == personIndex.end())
            personIndex[p1] = personIndex.size();
        if (personIndex.find(p2) == personIndex.end())
            personIndex[p2] = personIndex.size();

        int ind1 = personIndex[p1];
        int ind2 = personIndex[p2];
        grafo[ind1][ind2] = grafo[ind2][ind1] = 1;
    }

    EntInf sol = gradoSeparacion(P, grafo);

    if (sol == Infinito) std::cout << "DESCONECTADA\n";
    else std::cout << sol << "\n";

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