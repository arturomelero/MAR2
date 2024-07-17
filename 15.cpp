// Arturo Melero Ortiz

/*
    QUITANIEVES EN INVERNALIA -------------------------------------------------------

        --> Se desea maximizar la calidad de limpieza de n caminos empleando m(>n) carros de anchura determinada y con
            una calidad particular asociada para cada camino.
                Especulación: No se puede aplicar poda optimista-pesimista pues hay soluciones parciales que no cumplen
                restricciones del problema

            Espacio de soluciones: Las soluciones son tuplas de pares que relacionan un carro con su camino, de forma que se
            maximice la calidad de la limpieza. En nuestro caso, no necesitamos guardar dichas asociatividades, así que solo almacenamos
            el valor de beneficio maximo. Para ello, con un vector de booleanos marcamos los caminos con un carro ya asignado.

            La cota de estimacion es una funcion que calcula la asignacion con beneficio maximo para los caminos disponibles.

*/

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>

struct Nodo {
    int k;
    int beneficio;
    int beneficio_estimado;
    std::vector<bool> asig; // pos indica si dicho camino ha sido asignado a un carro
    bool operator < (Nodo const& otro) const {
        return otro.beneficio_estimado > beneficio_estimado;
    }
};

// Funcion coste estimado: Calcula el maximo de los carros disponibles
int calculo_estimacion(std::vector<std::vector<int>> const& calidad, std::vector<int> const& carros, std::vector<int> const& caminos, Nodo const& X) {
    int estimacion = X.beneficio;
    int i = X.k + 1;
    while (i < calidad.size()) {
        int max = 0;
        for (int j = 0; j < calidad[0].size(); ++j) {
            if (!X.asig[j] && carros[i] <= caminos[j] && calidad[i][j] > max)
                max = calidad[i][j];
        }
        estimacion += max;
        ++i;
    }
    return estimacion;
}

void quitanievesRP(std::vector<std::vector<int>> const& calidad, std::vector<int> const& carros, std::vector<int> const& caminos, int& beneficio_mejor) {
    size_t m = caminos.size();
    // Generamos la raiz -----------------------------
    Nodo Y;
    Y.asig = std::vector<bool>(m, false); Y.k = -1;  // tupla sol vacia
    Y.beneficio = 0; Y.beneficio_estimado = calculo_estimacion(calidad, carros, caminos, Y);
    std::priority_queue<Nodo> cola;
    cola.push(Y);
    beneficio_mejor = 0;

    // Busqueda mientras pueda haber algo mejor
    while (!cola.empty() && cola.top().beneficio_estimado > beneficio_mejor) {
        Y = cola.top(); cola.pop();
        Nodo X(Y);
        ++X.k;
        for (int i = 0; i < m; ++i) {
            // Probamos a considerar el carro
            if (!Y.asig[i] && carros[Y.k + 1] <= caminos[i]) {
                // Asignamos carro i-esimo
                X.asig[i] = true;
                X.beneficio = Y.beneficio + calidad[X.k][i];
                X.beneficio_estimado = calculo_estimacion(calidad, carros, caminos, X);
                // Es mejor solucion?
                if (X.beneficio_estimado > beneficio_mejor) {
                    if (X.k == (int)carros.size() - 1)
                        beneficio_mejor = X.beneficio;
                    else cola.push(X);
                }
                X.asig[i] = false;
            }
            
        }
        // No asignamos carro
        X.beneficio = Y.beneficio;
        X.beneficio_estimado = calculo_estimacion(calidad, carros, caminos, X);
        if (X.beneficio_estimado > beneficio_mejor) {
            if (X.k == (int)carros.size() - 1)
                beneficio_mejor = X.beneficio;
            else cola.push(X);
        }
    }
}

bool resuelveCaso() {
    // Datos de entrada
    int n, m;
    std::cin >> n >> m;
    std::vector<int> carros(n);
    for (int i = 0; i < n; ++i) std::cin >> carros[i];
    std::vector<int> caminos(m);
    for (int i = 0; i < m; ++i) std::cin >> caminos[i];
    std::vector<std::vector<int>> calidad(n, std::vector<int>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            std::cin >> calidad[i][j];
        }
    }

    // Invocamos a la funcion que resuelve el problema e imprimimos solucion
    int beneficio_mejor = 0;
    if (n != 0) quitanievesRP(calidad, carros, caminos, beneficio_mejor);
    std::cout << beneficio_mejor << "\n";

    return true;
}

int main() {
    // ajustes para que cin extraiga directamente de un fichero
#ifndef DOMJUDGE
    std::ifstream in("casos.txt");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

    int numCasos;
    std::cin >> numCasos;
    for (int i = 0; i < numCasos; ++i)
        resuelveCaso();

    // para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    system("PAUSE");
#endif
    return 0;
}