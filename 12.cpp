// Arturo Melero Ortiz

/*
    VACAS PENSANTES  -------------------------------------------------------

        --> Dada una hilera de N cubos, se desea maximizar la cantidad de comida tomada considerando 
            un rival que ataca siempre el extremo con el mayor valor.

            comer(i,j) = maxima cantidad de comida eligiendo como extremos los cubos i,j.

            El coste de la solucion es O(n^2), con coste en espacio O(n^2), sinedo n el numero cubos.
*/

#include <iostream>
#include <fstream>
#include <vector>

// Implementacion descendente
int comer(const std::vector<int>& cubos) {
    int n = cubos.size();
    std::vector<std::vector<int>> m(n, std::vector<int> (n));
    // Inicializacion
    for (int i = 0; i < n; ++i) m[i][i] = cubos[i];

    for (int d = 1; d < n; ++d) {
        for (int i = 0; i < n - d; ++i) {
            int j = i + d;
            // Quedan dos cubos
            if (i + 1 == j || i == j - 1) {
                m[i][j] = std::max(cubos[i], cubos[j]);
            }
            // Si quedan mas de dos cubos, consideramos ambos lados
            int izq, dcha;
            // Comemos por la izquierda
            izq = cubos[i] + (cubos[i + 1] > cubos[j] ? m[i + 2][j] : m[i + 1][j - 1]);
            // Comemos por la derecha
            dcha = cubos[j] + (cubos[j - 1] > cubos[i] ? m[i][j - 2] : m[i + 1][j - 1]);
            // Nos quedamos con el maximo
            m[i][j] = std::max(izq, dcha);
        }
    }
    return m[0][n - 1];
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // Datos de entrada
    int N; std::cin >> N;
    if (N == 0)  // fin de la entrada
        return false;
    std::vector<int> cubos(N);
    for (int i = 0; i < N; ++i) std::cin >> cubos[i];

    std::cout << comer(cubos) << '\n';
    
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