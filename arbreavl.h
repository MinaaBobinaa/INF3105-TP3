/*
  INF3105 - Structures de données et algorithmes
  UQAM / Département d'informatique
  Squelette de départ du TP3 - Été 2024

  Suggestions:
   - utilisez votre solution des Lab6 et Lab7;
   - ajoutez-y les fonctions demandées ci-dessous.
*/

#ifndef _ARBREAVL_H_
#define _ARBREAVL_H_

#include <cassert>
#include <algorithm>

#include "pile.h"

template<class T>
class ArbreAVL {
 public:
  ArbreAVL();
  ArbreAVL(const ArbreAVL&);
  ~ArbreAVL();
  ArbreAVL& operator=(const ArbreAVL&);

  bool vide() const;
  bool contient(const T&) const;
  void inserer(const T&);
  void enlever(const T&);
  void vider();

  // Fonctions pour certains tests ou diagnostique
  int compter() const;
  int hauteur() const;
  void afficher() const;

  // Annonce l'existence d'une classe Iterateur.
  // Nécessaire pour définir la classe Iterateur APRÈS la classe Noeud.
  class Iterateur;

  // Fonctions pour obtenir un itérateur (position dans l'arbre)
  Iterateur debut() const;
  Iterateur fin() const;
  Iterateur rechercher(const T&) const;
  Iterateur rechercherEgalOuSuivant(const T&) const;
  Iterateur rechercherEgalOuPrecedent(const T&) const;

  // ----- Fonctions supplémentaires pour le TP3 -----
  const T& operator[](int) const;
  ArbreAVL arbreInverse() const;
  ArbreAVL differenceSymetrique(const ArbreAVL&) const;


  T& operator[](int);

  // Accès aux éléments de l'arbre via un intérateur.
  const T& operator[](const Iterateur&) const;
  T& operator[](const Iterateur&);
 
 private:
  struct Noeud {
    Noeud(const T&);
    T contenu;
    int equilibre;
    int size; // taille du sous-arbre dont ce nœud est la racine
    Noeud *gauche, *droite;
  };

  Noeud* racine;

  // Fonctions internes
  bool inserer(Noeud*&, const T&);
  void rotationGaucheDroite(Noeud*&);
  void rotationDroiteGauche(Noeud*&);
  void vider(Noeud*&);
  bool enlever(Noeud *&noeud, const T &element);

  // Lab6 / Tâche 4
  void copier(const Noeud*, Noeud*&) const;

  // Fonctions internes pour certains tests
  int hauteur(const Noeud*) const;
  int compter(const Noeud*) const;
  void preparer_afficher(const Noeud* n, int profondeur, int& rang, T* elements,
                         int* profondeurs) const;

  const T& elementAt(Noeud*, int) const;
  T& elementAt(Noeud*, int);

 public:
  class Iterateur {
   public:
    Iterateur(const ArbreAVL& a);
    Iterateur(const Iterateur& a);
    Iterateur(const ArbreAVL& a, Noeud* c);

    operator bool() const;
    bool operator!() const;
    bool operator==(const Iterateur&) const;
    bool operator!=(const Iterateur&) const;

    const T& operator*() const;

    Iterateur& operator++();
    Iterateur operator++(int);
    Iterateur& operator=(const Iterateur&);

   private:
    const ArbreAVL& arbre_associe;
    Noeud* courant;
    Pile<Noeud*> chemin;

    friend class ArbreAVL;
  };
};

template<class T>
ArbreAVL<T>::Noeud::Noeud(const T& c)
    : contenu(c), equilibre(0), gauche(nullptr), droite(nullptr) {}

template<class T>
ArbreAVL<T>::ArbreAVL() : racine(nullptr) {}

template<class T>
ArbreAVL<T>::ArbreAVL(const ArbreAVL<T>& autre) : racine(nullptr) {
  copier(autre.racine, racine);
}

template<class T>
ArbreAVL<T>::~ArbreAVL() {
  vider(racine);
}

template<class T>
bool ArbreAVL<T>::contient(const T& element) const {
  // À compléter.
  Noeud* courant = racine;
  while (courant != nullptr) {
    if (element < courant->contenu) {
      courant = courant->gauche;
    } else if (courant->contenu < element) {
      courant = courant->droite;
    } else {
      return true;
    }
  }
  return false;
}

template<class T>
void ArbreAVL<T>::inserer(const T& element) {
  inserer(racine, element);
}

template<class T>
bool ArbreAVL<T>::inserer(Noeud*& noeud, const T& element) {
  if (noeud == nullptr) {
    noeud = new Noeud(element);
    return true;
  }
  if (element < noeud->contenu) {
    if (inserer(noeud->gauche, element)) {
      noeud->equilibre++;
      if (noeud->equilibre == 0) return false;
      if (noeud->equilibre == 1) return true;
      assert(noeud->equilibre == 2);
      if (noeud->gauche->equilibre == -1) rotationDroiteGauche(noeud->gauche);
      rotationGaucheDroite(noeud);
    }
    return false;
  } else if (noeud->contenu < element) {
    // À compléter.
    if (inserer(noeud->droite, element)) {
      noeud->equilibre--;
      if (noeud->equilibre == 0) return false;
      if (noeud->equilibre == -1) return true;
      assert(noeud->equilibre == -2);
      if (noeud->droite->equilibre == 1) rotationGaucheDroite(noeud->droite);
      rotationDroiteGauche(noeud);
    }
    return false;
  }

  // element == noeud->contenu
  noeud->contenu = element;  // Mise à jour
  return false;
}

template<class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud*& racinesousarbre) {
  Noeud* temp = racinesousarbre->gauche;
  int ea = temp->equilibre;
  int eb = racinesousarbre->equilibre;
  int neb = -(ea > 0 ? ea : 0) - 1 + eb;
  int nea = ea + (neb < 0 ? neb : 0) - 1;

  temp->equilibre = nea;
  racinesousarbre->equilibre = neb;
  racinesousarbre->gauche = temp->droite;
  temp->droite = racinesousarbre;
  racinesousarbre = temp;
}

template<class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud*& racinesousarbre) {
  // À compléter.
  // Les démonstrateurs peuvent fournir la formule à mettre ici.
  Noeud* temp = racinesousarbre->droite;
  int ea = temp->equilibre;
  int eb = racinesousarbre->equilibre;
  int neb = -(ea < 0 ? ea : 0) + 1 + eb;
  int nea = ea + (neb > 0 ? neb : 0) + 1;

  temp->equilibre = nea;
  racinesousarbre->equilibre = neb;
  racinesousarbre->droite = temp->gauche;
  temp->gauche = racinesousarbre;
  racinesousarbre = temp;
}

template<class T>
bool ArbreAVL<T>::vide() const {
  // À compléter : doit retourner vrai si et seulement si l'arbre est vide.
  return racine == nullptr;
}

template<class T>
void ArbreAVL<T>::vider() {
  vider(racine);
}

template<class T>
void ArbreAVL<T>::vider(Noeud*& noeud) {
  // À compléter.
  if (noeud != nullptr) {
    vider(noeud->gauche);
    vider(noeud->droite);
    delete noeud;
    noeud = nullptr;
  }
}

template<class T>
void ArbreAVL<T>::copier(const Noeud* source, Noeud*& noeud) const {
  // À compléter.
  if (source == nullptr) {
    noeud = nullptr;
  } else {
    noeud = new Noeud(source->contenu);
    noeud->equilibre = source->equilibre;
    copier(source->gauche, noeud->gauche);
    copier(source->droite, noeud->droite);
  }
}

template<class T>
int ArbreAVL<T>::hauteur() const {
  return hauteur(racine);
}

template<class T>
int ArbreAVL<T>::compter() const {
  return compter(racine);
}

template<class T>
int ArbreAVL<T>::hauteur(const Noeud* n) const {
  if (n == nullptr) return -1;
  return 1 + std::max(hauteur(n->gauche), hauteur(n->droite));
}

template<class T>
int ArbreAVL<T>::compter(const Noeud* n) const {
  if (n == nullptr) return 0;
  return compter(n->gauche) + compter(n->droite) + 1;
}

template<class T>
ArbreAVL<T>& ArbreAVL<T>::operator=(const ArbreAVL& autre) {
  if (this == &autre) return *this;
  vider();
  copier(autre.racine, racine);
  return *this;
}

template<class T>
void ArbreAVL<T>::enlever(const T& element) {
  enlever(racine, element);
}

template <class T>
bool ArbreAVL<T>::enlever(Noeud *&noeud, const T &element)
{
    if (noeud == nullptr) return false;

    if (element < noeud->contenu)
    {
        if (enlever(noeud->gauche, element))
        {
            noeud->equilibre--;
            return true;
        }
    }
    else if (element > noeud->contenu)
    {
        if (enlever(noeud->droite, element))
        {
            noeud->equilibre++;
            return true;
        }
    }
    else
    {
        if (noeud->gauche == nullptr && noeud->droite == nullptr)
        {
            delete noeud;
            noeud = nullptr;
            return true;
        }
        else if (noeud->gauche == nullptr)
        {
            Noeud *temp = noeud;
            noeud = noeud->droite;
            delete temp;
            return true;
        }
        else if (noeud->droite == nullptr)
        {
            Noeud *temp = noeud;
            noeud = noeud->gauche;
            delete temp;
            return true;
        }
        else
        {
            Noeud *successeur = noeud->droite;
            while (successeur->gauche != nullptr)
                successeur = successeur->gauche;
            noeud->contenu = successeur->contenu;
            if (enlever(noeud->droite, successeur->contenu))
            {
                noeud->equilibre++;
                return true;
            }
        }
    }
    return false;
}


// Code fourni pour afficher l'arbre.
// L'opérateur ostream& << const T&  doit être défini.
#include <iostream>
template<class T>
void ArbreAVL<T>::afficher() const {
  std::cout << "Contenu de l'arbre AVL (";
  int n = compter();
  std::cout << n << " noeuds) :\n";

  T* elements = new T[n];
  int* profondeurs = new int[n];
  n = 0;
  preparer_afficher(racine, 0, n, elements, profondeurs);
  for (int p = 0;; p++) {
    bool derniereprofondeur = true;
    for (int i = 0; i < n; i++) {
      if (profondeurs[i] == p) {
        std::cout << elements[i];
        derniereprofondeur = false;
      }
      std::cout << '\t';
    }
    std::cout << '\n';
    if (derniereprofondeur) break;
  }

  delete[] elements;
  delete[] profondeurs;
  std::cout << "--------------" << std::endl;
}

template<class T>
void ArbreAVL<T>::preparer_afficher(const Noeud* n, int profondeur, int& rang,
                                    T* elements, int* profondeurs) const {
  if (n == nullptr) return;
  preparer_afficher(n->gauche, profondeur + 1, rang, elements, profondeurs);
  elements[rang] = n->contenu;
  profondeurs[rang] = profondeur;
  rang++;
  preparer_afficher(n->droite, profondeur + 1, rang, elements, profondeurs);
}

template<class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const {
  Iterateur iter(*this);
  // À compléter.
  iter.courant = racine;
  if (iter.courant != nullptr) {
    while (iter.courant->gauche != nullptr) {
      iter.chemin.empiler(iter.courant);
      iter.courant = iter.courant->gauche;
    }
  }
  return iter;
}

template<class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::fin() const {
  return Iterateur(*this);
}

template<class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercher(const T& e) const {
  // À compléter.
  Iterateur iter(*this);
  iter.courant = racine;
  while (iter.courant != nullptr) {
    if (e < iter.courant->contenu) {
      iter.chemin.empiler(iter.courant);
      iter.courant = iter.courant->gauche;
    } else if (iter.courant->contenu < e) {
      iter.chemin.empiler(iter.courant);
      iter.courant = iter.courant->droite;
    } else {
      return iter;
    }
  }
  return fin();
}

template<class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuSuivant(
    const T& e) const {
  Iterateur iter(*this);
  // À compléter.
   iter.courant = racine;
  Noeud* dernier = nullptr;
  while (iter.courant != nullptr) {
    if (e < iter.courant->contenu) {
      dernier = iter.courant;
      iter.chemin.empiler(iter.courant);
      iter.courant = iter.courant->gauche;
    } else if (iter.courant->contenu < e) {
      iter.chemin.empiler(iter.courant);
      iter.courant = iter.courant->droite;
    } else {
      return iter;
    }
  }
  if (dernier != nullptr) {
    iter.courant = dernier;
    return iter;
  }
  return fin();
}

template<class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::rechercherEgalOuPrecedent(
    const T& e) const {
  // À compléter.
  Iterateur iter(*this);
  iter.courant = racine;
  Noeud* dernier = nullptr;
  while (iter.courant != nullptr) {
    if (e < iter.courant->contenu) {
      iter.chemin.empiler(iter.courant);
      iter.courant = iter.courant->gauche;
    } else if (iter.courant->contenu < e) {
      dernier = iter.courant;
      iter.chemin.empiler(iter.courant);
      iter.courant = iter.courant->droite;
    } else {
      return iter;
    }
  }
  if (dernier != nullptr) {
    iter.courant = dernier;
    return iter;
  }
  return fin();
}

template<class T>
const T& ArbreAVL<T>::operator[](const Iterateur& iterateur) const {
  assert(&iterateur.arbre_associe == this);
  assert(iterateur.courant);
  return iterateur.courant->contenu;
}

template<class T>
T& ArbreAVL<T>::operator[](const Iterateur& iterateur) {
  assert(&iterateur.arbre_associe == this);
  assert(iterateur.courant);
  return iterateur.courant->contenu;
}

// Retourne le i ième élément du parcours inordre de l'arbre.
template<class T>
const T& ArbreAVL<T>::operator[](int index) const {
    assert(index >= 0 && index < compter());
    return elementAt(racine, index);
}


template<class T>
T& ArbreAVL<T>::operator[](int index) {
    assert(index >= 0 && index < compter());
    return elementAt(racine, index);
}

template<class T>
const T& ArbreAVL<T>::elementAt(Noeud* noeud, int index) const {
    assert(noeud != nullptr);
    int gaucheCount = noeud->gauche ? compter(noeud->gauche) : 0;

    if (index < gaucheCount) {
        return elementAt(noeud->gauche, index);
    } else if (index > gaucheCount) {
        return elementAt(noeud->droite, index - gaucheCount - 1);
    } else {
        return noeud->contenu;
    }
}

template<class T>
T& ArbreAVL<T>::elementAt(Noeud* noeud, int index) {
    assert(noeud != nullptr);
    int gaucheCount = noeud->gauche ? compter(noeud->gauche) : 0;

    if (index < gaucheCount) {
        return elementAt(noeud->gauche, index);
    } else if (index > gaucheCount) {
        return elementAt(noeud->droite, index - gaucheCount - 1);
    } else {
        return noeud->contenu;
    }
}

template<class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL& a)
    : arbre_associe(a), courant(nullptr) {}

template<class T>
ArbreAVL<T>::Iterateur::Iterateur(const ArbreAVL<T>::Iterateur& a)
    : arbre_associe(a.arbre_associe) {
  courant = a.courant;
  chemin = a.chemin;
}

// Pré-incrément
template<class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator++() {
  // À compléter.
   if (courant->droite != nullptr) {
    chemin.empiler(courant);
    courant = courant->droite;
    while (courant->gauche != nullptr) {
      chemin.empiler(courant);
      courant = courant->gauche;
    }
  } else {
    Noeud* prec = nullptr;
    do {
      prec = courant;
      if (!chemin.vide()) {
        chemin.depiler(courant);
      } else {
        courant = nullptr;
        break;
      }
    } while (courant != nullptr && courant->droite == prec);
  }
  return *this;
}

// Post-incrément
template<class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::Iterateur::operator++(int) {
  Iterateur copie(*this);
  operator++();
  return copie;
}

template<class T>
ArbreAVL<T>::Iterateur::operator bool() const {
  return courant != nullptr;
}

template<class T>
bool ArbreAVL<T>::Iterateur::operator!() const {
  return courant == nullptr;
}

template<class T>
bool ArbreAVL<T>::Iterateur::operator==(const Iterateur& o) const {
  assert(&arbre_associe == &o.arbre_associe);
  return courant == o.courant;
}

template<class T>
bool ArbreAVL<T>::Iterateur::operator!=(const Iterateur& o) const {
  assert(&arbre_associe == &o.arbre_associe);
  return courant != o.courant;
}

template<class T>
const T& ArbreAVL<T>::Iterateur::operator*() const {
  assert(courant != nullptr);
  return courant->contenu;
}

template<class T>
typename ArbreAVL<T>::Iterateur& ArbreAVL<T>::Iterateur::operator=(
    const Iterateur& autre) {
  assert(&arbre_associe == &autre.arbre_associe);
  courant = autre.courant;
  chemin = autre.chemin;
  return *this;
}


 // Retourne un nouvel arbre AVL contenant les éléments inverses de l'arbre.
    // Ex.: si l'arbre contient 1, -2 et 3, le nouvel arbre doit contenir -1, 2 et -3.
    // On suppose que l'opérateur - est défini pour le type T.
template<class T>
ArbreAVL<T> ArbreAVL<T>::arbreInverse() const {
   ArbreAVL<T> arbreInverse;

  std::function<Noeud*(Noeud*)> inverserRecursif = [&](Noeud* noeudOriginal) -> Noeud* {
    if (noeudOriginal == nullptr) return nullptr;

    Noeud* noeudInverse = new Noeud(-noeudOriginal->contenu);
    noeudInverse->gauche = inverserRecursif(noeudOriginal->droite);
    noeudInverse->droite = inverserRecursif(noeudOriginal->gauche);
    return noeudInverse;
  };

  arbreInverse.racine = inverserRecursif(racine);
  return arbreInverse;
}



  // Retourne un nouvel arbre AVL représentant la différence symétrique
    // entre l'arbre courant et l'arbre passé en paramètre.
    // La différence symétrique est l'ensemble des éléments qui sont
    // dans un des deux arbres, mais pas dans les deux.
template<class T>
ArbreAVL<T> ArbreAVL<T>::differenceSymetrique(const ArbreAVL& autre) const {
    ArbreAVL<T> resultat;

    Iterateur it1 = debut();
    Iterateur it2 = autre.debut();

    while (it1 && it2) {
        if (*it1 < *it2) {
            resultat.inserer(*it1);
            ++it1;
        } else if (*it2 < *it1) {
            resultat.inserer(*it2);
            ++it2;
        } else {
            ++it1;
            ++it2;
        }
    }

    while (it1) {
        resultat.inserer(*it1);
        ++it1;
    }

    while (it2) {
        resultat.inserer(*it2);
        ++it2;
    }

    return resultat;
}





#endif
