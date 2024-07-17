// Arturo Melero Ortiz

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <algorithm>

struct Nodo {
    int k;
    int beneficio;
    int beneficio_estimado;
    std::vector<int> disposicion; // asiento de cada comensal
    std::vector<bool> asiento;    // si asiento[i], significa que esta libre
    bool operator < (Nodo const& otro) const {
        return otro.beneficio_estimado > beneficio_estimado;
    }
};

const double DIST = 2;

// No se necesita funcion de estimacion, pues se utiliza el vector de allegados

void algoritmoRP(int n, int m, const std::vector<std::vector<double>>& distancias, const std::vector<std::vector<int>>& allegados, const std::vector<int>& allegadosAC, int& beneficio_mejor) {
    beneficio_mejor = -1;
    Nodo Y;
    Y.k = -1; Y.beneficio = 0;
    Y.beneficio_estimado = allegadosAC[Y.k + 1];
    Y.disposicion = std::vector<int>(m, -1);
    Y.asiento = std::vector<bool>(n, true);
    std::priority_queue<Nodo> cola;
    cola.push(Y);

    while (!cola.empty() && cola.top().beneficio_estimado >= beneficio_mejor) {
        Y = cola.top(); cola.pop();
        Nodo X = Nodo(Y); ++X.k;

        // Probamos los asientos para el comensal k
        for (int silla = 0; silla < n; ++silla) {
            if (Y.asiento[silla]) {
                bool posible = true;
                int amigos = 0;
                // Debe cumplirse distancia de seguridad con anteriores
                for (int com = 0; com <= Y.k && posible; ++com) {
                    if (distancias[silla][X.disposicion[com]] < DIST) {
                        if (allegados[X.k][com]) ++amigos;
                        else posible = false;
                    }
                }
                // Si colocado, actualizamos nodo y lo metemos en la cola
                if (posible) {
                    X.asiento[silla] = false;
                    X.disposicion[X.k] = silla;
                    X.beneficio = Y.beneficio + amigos;
                    X.beneficio_estimado = X.beneficio + allegadosAC[X.k];

                    if (X.beneficio_estimado >= beneficio_mejor) {
                        if (X.k == m - 1) {
                            beneficio_mejor = X.beneficio;
                        }
                        else {
                            cola.push(X);
                        }
                    }
                    // Desmarcamos
                    X.asiento[silla] = true;
                }
            }
        }
    }
}

void resuelveCaso() {
    // Datos de entrada
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<double>> distancias(n, std::vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> distancias[i][j];
        }
    }
    std::vector<std::vector<int>> allegados(m, std::vector<int>(m));
    std::vector<int> allegadosAC(m, 0);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            std::cin >> allegados[i][j];
            if (j > i && allegados[i][j]) ++allegadosAC[i];
        }
    }
    for (int i = m - 2; i >= 0; --i) {
        allegadosAC[i] += allegadosAC[i + 1];
    }

    // Imprimimos la solucion
    int beneficio_mejor;
    algoritmoRP(n, m, distancias, allegados, allegadosAC, beneficio_mejor);
    beneficio_mejor == -1 ? std::cout << "CANCELA\n" : std::cout << "PAREJAS " << beneficio_mejor << "\n";
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