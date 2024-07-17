// Arturo Melero Ortiz

/*
    COMPRA DE LA SEMANA -------------------------------------------------------

        --> Se desea minimizar el coste de realizar la compra de n productos en m supermercados, comprando a lo sumo 3 productos 
            por supermercado.
                Especulación: Es posible aplicar esquema optimista-pesimista al poder cumplir las restricciones implícitas desde cualquier
                solucion parcial, al haber existencias de cualquier producto en todos los supermercados.

            Espacio de soluciones: n-tuplas con los supermercados elegidos para cada producto cumpliendo con la restriccion de no comprar
            más de tres productos por supermercado. En nuestro caso, no necesitamos almacenar la información sobre qué producto es comprado en
            qué supermercado, sino solo los costes. 

            Para realizar las estimaciones se emplea el esquema optimista-pesimista, empleando para ambos vectores de acumulados precalculados
            con los precios minimos y maximos para cada producto respectivamente.

*/

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>

const int LIM_PROD = 3;

struct Nodo {
    int k;                        // Productos comprados
    int coste;                    // Suma del precio de los productos 0..k
    int coste_estimado;           
    std::vector<int> compras_sup; // i: cantidad de productos comprados en el super i
    bool operator < (Nodo const& otro) const {
        return otro.coste_estimado < coste_estimado;
    }
};

// Cota optimista: minimo de cada producto. Cota optimista: maximo de cada producto
void calculo_estimacion(std::vector<std::vector<int>> const& lista, std::vector<int>& minPrecios, std::vector<int>& maxPrecios) {
    for (int p = 0; p < lista[0].size(); ++p) {
        minPrecios[p] = lista[0][p]; maxPrecios[p] = lista[0][p];
        for (int s = 1; s < lista.size(); ++s) {
            if (lista[s][p] < minPrecios[p]) minPrecios[p] = lista[s][p];
            if (lista[s][p] > maxPrecios[p]) maxPrecios[p] = lista[s][p];
        }
    }
    // Acumulamos valores
    minPrecios[lista[0].size()] = 0; maxPrecios[lista[0].size()] = 0;
    for (int p = lista[0].size() - 2; p >= 0; --p) {
        minPrecios[p] += minPrecios[p + 1];
        maxPrecios[p] += maxPrecios[p + 1];
    }
}


void quitanievesRP(std::vector<std::vector<int>> const& lista, int& coste_mejor) {
    size_t m = lista.size(); size_t n = lista[0].size();

    // Calculamos como estimacion el vector de precios min acumulado
    std::vector<int> minPrecios(n+1); std::vector<int> maxPrecios(n+1);
    calculo_estimacion(lista, minPrecios, maxPrecios);

    // Generamos la raiz -----------------------------
    Nodo Y; 
    Y.compras_sup = std::vector<int>(m, 0);
    Y.k = -1;  // tupla sol vacia
    Y.coste = 0; Y.coste_estimado = minPrecios[0];
    std::priority_queue<Nodo> cola;
    cola.push(Y);

    coste_mejor = maxPrecios[0];

    // Busqueda mientras pueda haber algo mejor
    while (!cola.empty() && cola.top().coste_estimado <= coste_mejor) {
        Y = cola.top(); cola.pop();
        // Probamos a considerar cada super disponible
        Nodo X(Y); ++X.k;
        for (int i = 0; i < m; ++i) {
            if (Y.compras_sup[i] < LIM_PROD) {
                // Asignamos super i-esimo al producto k --- MARCAMOS
                ++X.compras_sup[i];
                X.coste = Y.coste + lista[i][X.k];
                X.coste_estimado = X.coste + minPrecios[X.k + 1];
                // Es mejor solucion?
                if (X.coste_estimado <= coste_mejor) {
                    if (X.k == n - 1)
                        coste_mejor = X.coste;
                    else {
                        cola.push(X);
                        coste_mejor = std::min(coste_mejor, X.coste + maxPrecios[X.k]);
                    }
                }
                --X.compras_sup[i];
            }
        }
    }
}

bool resuelveCaso() {
    // Datos de entrada
    int n, m;
    std::cin >> m >> n;
    std::vector<std::vector<int>> lista (m, std::vector<int> (n));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> lista[i][j];
        }
    }

    // Invocamos a la funcion que resuelve el problema e imprimimos solucion
    int coste_mejor;
    if (n != 0) quitanievesRP(lista, coste_mejor);
    std::cout << coste_mejor << "\n";
    
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