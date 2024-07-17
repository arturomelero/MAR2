// Arturo Melero Ortiz

/*
    LOS FUNCIONARIOS DEL MINISTERIO  -------------------------------------------------------

        --> Se desea establecer una biyección entre un conjunto de n
            trabajos y un grupo de n funcionarios de forma que la suma de los tiempos invertidos en la realización de 
            los mismos sea mínima. Todos los funcionarios son capaces de hacer cualquiera de los trabajos aunque tardan 
            distinto tiempo, información que viene dada por una matriz de tiempos.

            Espacio de soluciones: El conjunto de soluciones es una tupla de longitud fija asig(1..n) tal que
            asigna a cada funcionario 1..n un trabajo 1..n tal que dos funcionarios no tienen un mismo trabajo.
            Como no necesitamos almacenar la asignacion de trabajos, el vector solucion es un vector de booleanos que
            indica si un trabajo i ha sido asignado para hacer mas eficientes las comprobaciones.

            La cota de estimacion es una funcion que calcula la asignacion con coste minimo para los trabajos disponibles.

*/

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>


struct Nodo {
    std::vector<bool> asig; // pos indica si trabajo pos ha sido asignado;
    int k;                  // Junto con var anterior --> Sol parcial
    int coste;           // coste acumulado
    int coste_est;       // prioridad
    
    // Operador menor, preparado para una cola de minimos
    bool operator < (Nodo const& otro) const { 
        return otro.coste_est < coste_est;
    }
};


// Funcion coste estimado: Calcula el minimo de entre los trabajos disponibles
int calculo_estimacion(std::vector<std::vector<int>> const& ministerio, Nodo const& X, const int max) {
    int estimacion = X.coste;
    int i = X.k + 1;
    while (i < ministerio.size()) {
        int min = max;
        for (int j = 0; j < ministerio.size(); ++j) {
            if (!X.asig[j] && ministerio[j][i] < min)
                min = ministerio[j][i];
        }
        estimacion += min;
        ++i;
    }
    return estimacion;
}

void funcionariosRP(std::vector<std::vector<int>> const& ministerio, const int max, int& coste_mejor) {
    size_t N = ministerio.size();
    // Generamos la raiz -----------------------------
    Nodo Y; 
    Y.asig = std::vector<bool>(N, false); Y.k = -1;  // tupla sol vacia
    Y.coste = 0; Y.coste_est = calculo_estimacion(ministerio, Y, max);
    std::priority_queue<Nodo> cola;
    cola.push(Y);
    coste_mejor = max;
    
    // Busqueda mientras pueda haber algo mejor
    while (!cola.empty() && cola.top().coste_est < coste_mejor) {
        Y = cola.top(); cola.pop();
        // Probamos a considerar trabajo i-esimo del funcionario
        for (int i = 0; i < N; ++i) {
            Nodo X(Y);
            ++X.k;
            // Comprobamos que dicha tarea no haya sido asignada antes
            if (!Y.asig[i]) {
                // Asignamos trabajo i-esimo
                X.asig[i] = true; 
                X.coste = Y.coste + ministerio[i][X.k];
                X.coste_est = calculo_estimacion(ministerio, X, max);
                // Es mejor solucion?
                if (X.k == N - 1 && X.coste_est < coste_mejor) {
                    coste_mejor = X.coste;
                }
                else cola.push(X);
            }
        }
    }
}

bool resuelveCaso() {
    // Datos de entrada
    int N; std::cin >> N;
    if (N == 0)  // fin de la entrada
        return false;

    int max = 0;
    std::vector<std::vector<int>> ministerio(N, std::vector<int> (N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cin >> ministerio[i][j]; max += ministerio[i][j];
        }
    }

    // Imprimimos solucion
    int coste_mejor;
    funcionariosRP(ministerio, max, coste_mejor);
    std::cout << coste_mejor << "\n";
    
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