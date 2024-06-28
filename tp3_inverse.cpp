/* INF3105 - Structures de données et algorithmes
 * UQAM / Département d'informatique
 * Été 2024 / TP3
 *
 * Test d'efficacité du TP3 pour la fonction ArbreAVL::arbreInverse().
 * Pour compiler: g++ -Wall -Wextra -O3 -o tp3_inverse tp3_inverse.cpp
 */

#include <chrono>
#include <iostream>

#include "arbreavl.h"

using namespace std;

int main() {
  ArbreAVL<long> a;
  const int n = 20'000'000;

  for (int i = 0; i <= n; i += 2) a.inserer(i);

  const auto start = chrono::high_resolution_clock::now();
  const ArbreAVL<long> b = a.arbreInverse();
  const auto stop = chrono::high_resolution_clock::now();
  const long temps_ms
      = chrono::duration_cast<chrono::milliseconds>(stop - start).count();

  bool ok = true;
  int i = 0;
  for (auto it = b.debut(); it != b.fin(); ++it, ++i) {
    if (*it != -n + 2 * i) {
      cout << "Erreur: Attendu " << -n + 2 * i << "; Obtenu " << *it << "\n";
      ok = false;
    }
  }

  cout << (ok ? "OK" : "Échec!") << "\n";
  cout << "Temps: " << temps_ms << " ms\n";
  return ok ? 0 : 1;
}
