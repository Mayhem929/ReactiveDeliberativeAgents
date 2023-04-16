#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

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


/** Definición del tipo nodo para el nivel 0*/
class nodeN0 {
  public:
    stateN0 st;
    Action accion;
    nodeN0 *padre;

    nodeN0(){
      padre = nullptr;
    }

    nodeN0(const nodeN0& n) {
      st = n.st;
      accion = n.accion;
      padre = n.padre;
    }

    ~nodeN0() {
      padre = nullptr;
      delete padre;
    }

    nodeN0(nodeN0* n) {
      st = n->st;
      accion = n->accion;
      padre = n->padre;
    }

    void operator=(const nodeN0 &n) {
      st = n.st;
      accion = n.accion;
      padre = n.padre;
    }
    
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
    list<Action> secuencia;

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
      goal.f = size;
      goal.c = size;
      hayPlan = false;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      hayPlan = false;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);


  private:
    // Declarar Variables de Estado

    list<Action> plan;    // Almacena el plan de ejecución
    bool hayPlan;         // Si true indica que se está siguiendo un plan
    ubicacion goal;
    stateN1 c_state;
    // Funciones privadas

    void VisualizaPlan(const stateN0 &st, const list<Action> &plan);

};

#endif