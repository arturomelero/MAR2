// Arturo Melero Ortiz

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <algorithm>

struct Nodo {
    int k;                         // Persona que estamos considerando
    int beneficio;                 // Cantidad de basura recogida
    int beneficio_estimado;
    int numLimpias;                 // Entero que almacena la cantidad de playas limpias
    std::vector<int> suciedad;      // limpias[i] indica que la playa i ya está completamente limpia
    bool operator < (Nodo const& otro) const {
        return otro.beneficio_estimado > beneficio_estimado;
    }
};

// Funcion de estimacion --> No es necesaria, pues empleamos vector de acumulados con la maxima cantidad de basura por playa que puede recoger la siguiente persona

void algoritmoRP(int n, int m, int l, int& beneficio_mejor, int& playasLimpias, const std::vector<int>& basura, const std::vector<std::vector<int>>& personas, const std::vector<int>& estimacion) {
    beneficio_mejor = -1;
    // Inicializamos el nodo raíz
    Nodo Y;
    Y.k = -1; Y.beneficio = 0;
    Y.beneficio_estimado = estimacion[0];
    Y.numLimpias = playasLimpias;
    Y.suciedad = basura;
    std::priority_queue<Nodo> cola;
    cola.push(Y);

    // Aplicamos algoritmo de ramificacion y poda
    while (!cola.empty() && cola.top().beneficio_estimado >= beneficio_mejor) {
        Y = cola.top(); cola.pop();
        Nodo X = Nodo(Y); ++X.k;
        // Si me quedan mas playas por limpiar que personas, no es solucion
        if (n - X.k >= l - X.numLimpias) {
            // Consideramos asignar cualquiera de las playas
            for (int playa = 0; playa < m; ++playa) {
                // Si la playa no esta limpia, la consideramos
                if (X.suciedad[playa] > 0) {
                    int limpieza = std::min(personas[X.k][playa], X.suciedad[playa]);
                    X.beneficio = Y.beneficio + limpieza;
                    X.beneficio_estimado = X.beneficio + estimacion[X.k + 1];
                    X.suciedad[playa] -= limpieza;
                    if (X.suciedad[playa] == 0) ++X.numLimpias;
                    // Consideramos si es una solucion
                    if (X.beneficio_estimado > beneficio_mejor) {
                        if (X.k == n - 1) {
                            if (X.numLimpias >= l) {
                                beneficio_mejor = X.beneficio;
                                playasLimpias = X.numLimpias;
                            }
                        }
                        else {
                            cola.push(X);
                        }
                    }
                    // Desmarcamos
                    X.suciedad[playa] += limpieza;
                    if (X.numLimpias > Y.numLimpias) --X.numLimpias;
                }
            }
        }
    }
}

void resuelveCaso() {
    // Datos de entrada
    int n, m, l, beneficio_mejor, playasLimpias = 0;
    std::cin >> n >> m >> l;
    std::vector<int> basura(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> basura[i];
        if (basura[i] == 0) ++playasLimpias;
    }
    std::vector<std::vector<int>> personas(n, std::vector<int>(m));
    std::vector<int> estimacion(n + 1); // vector de estimacion
    for (int i = 0; i < n; ++i) {
        std::cin >> personas[i][0];
        estimacion[i] = std::min(personas[i][0], basura[0]);
        for (int j = 1; j < m; ++j) {
            std::cin >> personas[i][j];
            int aux = std::min(personas[i][j], basura[j]);
            if (aux > estimacion[i]) estimacion[i] = aux;
        }
    }

    for (int i = n - 2; i >= 0; --i) {
        estimacion[i] += estimacion[i + 1];
    }
    estimacion[n] = 0;
    // Imprimimos la solucion
    algoritmoRP(n, m, l, beneficio_mejor, playasLimpias, basura, personas, estimacion);
    beneficio_mejor == -1 ? std::cout << "IMPOSIBLE\n" : std::cout << beneficio_mejor << " " << playasLimpias << "\n";
}

int main() {
    // ajustes para que cin extraiga directamente de un fichero
#ifndef DOMJUDGE
    std::ifstream in("casos.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

    int n; std::cin >> n;
    for (int i = 0; i < n; ++i) resuelveCaso();

    // para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif
    return 0;
}