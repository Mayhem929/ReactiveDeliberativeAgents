#include "../Comportamientos_Jugador/aldeano.hpp"
#include "motorlib/util.h"

Action ComportamientoAldeano::think(Sensores sensores) {
  Action accion = actIDLE;

/*  // Aldeano basico
  switch (paso){
      case 0: accion = actFORWARD;
              break;
      case 1: accion = actTURN_R;
	      break;
  }
  paso = (paso+1)%2;*/

  // Aldeano comun
  int cual = aleatorio(20);
  switch (cual) {
	case 0: accion = actIDLE; break;
	case 1: accion = actTURN_L; break;
	case 2: accion = actTURN_R; break;
  case 3: accion = actTURN_SL; break;
  case 4: accion = actTURN_SR; break;
	default: 
		if (sensores.terreno[2] != 'P' and sensores.terreno[2] != 'M' and sensores.superficie[2] == '_') accion = actFORWARD;
    else accion = actTURN_SL;
		break;
  }

  return accion;
}

int ComportamientoAldeano::interact(Action accion, int valor) {
    return 0;
}
