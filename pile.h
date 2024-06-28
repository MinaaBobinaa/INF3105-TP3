/*  INF3105 - Structures de données et algorithmes
    UQAM / Département d'informatique
    Pile - Solution
*/

#ifndef __PILE_H__
#define __PILE_H__

template<class T>
class Pile {
 public:
  Pile();
  Pile(const Pile&);
  ~Pile();

  void empiler(const T&);

  void depiler(T& out);  // Depiler et copier le sommet dans out.
  T depiler();

  bool vide() const { return sommet == nullptr; }
  void vider();

  const Pile<T>& operator=(const Pile<T>&);

 private:
  class Cellule {
   public:
    Cellule(const T& e, Cellule* c);
    T contenu;
    Cellule* suivante;
  };

  Cellule* sommet;
};

/*
 Puisque Pile<T> est une classe générique, toutes ses définitions doivent être
 inclues après les déclarations. On peut quand même séparer les déclarations et
 les définitions en deux fichiers (pile.h et pile.hcc), à condition d'inclure le
 deuxième (pile.hcc) à la fin du premier (pile.h). Ainsi, un fichier source
 (.cc, .cpp, c++) qui inclut pile.h inclura indirectement aussi pile.hcc.
*/

// #include "pile.hcc"
/**** Début des définitions pouvant être mises dans pile.hcc. ****/

#include "assert.h"

template<class T>
Pile<T>::Pile() {
  sommet = nullptr;
}

template<class T>
Pile<T>::~Pile() {
  vider();
}

template<class T>
Pile<T>::Pile(const Pile<T>& autre) : sommet(nullptr) {
  Cellule** t = &sommet;
  for (Cellule* c = autre.sommet; c != nullptr;
       c = c->suivante, t = &((*t)->suivante))
    *t = new Cellule(c->contenu, *t);
}

template<class T>
void Pile<T>::vider() {
  while (!vide()) depiler();
}

template<class T>
Pile<T>::Cellule::Cellule(const T& e, Cellule* c) : suivante(c) {
  contenu = e;
}

template<class T>
void Pile<T>::empiler(const T& e) {
  sommet = new Cellule(e, sommet);
  assert(sommet);
}

template<class T>
T Pile<T>::depiler() {
  assert(sommet);
  Cellule c(*sommet);
  delete sommet;
  sommet = c.suivante;
  return c.contenu;
}

template<class T>
void Pile<T>::depiler(T& e) {
  assert(sommet != nullptr);
  e = sommet->contenu;
  Cellule* c = sommet;
  sommet = sommet->suivante;
  delete c;
}

template<class T>
const Pile<T>& Pile<T>::operator=(const Pile<T>& autre) {
  if (&autre == this) return *this;

  Cellule** t = &sommet;
  Cellule* a = autre.sommet;
  while (a) {
    if ((*t) == nullptr)
      *t = new Cellule(a->contenu, nullptr);
    else
      (*t)->contenu = a->contenu;
    t = &((*t)->suivante);
    a = a->suivante;
  }
  a = *t;
  *t = nullptr;
  while (a != nullptr) {
    Cellule* temp = a->suivante;
    delete a;
    a = temp;
  }

  return *this;
}

#endif
