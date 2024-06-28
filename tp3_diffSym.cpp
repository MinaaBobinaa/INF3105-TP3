/* INF3105 - Structures de données et algorithmes
 * UQAM / Département d'informatique
 * Été 2024 / TP3
 *
 * Test d'efficacité du TP3 pour la fonction ArbreAVL::differenceSymetrique(const ArbreAVL&).
 * Pour compiler: g++ -Wall -Wextra -O3 -o tp3_diffSym tp3_diffSym.cpp
 */

#include <iostream>
#include <chrono>
#include <vector>

#include "arbreavl.h"

using namespace std;

int main() {
  ArbreAVL<long> a, b;
  const int n = 50'000'000;

  for (int i = 0; i < n; i++) a.inserer(i);
  for (int i = 2 * n - 1; i > 0; i -= 2) b.inserer(i);

  const auto start = chrono::high_resolution_clock::now();
  const ArbreAVL<long> diffsym = a.differenceSymetrique(b);
  const auto stop = chrono::high_resolution_clock::now();
  const long temps_ms
      = chrono::duration_cast<chrono::milliseconds>(stop - start).count();

  vector<long> resVec;
  for (int i = 0; i < n; i += 2) resVec.push_back(i);
  for (int i = n + 1; i <= 2 * n - 1; i += 2) resVec.push_back(i);

  bool ok = true;
  int i = 0;
  for (auto it = diffsym.debut(); it != diffsym.fin(); ++it, ++i) {
    if (*it != resVec[i]) {
      cout << "Erreur: Attendu " << resVec[i] << "; Obtenu " << *it << "\n";
      ok = false;
    }
  }

  cout << (ok ? "OK" : "Échec!") << "\n";
  cout << "Temps: " << temps_ms << " ms" << endl;;
  return ok ? 0 : 1;
}
