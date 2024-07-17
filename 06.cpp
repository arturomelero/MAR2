// Arturo Melero Ortiz

/*
    AIBOFOBIA -------------------------------------------------------

        --> Dada una palabra, devolver el menor numero de letras que es necesario
            añadir para formar un palíndromo

            palindromo (i, j) = numero minimo de letras que hay que añadir en [i..j] para que sea palindromo

            palindromo(i, j) = | palindromo (i+1, j-1) si palabra[i] = palabra[j];
                               | 1 + min(palindromo (i-1, j), palindromo (i, j-1)

            El coste de la solución es O(), siendo
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Implementación descendente
int aibofobia2(int i, int j, const std::string& palabra, std::vector<std::vector<int>>& palindromo) {
    if (i == j) return 0;
    else if (i > j) return 0;
    else {
        if (palabra[i] == palabra[j]) palindromo[i][j] = aibofobia2(i + 1, j - 1, palabra, palindromo);
        else palindromo[i][j] = 1 + std::min(aibofobia2(i + 1, j, palabra, palindromo), aibofobia2(i, j - 1, palabra, palindromo));
        return palindromo[i][j];
    }
}

// Implementacion ascendente
int aibofobia(const std::string& palabra, std::vector<std::vector<int>>& palindromo) {
    int max = palabra.size();
    for (int i = 1; i < max; ++i) {
        for (int j = i; j < max; ++j) {
            int d = j - i;
            if (palabra[d] == palabra[j]) palindromo[d][j] = palindromo[d + 1][j - 1];
            else palindromo[d][j] = 1 + std::min(palindromo[d][j - 1], palindromo[d + 1][j]);
        }
    }
    return palindromo[0][max - 1];
}

void solucion(int i, int j, std::string const& palabra, std::vector<std::vector<int>>& palindromo, std::string& sol) {
    if (i > j) return;
    else if (i == j) sol.push_back(palabra[i]);
    else if (palabra[i] == palabra[j]) {
        sol.push_back(palabra[i]);
        solucion(i + 1, j - 1, palabra, palindromo, sol);
        sol.push_back(palabra[j]);
    }
    else if (palindromo[i][j] > palindromo[i][j - 1]) {
        sol.push_back(palabra[j]);
        solucion(i, j - 1, palabra, palindromo, sol);
        sol.push_back(palabra[j]);
    }
    else {
        sol.push_back(palabra[i]);
        solucion(i + 1, j, palabra, palindromo, sol);
        sol.push_back(palabra[i]);
    }
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // Datos de entrada
    std::string palabra;
    std::cin >> palabra;
    if (!std::cin)  // fin de la entrada
        return false;
    std::vector<std::vector<int>> palindromo(palabra.size(), std::vector<int>(palabra.size(), 0));

    // escribir sol
    std::cout << aibofobia(palabra, palindromo) << " ";
    std::string sol;
    solucion(0, palabra.size() - 1, palabra, palindromo, sol);
    std::cout << sol << "\n";

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