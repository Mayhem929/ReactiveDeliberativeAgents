#include "../Comportamientos_Jugador/sonambulo.hpp"
#include "motorlib/util.h"

Action ComportamientoSonambulo::think(Sensores sensores) {  
  return sensores.ActionSent;
}

int ComportamientoSonambulo::interact(Action accion, int valor) {
    return 0;
}
