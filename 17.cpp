// Arturo Melero Ortiz

/*
    PEPE CASANOVA -------------------------------------------------------

        --> Se desea maximizar la satisfaccion de la recopilacion de n canciones en un disco cuyo tamaño viene limitado por la duracion maxima
            de reproduccion que puede almacenar. La cinta del casete tiene dos cara y ambas deben respetar que no pueden incluir una cancion
            partida. No vale seleccionar canciones repetidas.
                Especulación: Es posible aplicar esquema optimista-pesimista al poder cumplir las restricciones implícitas desde cualquier
                solucion parcial, pues hasta una lista sin canciones es una solucion admisible (si ninguna cancion cabe).

            Espacio de soluciones: una tupla con las canciones seleccionadas para cada cara de la cinta, pudiendo estar en la primera, en la segunda
            o no haber sido seleccionada. En nuestro caso, solamente nos interesa almacenar la satisfaccion maxima, por lo que no es necesario almacenar
            la representacion de la solucion.

            Para realizar las estimaciones se emplea el esquema optimista-pesimista, empleando:
                Para la estimacion optimista, algoritmo voraz que, tomando la lista de canciones en orden creciente por punt/dur, las introduce en un
                casete supuesto que puede almacenar todo de seguido sin la restriccion de las caras.
                Para la estimacion pesimista, tratamos de introducir canciones mientras quepan, primero en la primera cara y después en la segunda.

*/

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <algorithm>

struct Nodo {
    int k;
    int beneficio;
    int beneficio_estimado;
    std::pair<int, int> cinta;
    bool operator < (Nodo const& otro) const {
        return otro.beneficio_estimado > beneficio_estimado;
    }
};

struct tCancion {
    int duracion, puntuacion;
    bool operator<(tCancion const& otro) const {
        return (puntuacion / (double)duracion) > (otro.puntuacion / (double)otro.duracion);
    }
};



double estimacionOptimista(int t, const std::vector<tCancion>& canciones, const Nodo& X) {
    size_t n = canciones.size() - 1;
    int hueco = 2*t - X.cinta.first - X.cinta.second;
    double optimo = X.beneficio;
    int i = X.k + 1;
    while (i <= n && canciones[i].duracion <= hueco) {
        hueco -= canciones[i].duracion;
        optimo += (double)canciones[i].puntuacion;
        ++i;
    }
    if (i <= n) optimo += (hueco / (double)canciones[i].duracion) * canciones[i].puntuacion;
    return optimo;
}

int estimacionPesimista(int t, const std::vector<tCancion>& canciones, const Nodo& X) {
    size_t n = canciones.size() - 1;
    int pesimista = X.beneficio;
    int i = X.k + 1;
    std::pair<int, int> tiempo = { X.cinta.first , X.cinta.second };
    while (i <= n && (tiempo.first <= t || tiempo.second <= t)) {
        if (tiempo.first + canciones[i].duracion <= t) {
            tiempo.first += canciones[i].duracion;
            pesimista += canciones[i].puntuacion;
        }
        else if (tiempo.second + canciones[i].duracion <= t) {
            tiempo.second += canciones[i].duracion;
            pesimista += canciones[i].puntuacion;
        }
        ++i;
    }
    return pesimista;
}


void algoritmoRP(int t, std::vector<tCancion> const& lista, double& beneficio_mejor) {
    size_t n = lista.size() - 1;

    // Generamos la raiz -----------------------------
    Nodo Y; Y.k = 0;
    Y.beneficio = 0;
    Y.cinta = { 0, 0 };
    Y.beneficio_estimado = estimacionOptimista(t, lista, Y);

    beneficio_mejor = estimacionPesimista(t, lista, Y);

    std::priority_queue<Nodo> cola;
    cola.push(Y);

    // Busqueda mientras pueda haber algo mejor
    while (!cola.empty() && cola.top().beneficio_estimado > beneficio_mejor) {
        Y = cola.top(); cola.pop();
        Nodo X(Y); ++X.k;
        // Meter en la primera cara
        if (Y.cinta.first + lista[Y.k + 1].duracion <= t) {
            X.cinta = { Y.cinta.first + lista[X.k].duracion, Y.cinta.second };
            X.beneficio = Y.beneficio + lista[X.k].puntuacion;
            X.beneficio_estimado = Y.beneficio_estimado; // Mismo porque metemos la cancion
            // Es mejor solucion?
            if (X.beneficio_estimado >= beneficio_mejor) { // pesimista no varia porque ponemos cancion en primera cara
                if (X.k == n)
                    beneficio_mejor = X.beneficio;
                else {
                    cola.push(X);
                }
            }
        }
        // Meter en la segunda cara
        if (Y.cinta.first != Y.cinta.second && Y.cinta.second + lista[Y.k + 1].duracion <= t) {
            X.cinta = { Y.cinta.first, Y.cinta.second + lista[X.k].duracion };
            X.beneficio = Y.beneficio + lista[X.k].puntuacion;
            X.beneficio_estimado = Y.beneficio_estimado; // Mismo porque metemos la cancion
            if (X.beneficio_estimado >= beneficio_mejor) {
                if (X.k == n)
                    beneficio_mejor = X.beneficio; // pesimista no varia porque ponemos cancion en primera cara
                else {
                    cola.push(X);
                    beneficio_mejor = std::max(beneficio_mejor, (double)estimacionPesimista(t, lista, X));
                }
            }
        }
        // No meter la cancion
        X.cinta = Y.cinta;
        X.beneficio = Y.beneficio;
        X.beneficio_estimado = estimacionOptimista(t, lista, X);
        if (X.beneficio_estimado >= beneficio_mejor) {
            if (X.k == n)
                beneficio_mejor = X.beneficio;
            else {
                cola.push(X);
                beneficio_mejor = std::max(beneficio_mejor, (double)estimacionPesimista(t, lista, X));
            }
        }
    }
}

bool resuelveCaso() {
    // Datos de entrada
    int n; std::cin >> n;
    if (n == 0) return false;
    int t; std::cin >> t;
    std::vector<tCancion> lista(n+1);
    for (int i = 1; i <= n; ++i) {
        std::cin >> lista[i].duracion >> lista[i].puntuacion;
    }

    // ordenamos por puntuacion/duracion
    std::sort(lista.begin() + 1, lista.end());

    // Invocamos a la funcion que resuelve el problema e imprimimos solucion
    double beneficio_mejor;
    if (n != 0) algoritmoRP(t, lista, beneficio_mejor);
    std::cout << beneficio_mejor << "\n";

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