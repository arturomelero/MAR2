// Arturo Melero Ortiz

/*
    CINE ROMÁNTICO A RAUDALES  -------------------------------------------------------

        --> Dadas N películas, proporcionadas en orden cronológico, con hora de inicio y duracion,
            se desea maximizar el tiempo de visualizacion de las peliculas, considerando que se necesitan 
            10 minutos entre películas.
            selec(i) representa el tiempo que puede pasar viendo peliculas seleccionando hasta la pelicula i

            El coste de la solución es O(N^2), siendo N el numero de peliculas.
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

struct Pelicula {
    int inicio, duracion;
};

bool compararPeliculas(const Pelicula& a, const Pelicula& b) {
    return a.inicio < b.inicio;
}

// Implementacion descendente
int maxVisualizacion(const std::vector<Pelicula>& peliculas) {
    int n = peliculas.size();
    std::vector<int> selec(n);

    // Calculando la duración máxima considerando los 10 minutos de descanso entre películas
    for (int i = 0; i < n; ++i) {
        selec[i] = peliculas[i].duracion;
        for (int j = 0; j < i; ++j) {
            if (peliculas[i].inicio >= peliculas[j].inicio + peliculas[j].duracion + 10) {
                selec[i] = std::max(selec[i], selec[j] + peliculas[i].duracion);
            }
        }
    }

    // Encontrando la duración máxima
    int maxDuracion = 0;
    for (int i = 0; i < n; ++i) {
        maxDuracion = std::max(maxDuracion, selec[i]);
    }

    return maxDuracion;
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // Datos de entrada
    int N; std::cin >> N;
    if (N == 0)  // fin de la entrada
        return false;
    std::vector<Pelicula> peliculas(N);
    for (int i = 0; i < N; ++i) {
        int hora, min;
        char c;
        std::cin >> hora >> c >> min >> peliculas[i].duracion;
        peliculas[i].inicio = hora * 60 + min;
    }

    sort(peliculas.begin(), peliculas.end(), compararPeliculas);

    std::cout << maxVisualizacion(peliculas) << "\n";

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