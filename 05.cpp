// Arturo Melero Ortiz

/*
    LA CUERDA DE LA COMETA -------------------------------------------------------

        --> Formar una cuerda de longitud L con n cordeles con coste y longitud
            de manera que se devuelva si es posible formar la cuerda, el nº de
            combinaciones posibles, min num de cordeles y min coste.

            El coste de la solución es O(n*l), coste espacial O(l).
*/

#include <iostream>
#include <fstream>
#include <vector>

struct Cofre {
    int prof;
    int oro;
};

struct tSol {
    int oro = 0;
    std::vector<Cofre> cofres;
};

tSol preguntas(int s, const std::vector<Cofre>& cofres) {
    std::vector<std::vector<int>> buceo(cofres.size() + 1, std::vector<int>(s + 1, 0));
    for (int i = 1; i <= cofres.size(); ++i) {
        for (int j = 1; j <= s; ++j) {
            if (3 * cofres[i - 1].prof > j) buceo[i][j] = buceo[i - 1][j];
            else buceo[i][j] = std::max(buceo[i - 1][j], buceo[i - 1][j - 3 * cofres[i - 1].prof] + cofres[i - 1].oro);
        }
    }
    tSol sol;
    if (buceo[cofres.size()][s] != 0) {
		sol.oro = buceo[cofres.size()][s];
		int i = cofres.size(), j = s;
		while (j > 0 && i > 0) {
			if (buceo[i - 1][j] != buceo[i][j]) {
				sol.cofres.push_back(cofres[i - 1]);
                j -= 3 * cofres[i-1].prof;
			}
			--i;
		}
    }
    return sol;
}

// resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // Datos de entrada
    int s, n;
    std::cin >> s >> n;
    if (!std::cin)  // fin de la entrada
        return false;
    std::vector<Cofre> cofres(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> cofres[i].prof >> cofres[i].oro;
    }

    tSol sol = preguntas(s, cofres);

    // escribir sol
    std::cout << sol.oro << '\n' << sol.cofres.size() << "\n";
    for (int i = 0; i < sol.cofres.size(); ++i) {
        std::cout << cofres[i].prof << ' ' << cofres[i].oro << '\n';
    }
    std::cout << "---\n";

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