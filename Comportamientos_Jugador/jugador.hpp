#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <queue>
#include <memory>
#include <limits>

class stateN0 {
  public:
    ubicacion jugador;
    ubicacion sonambulo;

    bool operator== (const stateN0 &x) const {
      return (jugador==x.jugador && sonambulo.f==x.sonambulo.f && 
              sonambulo.c==x.sonambulo.c);
    }
};

class stateN1 : public stateN0 {
  public:
    bool operator== (const stateN1 &x) const {
      return (jugador==x.jugador && sonambulo==x.sonambulo);
    }
};

class stateN2 : public stateN1 {
  public:
    bool bikini;
    bool zapatillas;
    int coste;

    bool operator== (const stateN2 &x) const {
      return (jugador==x.jugador && sonambulo==x.sonambulo);
    }
};

class stateN3 : public stateN2 {
  public:
    bool bikini_son;
    bool zapatillas_son;
    int heuristica;
    int suma;

    bool operator== (const stateN3 &x) const {
      return (jugador==x.jugador && sonambulo==x.sonambulo);
    }
};

/** Definición del tipo nodo para el nivel 0*/
class nodeN0 {
  public:
    stateN0 st;
    Action accion;
    shared_ptr<nodeN0> padre;
    
    bool operator==(const nodeN0 &n) const {
      return (st==n.st);
    }

    bool operator<(const nodeN0 &n) const {
      if ((st.jugador.f<n.st.jugador.f)|| 
          (st.jugador.f==n.st.jugador.f && st.jugador.c<n.st.jugador.c) ||
          (st.jugador.f==n.st.jugador.f && st.jugador.c==n.st.jugador.c
                  && st.jugador.brujula<n.st.jugador.brujula)) {
        return true;
      } else
        return false;
    }

    bool operator<(nodeN0* n) const {
      if ((st.jugador.f<n->st.jugador.f)|| 
          (st.jugador.f==n->st.jugador.f && st.jugador.c<n->st.jugador.c) ||
          (st.jugador.f==n->st.jugador.f && st.jugador.c==n->st.jugador.c
                  && st.jugador.brujula<n->st.jugador.brujula)) {
        return true;
      } else
        return false;
    }
};

/** Definición del tipo nodo para el nivel 1*/
class nodeN1{
  public:
    stateN1 st;
    Action accion;
    shared_ptr<nodeN1> padre;

    bool operator==(const nodeN1 &n) const {
      return (st==n.st);
    }

    bool operator<(const nodeN1 &n) const {
      if (((st.jugador.f<n.st.jugador.f)|| 
          (st.jugador.f==n.st.jugador.f && st.jugador.c<n.st.jugador.c) ||
          (st.jugador.f==n.st.jugador.f && st.jugador.c==n.st.jugador.c
                  && st.jugador.brujula<n.st.jugador.brujula)) ||
          (st.jugador == n.st.jugador &&
          ((st.sonambulo.f<n.st.sonambulo.f)||
          (st.sonambulo.f==n.st.sonambulo.f && st.sonambulo.c<n.st.sonambulo.c) ||
          (st.sonambulo.f==n.st.sonambulo.f && st.sonambulo.c==n.st.sonambulo.c
                  && st.sonambulo.brujula<n.st.sonambulo.brujula)))) {
        return true;
      } else
        return false;
    }
};

/** Definición del tipo nodo para el nivel 2*/
class nodeN2{
  public:
    stateN2 st;
    Action accion;
    shared_ptr<nodeN2> padre;

    bool operator==(const nodeN2 &n) const {
      return (st==n.st);
    }

    bool peor_que(const nodeN2 &n) const {
      if (st.coste > n.st.coste) 
        return true;
      else if (st.coste == n.st.coste && st.jugador.f<n.st.jugador.f)
        return true;
      else if (st.coste == n.st.coste && st.jugador.f==n.st.jugador.f && st.jugador.c<n.st.jugador.c)
        return true;
      else if (st.coste == n.st.coste && st.jugador.f==n.st.jugador.f && st.jugador.c==n.st.jugador.c
                  && st.jugador.brujula<n.st.jugador.brujula)
        return true;
      else if (st.coste == n.st.coste && st.jugador == n.st.jugador && st.sonambulo.f<n.st.sonambulo.f)
        return true;
      else if (st.coste == n.st.coste && st.jugador == n.st.jugador && st.sonambulo.f==n.st.sonambulo.f
                  && st.sonambulo.c<n.st.sonambulo.c)
        return true;
      else if (st.coste == n.st.coste && st.jugador == n.st.jugador && st.sonambulo.f==n.st.sonambulo.f
                  && st.sonambulo.c==n.st.sonambulo.c && st.sonambulo.brujula<n.st.sonambulo.brujula)
        return true;
      else if (st.coste == n.st.coste && st.jugador == n.st.jugador && st.sonambulo==n.st.sonambulo
                  && st.zapatillas<n.st.zapatillas)
        return true;
      else if (st.coste == n.st.coste && st.jugador == n.st.jugador && st.sonambulo==n.st.sonambulo
                  && st.zapatillas==n.st.zapatillas && st.bikini<n.st.bikini)
        return true;
      else
        return false;
    }

    // Decidir si un nodo es mejor que otro
    bool operator<(const nodeN2 &n) const {
      if (st.jugador.f<n.st.jugador.f)
        return true;
      else if (st.jugador.f==n.st.jugador.f && st.jugador.c<n.st.jugador.c)
        return true;
      else if (st.jugador.f==n.st.jugador.f && st.jugador.c==n.st.jugador.c
                  && st.jugador.brujula<n.st.jugador.brujula)
        return true;
      else if (st.jugador == n.st.jugador && st.sonambulo.f<n.st.sonambulo.f)
        return true;
      else if (st.jugador == n.st.jugador && st.sonambulo.f==n.st.sonambulo.f
                  && st.sonambulo.c<n.st.sonambulo.c)
        return true;
      else if (st.jugador == n.st.jugador && st.sonambulo.f==n.st.sonambulo.f
                  && st.sonambulo.c==n.st.sonambulo.c && st.sonambulo.brujula<n.st.sonambulo.brujula)
        return true;
      else if (st.jugador == n.st.jugador && st.sonambulo==n.st.sonambulo
                  && st.zapatillas<n.st.zapatillas)
        return true;
      else if (st.jugador == n.st.jugador && st.sonambulo==n.st.sonambulo
                  && st.zapatillas==n.st.zapatillas && st.bikini<n.st.bikini)
        return true;
      else
        return false;
    }
};

/** Definición del tipo nodo para el nivel 3*/
class nodeN3{
  public:
    stateN3 st;
    Action accion;
    shared_ptr<nodeN3> padre;

    bool operator==(const nodeN3 &n) const {
      return (st==n.st);
    }

    // Decidir si un nodo es mejor que otro
    bool peor_que(const nodeN3 &n) const {
      if (st.suma > n.st.suma) 
        return true;
      else if (st.suma == n.st.suma && st.jugador.f<n.st.jugador.f)
        return true;
      else if (st.suma == n.st.suma && st.jugador.f==n.st.jugador.f && st.jugador.c<n.st.jugador.c)
        return true;
      else if (st.suma == n.st.suma && st.jugador.f==n.st.jugador.f && st.jugador.c==n.st.jugador.c
                  && st.jugador.brujula<n.st.jugador.brujula)
        return true;
      else if (st.suma == n.st.suma && st.jugador == n.st.jugador && st.sonambulo.f<n.st.sonambulo.f)
        return true;
      else if (st.suma == n.st.suma && st.jugador == n.st.jugador && st.sonambulo.f==n.st.sonambulo.f
                  && st.sonambulo.c<n.st.sonambulo.c)
        return true;
      else if (st.suma == n.st.suma && st.jugador == n.st.jugador && st.sonambulo.f==n.st.sonambulo.f
                  && st.sonambulo.c==n.st.sonambulo.c && st.sonambulo.brujula<n.st.sonambulo.brujula)
        return true;
      else if (st.suma == n.st.suma && st.jugador == n.st.jugador && st.sonambulo==n.st.sonambulo
                  && st.zapatillas<n.st.zapatillas)
        return true;
      else if (st.suma == n.st.suma && st.jugador == n.st.jugador && st.sonambulo==n.st.sonambulo
                  && st.zapatillas==n.st.zapatillas && st.bikini<n.st.bikini)
        return true;
      else
        return false;
    }

    bool operator<(const nodeN3 &n) const {
      if (st.jugador.f<n.st.jugador.f)
        return true;
      else if (st.jugador.f==n.st.jugador.f && st.jugador.c<n.st.jugador.c)
        return true;
      else if (st.jugador.f==n.st.jugador.f && st.jugador.c==n.st.jugador.c
                  && st.jugador.brujula<n.st.jugador.brujula)
        return true;
      else if (st.jugador == n.st.jugador && st.sonambulo.f<n.st.sonambulo.f)
        return true;
      else if (st.jugador == n.st.jugador && st.sonambulo.f==n.st.sonambulo.f
                  && st.sonambulo.c<n.st.sonambulo.c)
        return true;
      else if (st.jugador == n.st.jugador && st.sonambulo.f==n.st.sonambulo.f
                  && st.sonambulo.c==n.st.sonambulo.c && st.sonambulo.brujula<n.st.sonambulo.brujula)
        return true;
      else if (st.jugador == n.st.jugador && st.sonambulo==n.st.sonambulo
                  && st.zapatillas<n.st.zapatillas)
        return true;
      else if (st.jugador == n.st.jugador && st.sonambulo==n.st.sonambulo
                  && st.zapatillas==n.st.zapatillas && st.bikini<n.st.bikini)
        return true;
      else if (st.jugador == n.st.jugador && st.sonambulo==n.st.sonambulo
                  && st.zapatillas==n.st.zapatillas && st.bikini==n.st.bikini
                  && st.zapatillas_son < n.st.zapatillas_son)
        return true;
      else if (st.jugador == n.st.jugador && st.sonambulo==n.st.sonambulo
                  && st.zapatillas==n.st.zapatillas && st.bikini==n.st.bikini
                  && st.zapatillas_son == n.st.zapatillas_son && st.bikini_son < n.st.bikini_son)
        return true;
      else
        return false;
    }
};

class CompareN2 {
public:
    bool operator()(nodeN2 below, nodeN2 above)
    {
        if (below.peor_que(above)) {
            return true;
        }
        else if (above.peor_que(below))
            return false;
        else
            return false;
    }
};


class CompareN3 {
public:
    bool operator()(nodeN3 below, nodeN3 above)
    {
        if (below.peor_que(above)) {
            return true;
        }
        else if (above.peor_que(below))
            return false;
        else
            return false;
    }
};


class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      c_state.jugador.f = size;
      c_state.jugador.c = size;
      c_state.jugador.brujula = norte;
      c_state.sonambulo.f = size;
      c_state.sonambulo.c = size;
      c_state.sonambulo.brujula = norte;
      c_state.coste = 0;
      c_state.bikini = false;
      c_state.zapatillas = false;
      c_state.bikini_son = false;
      c_state.zapatillas_son = false;

      ultima_accion = actIDLE;
      bien_situado = false;
      situando = false;

      goal.f = size;
      goal.c = size;
      hayPlan = false;
      stop = false;

      recargando = false;
      necesita_recarga = false;
      hay_recarga = false;

      planRecarga = false;
      planExplorar = false;
      this->size = size;
      vecesWhereIs = 0;
      radio_son = 45;
      pinta_precipicios(mapaResultado, size);

    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      hayPlan = false;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    bool HayRecarga();

  private:
    // Declarar Variables de Estado

    list<Action> plan;    // Almacena el plan de ejecución
    bool hayPlan;         // Si true indica que se está siguiendo un plan
    ubicacion goal;
    stateN3 c_state;
    bool bien_situado;
    bool situando;
    bool stop;
    bool recargando, necesita_recarga, hay_recarga;
    bool plan_completo;
    bool planJug;
    bool planExplorar;
    bool planRecarga;
    int size;
    int radio_son;
    int vecesWhereIs;
    Action ultima_accion;
    vector<unsigned char> terreno_anterior;
    // Funciones privadas

    void VisualizaPlan(const stateN0 &st, const list<Action> &plan);
    void pinta_precipicios(vector<vector<unsigned char>> &mapa, unsigned int size){
      for(int i=0; i < size; i++){
        for(int j=0; j < size; j++){
          if ( i < 3 or j < 3 or (i > size - 4) or (j > size - 4))
            mapa [i][j] = 'P';
        }
      }
    }

};

#endif