/* INF3105 - Structures de données et algorithmes
 * UQAM / Département d'informatique
 * Été 2024 / TP3
 *
 * Test d'efficacité du TP3 pour la fonction ArbreAVL::operator[](int).
 * Pour compiler: g++ -Wall -Wextra -O3 -o tp3_operator tp3_operator.cpp
 */

#include <iostream>
#include <chrono>

#include "arbreavl.h"

using namespace std;

int main() {
  ArbreAVL<long> a;
  const int n = 50'000;

  for (int i = 0; i < n; i += 2) a.inserer(i);
  for (int i = n - 1; i > 0; i -= 2) a.inserer(i);

  const auto start = chrono::high_resolution_clock::now();
  bool ok = true;
  for (int i = 0; i < n; i++) {
    if (a[i] != i) {
      cout << "Erreur: a[" << i << "] = " << a[i] << "\n";
      ok = false;
    }
  }
  const auto stop = chrono::high_resolution_clock::now();
  const long temps_ms
      = chrono::duration_cast<chrono::milliseconds>(stop - start).count();

  cout << (ok ? "OK" : "Échec!") << "\n";
  cout << "Temps: " << temps_ms << " ms" << endl;;
  return ok ? 0 : 1;
}
