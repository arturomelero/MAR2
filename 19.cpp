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
int estimacion(int n, int m, const Nodo& X, const std::vector<std::vector<int>>& personas) {
    int est = X.beneficio, max, aux;
    for (int i = X.k + 1; i < n; ++i) {
        max = std::min(personas[i][0], X.suciedad[0]);
        for (int j = 1; j < m; ++j) {
            aux = std::min(personas[i][j], X.suciedad[j]);
            if (aux > max) max = aux;
        }
        est += max;
    }
    return est;
}

void algoritmoRP(int n, int m, int l, int& beneficio_mejor, int& playasLimpias, const std::vector<int>& basura, const std::vector<std::vector<int>>& personas) {
    beneficio_mejor = -1;
    // Inicializamos el nodo raíz
    Nodo Y;
    Y.k = -1; Y.beneficio = 0;
    Y.numLimpias = playasLimpias;
    Y.suciedad = basura;
    Y.beneficio_estimado = estimacion(n, m, Y, personas);
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
                    X.suciedad[playa] -= limpieza;
                    if (X.suciedad[playa] == 0) ++X.numLimpias;
                    X.beneficio_estimado = estimacion(n, m, X, personas);
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
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            std::cin >> personas[i][j];
        }
    }


    // Imprimimos la solucion
    algoritmoRP(n, m, l, beneficio_mejor, playasLimpias, basura, personas);
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