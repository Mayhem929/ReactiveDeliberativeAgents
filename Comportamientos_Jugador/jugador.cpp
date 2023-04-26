#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include<unistd.h>

using namespace std;

// Inicializa c_state con los valores adecuados según el nivel
void Initialize(const Sensores &sensores, stateN0 &c_state) {
	c_state.jugador.f = sensores.posF;
	c_state.jugador.c = sensores.posC;
	c_state.jugador.brujula = sensores.sentido;
	c_state.sonambulo.f = sensores.SONposF;
	c_state.sonambulo.c = sensores.SONposC;
	c_state.sonambulo.brujula = sensores.SONsentido;
}

bool SonambuloEnVision(stateN0 st) {
	
	int i = st.jugador.f; int j = st.jugador.c;
	int f = st.sonambulo.f; int c = st.sonambulo.c;
	int add;
	int radio = 4;
	Orientacion o = st.jugador.brujula;

	switch(o){
		case este:	 	
		case sur: 		add= 1;		break;
		case norte: 	
		case oeste: 	add=-1;		break;
	}

	if(o == norte or o == sur){
		for (int k=0; k < radio; k++){
			for(int l=k*k; l < (k+1)*(k+1); l++){
				if (i == f && j == c) return true;
				j-=add;
			}
			j+= 2*(k+1)*add;
			i+=add;
		}
	}

	else if(o == este or o == oeste){
		for (int k=0; k < radio; k++){
			for(int l=k*k; l < (k+1)*(k+1); l++){
				if(i == f && j == c) return true;
				i+=add;
			}
			i-= 2*(k+1)*add;
			j+=add;
		}
	}

	return false;
}

/** Devuelve si una ubicación en el mapa es transitable para el agente */
bool CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char>> &mapa)
{
	return (mapa[x.f][x.c] != 'P' and mapa[x.f][x.c] != 'M');
}

/** Devuelve la ubicación siguiente según el avance del agente*/
ubicacion NextCasilla(const ubicacion &pos)
{
	ubicacion next = pos;
	switch (pos.brujula)
	{
	case norte:
		next.f = pos.f - 1;
		break;
	case noreste:
		next.f = pos.f - 1;
		next.c = pos.c + 1;
		break;
	case este:
		next.c = pos.c + 1;
		break;
	case sureste:
		next.f = pos.f + 1;
		next.c = pos.c + 1;
		break;
	case sur:
		next.f = pos.f + 1;
		break;
	case suroeste:
		next.f = pos.f + 1;
		next.c = pos.c - 1;
		break;
	case oeste:
		next.c = pos.c - 1;
		break;
	case noroeste:
		next.f = pos.f - 1;
		next.c = pos.c - 1;
		break;
	default:
		break;
	}

	return next;
}

/** Devuelve el estado que se genera si el agente puede avanzar.
 * Si no puede avanzar, se devuelve como salida el mismo estado de entrada.
 */
stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char>> &mapa)
{
  stateN0 st_result = st;
  ubicacion sig_ubicacion;
  switch (a){
  case actFORWARD: // si casilla delante es transitable y no está ocupada por el sonámbulo
	sig_ubicacion = NextCasilla(st.jugador);
	if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.sonambulo.f && sig_ubicacion.c == st.sonambulo.c)){
	  st_result.jugador = sig_ubicacion;
	}
	break;
  case actTURN_L:
	st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6) % 8);
	break;
  case actTURN_R:
	st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 2) % 8);
	break;
  case actSON_FORWARD: // si casilla delante es transitable y no está ocupada por el jugador
	sig_ubicacion = NextCasilla(st.sonambulo);
	if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.jugador.f && sig_ubicacion.c == st.jugador.c)){
	  st_result.sonambulo = sig_ubicacion;
	}
	break;
  case actSON_TURN_SL:
	st_result.sonambulo.brujula = static_cast<Orientacion>((st_result.sonambulo.brujula + 7) % 8);
	break;
  case actSON_TURN_SR:
	st_result.sonambulo.brujula = static_cast<Orientacion>((st_result.sonambulo.brujula + 1) % 8);
	break;
  }
  return st_result;
}

/** Devuelve el estado que se genera si el agente puede avanzar.
 * Si no puede avanzar, se devuelve como salida el mismo estado de entrada.
 */
stateN1 apply(const Action &a, const stateN1 &st, const vector<vector<unsigned char>> &mapa)
{
  stateN1 st_result = st;
  ubicacion sig_ubicacion;
  switch (a){
  case actFORWARD: // si casilla delante es transitable y no está ocupada por el sonámbulo
	sig_ubicacion = NextCasilla(st.jugador);
	if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.sonambulo.f && sig_ubicacion.c == st.sonambulo.c)){
	  st_result.jugador = sig_ubicacion;
	}
	break;
  case actTURN_L:
	st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6) % 8);
	break;
  case actTURN_R:
	st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 2) % 8);
	break;
  case actSON_FORWARD: // si casilla delante es transitable y no está ocupada por el jugador
	sig_ubicacion = NextCasilla(st.sonambulo);
	if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.jugador.f && sig_ubicacion.c == st.jugador.c)){
	  st_result.sonambulo = sig_ubicacion;
	}
	break;
  case actSON_TURN_SL:
	st_result.sonambulo.brujula = static_cast<Orientacion>((st_result.sonambulo.brujula + 7) % 8);
	break;

  case actSON_TURN_SR:
	st_result.sonambulo.brujula = static_cast<Orientacion>((st_result.sonambulo.brujula + 1) % 8);
	break;
  }
  return st_result;
}



int coste(const stateN2 &st, const Action &act, const vector<vector<unsigned char>> &mapa){
	
	switch (mapa[st.jugador.f][st.jugador.c]){
		case 'A':
			if(act == actFORWARD)
				if(st.bikini)
					return 10;
				else
					return 100;
			else if(act == actTURN_L || act == actTURN_R)
				if(st.bikini)
					return 5;
				else
					return 25;
		case 'B':
			if(act == actFORWARD)
				if(st.zapatillas)
					return 15;
				else
					return 50;
			else if(act == actTURN_L || act == actTURN_R)
				if(st.zapatillas)
					return 1;
				else
					return 5;
		case 'T':
			return 2;
			break;
		default:
			return 1;
			break;
	}
}

int coste(const stateN3 &st, const Action &act, const vector<vector<unsigned char>> &mapa){
	
	if(act == actSON_FORWARD || act == actSON_TURN_SL || act == actSON_TURN_SR){	
		switch (mapa[st.sonambulo.f][st.sonambulo.c])
		{
		case 'A':
			if(act == actSON_FORWARD)
				if(st.bikini_son)
					return 10;
				else
					return 100;
			else if(act == actSON_TURN_SL || act == actSON_TURN_SR)
				if(st.bikini_son)
					return 2;
				else
					return 7;
			break;
		case 'B':
			if(act == actSON_FORWARD)
				if(st.zapatillas_son)
					return 15;
				else
					return 50;
			else if(act == actSON_TURN_SL || act == actSON_TURN_SR)
				if(st.zapatillas_son)
					return 1;
				else
					return 3;
			break;
		case 'T':
			if(act == actSON_FORWARD)
				return 2;
			else
				return 1;
			break;
		default:
			return 1;
			break;
		}
	}
	else switch (mapa[st.jugador.f][st.jugador.c]){
		case 'A':
			if(act == actFORWARD)
				if(st.bikini)
					return 10;
				else
					return 100;
			else if(act == actTURN_L || act == actTURN_R)
				if(st.bikini)
					return 5;
				else
					return 25;
		case 'B':
			if(act == actFORWARD)
				if(st.zapatillas)
					return 15;
				else
					return 50;
			else if(act == actTURN_L || act == actTURN_R)
				if(st.zapatillas)
					return 1;
				else
					return 5;
		case 'T':
			return 2;
			break;
		default:
			return 1;
			break;
	}
}
/** Devuelve el estado que se genera si el agente puede avanzar.
 * Si no puede avanzar, se devuelve como salida el mismo estado de entrada.
 */
stateN2 apply(const Action &a, const stateN2 &st, const vector<vector<unsigned char>> &mapa)
{
  stateN2 st_result = st;
  ubicacion sig_ubicacion;

  if(a != actFORWARD)
	st_result.coste += coste(st_result, a, mapa);

  switch (a){
  case actFORWARD: // si casilla delante es transitable y no está ocupada por el sonámbulo
	sig_ubicacion = NextCasilla(st.jugador);
	if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.sonambulo.f && sig_ubicacion.c == st.sonambulo.c)){
	  st_result.coste += coste(st_result, actFORWARD, mapa);
	  st_result.jugador = sig_ubicacion;
	}
	if(mapa[st_result.jugador.f][st_result.jugador.c] == 'K'){
		st_result.bikini = true;
		st_result.zapatillas = false;
	}
	if(mapa[st_result.jugador.f][st_result.jugador.c] == 'D'){
		st_result.bikini = false;
		st_result.zapatillas = true;
	}
	break;
  case actTURN_L:
	st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6) % 8);
	break;
  case actTURN_R:
	st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 2) % 8);
	break;
  case actSON_FORWARD: // si casilla delante es transitable y no está ocupada por el jugador
	sig_ubicacion = NextCasilla(st.sonambulo);
	if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.jugador.f && sig_ubicacion.c == st.jugador.c)){
	  st_result.sonambulo = sig_ubicacion;
	}
	break;
  case actSON_TURN_SL:
	st_result.sonambulo.brujula = static_cast<Orientacion>((st_result.sonambulo.brujula + 7) % 8);
	break;

  case actSON_TURN_SR:
	st_result.sonambulo.brujula = static_cast<Orientacion>((st_result.sonambulo.brujula + 1) % 8);
	break;
  }

  return st_result;
}

stateN3 apply(const Action &a, const stateN3 &st, const vector<vector<unsigned char>> &mapa)
{
  stateN3 st_result = st;
  ubicacion sig_ubicacion;

  if(a != actFORWARD and a != actSON_FORWARD)
    st_result.coste += coste(st_result, a, mapa);

  switch (a){
  case actFORWARD: // si casilla delante es transitable y no está ocupada por el sonámbulo
	sig_ubicacion = NextCasilla(st.jugador);
	if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.sonambulo.f && sig_ubicacion.c == st.sonambulo.c)){
	  st_result.coste += coste(st_result, actFORWARD, mapa);
	  st_result.jugador = sig_ubicacion;
	}
	if(mapa[st_result.jugador.f][st_result.jugador.c] == 'K'){
		st_result.bikini = true;
		st_result.zapatillas = false;
	}
	if(mapa[st_result.jugador.f][st_result.jugador.c] == 'D'){
		st_result.bikini = false;
		st_result.zapatillas = true;
	}
	break;
  case actTURN_L:
	st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6) % 8);
	break;
  case actTURN_R:
	st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 2) % 8);
	break;
  case actSON_FORWARD: // si casilla delante es transitable y no está ocupada por el jugador
	sig_ubicacion = NextCasilla(st.sonambulo);
	if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.jugador.f && sig_ubicacion.c == st.jugador.c)){
	  st_result.coste += coste(st_result, actSON_FORWARD, mapa);
	  st_result.sonambulo = sig_ubicacion;
	}
	if(mapa[st_result.sonambulo.f][st_result.sonambulo.c] == 'K'){
		st_result.bikini_son = true;
		st_result.zapatillas_son = false;
	}
	if(mapa[st_result.sonambulo.f][st_result.sonambulo.c] == 'D'){
		st_result.bikini_son = false;
		st_result.zapatillas_son = true;
	}
	break;
  case actSON_TURN_SL:
	st_result.sonambulo.brujula = static_cast<Orientacion>((st_result.sonambulo.brujula + 7) % 8);
	break;

  case actSON_TURN_SR:
	st_result.sonambulo.brujula = static_cast<Orientacion>((st_result.sonambulo.brujula + 1) % 8);
	break;
  }

  return st_result;
}

/** pone a cero todos los elementos de una matriz */
void AnularMatriz(vector<vector<unsigned char>> &matriz)
{
  for (int i = 0; i < matriz.size(); i++)
    for (int j = 0; j < matriz[0].size(); j++)
      matriz[i][j] = 0;
}

/** Permite pintar sobre el mapa del simulador el plan partiendo desde el estado st */
void ComportamientoJugador::VisualizaPlan(const stateN0 &st, const list<Action> &plan)
{
  AnularMatriz(mapaConPlan);
  stateN0 cst = st;

  auto it = plan.begin();
  while (it != plan.end()){
    switch (*it){
      case actFORWARD:
        cst.jugador = NextCasilla(cst.jugador);
	  mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
	  break;
	case actTURN_R:
        cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 2) % 8);
	  break;
	case actTURN_L:
	  cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
	  break;
	case actSON_FORWARD:
	  cst.sonambulo = NextCasilla(cst.sonambulo);
	  mapaConPlan[cst.sonambulo.f][cst.sonambulo.c] = 2;
	  break;
	case actSON_TURN_SR:
	  cst.sonambulo.brujula = (Orientacion)((cst.sonambulo.brujula + 1) % 8);
	  break;
	case actSON_TURN_SL:
	  cst.sonambulo.brujula = (Orientacion)((cst.sonambulo.brujula + 7) % 8);
	  break;
    }
    it++;
  }
}

list<Action> AnchuraSoloJugador(const stateN1 &inicio, const ubicacion &final,
							const vector<vector<unsigned char>> &mapa) {

	nodeN1 current_node;
	list<nodeN1> frontier;
	set<nodeN1> explored;
	list<Action> plan;
	int n = 0;
	current_node.st = inicio;
	current_node.padre = nullptr;
	bool SolutionFound = (current_node.st.jugador.f==final.f &&
							current_node.st.jugador.c==final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() && !SolutionFound) {
		frontier.pop_front();
		explored.insert(current_node);
		bool son_en_vision = SonambuloEnVision(current_node.st);
		n++;
		if(n%10000==0)
			cout << "Nodos explorados: " << n << endl;
		// Generar hijo actFORWARD
		
		nodeN1 child_forward;
		child_forward.padre = make_shared<nodeN1>(current_node);
		child_forward.st = apply(actFORWARD, current_node.st, mapa);
		if (child_forward.st.jugador.f==final.f && child_forward.st.jugador.c==final.c) {
			child_forward.accion = actFORWARD;
			current_node = child_forward;
			SolutionFound = true;
			cout << "Nodos explorados: " << n << endl;
		} else if (explored.find(child_forward)==explored.end()) {
			child_forward.accion = actFORWARD;
			frontier.push_back(child_forward);
		}

		if (!SolutionFound) {
			// Generar hijo actTURN_L
			nodeN1 child_turnl;
			child_turnl.padre = make_shared<nodeN1>(current_node);
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			if (explored.find(child_turnl)==explored.end()){
				child_turnl.accion = actTURN_L;
				frontier.push_back(child_turnl);
			}
			// Generar hijo actTURN_R
			nodeN1 child_turnr;
			child_turnr.padre = make_shared<nodeN1>(current_node);
			child_turnr.st = apply(actTURN_R, current_node.st, mapa);
			if (explored.find(child_turnr)==explored.end()){
				child_turnr.accion = actTURN_R;
				frontier.push_back(child_turnr);
			}
			// if(son_en_vision){
			if(false){
				// Generar hijo actSON_FORWARD
				nodeN1 child_son_forward;
				child_son_forward.padre = make_shared<nodeN1>(current_node);
				child_son_forward.st = apply(actSON_FORWARD, current_node.st, mapa);
				if (explored.find(child_son_forward)==explored.end()) {
					child_son_forward.accion = actSON_FORWARD;
					frontier.push_back(child_son_forward);
				}
				// Generar hijo actSON_TURN_L
				nodeN1 child_son_turnl;
				child_son_turnl.padre = make_shared<nodeN1>(current_node);
				child_son_turnl.st = apply(actSON_TURN_SL, current_node.st, mapa);
				if (explored.find(child_son_turnl)==explored.end()){
					child_son_turnl.accion = actSON_TURN_SL;
					frontier.push_back(child_son_turnl);
				}
				// Generar hijo actSON_TURN_R
				nodeN1 child_son_turnr = current_node;
				child_son_turnr.padre = make_shared<nodeN1>(current_node);
				child_son_turnr.st = apply(actSON_TURN_SR, current_node.st, mapa);
				if (explored.find(child_son_turnr)==explored.end()){
					child_son_turnr.accion = actSON_TURN_SR;
					frontier.push_back(child_son_turnr);
				}
			}

		}

		if (!SolutionFound && !frontier.empty()){
			current_node = frontier.front();
			while (!frontier.empty() && explored.find(current_node)!=explored.end()) {
				frontier.pop_front();
				current_node = frontier.front();
			}
		}
	}

  	if (SolutionFound){
		plan.push_front(current_node.accion);
		if(current_node.padre != nullptr) do {
			plan.push_front(current_node.padre->accion);
			current_node.padre = current_node.padre->padre;			
		} while(current_node.padre->padre != nullptr);
	}
	
	return plan;

}


// Calcula el plan para el nivel 1
list<Action> AnchuraNivel1(const stateN1 &inicio, const ubicacion &final,
							const vector<vector<unsigned char>> &mapa) {
	nodeN1 current_node;
	list<nodeN1> frontier;
	set<nodeN1> explored;
	list<Action> plan{};
	int n = 0;
	current_node.st = inicio;
	current_node.padre = nullptr;
	bool SolutionFound = (current_node.st.sonambulo.f==final.f &&
							current_node.st.sonambulo.c==final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() && !SolutionFound) {
		frontier.pop_front();
		explored.insert(current_node);
		bool son_en_vision = SonambuloEnVision(current_node.st);
		n++;
		if(n%10000==0)
			cout << "Nodos explorados: " << n << endl;
		// Generar hijo actSON_FORWARD
		if(son_en_vision){
			nodeN1 child_son_forward;
			child_son_forward.padre = make_shared<nodeN1>(current_node);
			child_son_forward.st = apply(actSON_FORWARD, current_node.st, mapa);
			if (child_son_forward.st.sonambulo.f==final.f && child_son_forward.st.sonambulo.c==final.c) {
				child_son_forward.accion = actSON_FORWARD;
				current_node = child_son_forward;
				SolutionFound = true;
				cout << "Nodos explorados: " << n << endl;
			} else if (explored.find(child_son_forward)==explored.end()) {
				child_son_forward.accion = actSON_FORWARD;
				frontier.push_back(child_son_forward);
			}
		}

		if (!SolutionFound) {
			// Generar hijo actFORWARD
			nodeN1 child_forward;
			child_forward.padre = make_shared<nodeN1>(current_node);
			child_forward.st = apply(actFORWARD, current_node.st, mapa);
			if (explored.find(child_forward)==explored.end()) {
				child_forward.accion = actFORWARD;
				frontier.push_back(child_forward);
			}
			// Generar hijo actTURN_L
			nodeN1 child_turnl;
			child_turnl.padre = make_shared<nodeN1>(current_node);
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			if (explored.find(child_turnl)==explored.end()){
				child_turnl.accion = actTURN_L;
				frontier.push_back(child_turnl);
			}
			// Generar hijo actTURN_R
			nodeN1 child_turnr;
			child_turnr.padre = make_shared<nodeN1>(current_node);
			child_turnr.st = apply(actTURN_R, current_node.st, mapa);
			if (explored.find(child_turnr)==explored.end()){
				child_turnr.accion = actTURN_R;
				frontier.push_back(child_turnr);
			}
			if(son_en_vision){
				// Generar hijo actSON_TURN_L
				nodeN1 child_son_turnl;
				child_son_turnl.padre = make_shared<nodeN1>(current_node);
				child_son_turnl.st = apply(actSON_TURN_SL, current_node.st, mapa);
				if (explored.find(child_son_turnl)==explored.end()){
					child_son_turnl.accion = actSON_TURN_SL;
					frontier.push_back(child_son_turnl);
				}
				// Generar hijo actSON_TURN_R
				nodeN1 child_son_turnr = current_node;
				child_son_turnr.padre = make_shared<nodeN1>(current_node);
				child_son_turnr.st = apply(actSON_TURN_SR, current_node.st, mapa);
				if (explored.find(child_son_turnr)==explored.end()){
					child_son_turnr.accion = actSON_TURN_SR;
					frontier.push_back(child_son_turnr);
				}
			}

		}

		if (!SolutionFound && !frontier.empty()){
			current_node = frontier.front();
			while (!frontier.empty() && explored.find(current_node)!=explored.end()) {
				frontier.pop_front();
				current_node = frontier.front();
			}
		}
	}

  	if (SolutionFound){
		plan.push_front(current_node.accion);
		if(current_node.padre != nullptr) do {
			plan.push_front(current_node.padre->accion);
			current_node.padre = current_node.padre->padre;			
		} while(current_node.padre->padre != nullptr);
	}
	
	return plan;
}


list<Action> Dijkstra(const stateN2 &inicio, const ubicacion &final,
							const vector<vector<unsigned char>> &mapa) {
	nodeN2 current_node;
	nodeN2 solution_node;
	priority_queue<nodeN2> frontier;
	set<nodeN2> explored;
	list<Action> plan;
	int n = 0;
	solution_node.st.coste = numeric_limits<int>::max();
	current_node.st = inicio;
	current_node.padre = nullptr;
	bool SolutionFound = (current_node.st.jugador.f==final.f &&
							current_node.st.jugador.c==final.c);
	if(SolutionFound)
		return plan;


	if(mapa[inicio.jugador.f][inicio.jugador.c] == 'K'){
		current_node.st.bikini = true;
		current_node.st.zapatillas = false;
	}
	if(mapa[inicio.jugador.f][inicio.jugador.c] == 'D'){
		current_node.st.bikini = false;
		current_node.st.zapatillas = true;
	}


	frontier.push(current_node);

	while (!frontier.empty() && !SolutionFound) {
		frontier.pop();
		explored.insert(current_node);
		n++;
		if(n%10000==0){
			cout << "Nodos explorados: " << n << endl;
			cout << "Coste actual: " << current_node.st.coste << endl;
		}
		// Generar hijo actFORWARD

		nodeN2 child_forward;
		child_forward.padre = make_shared<nodeN2>(current_node);
		child_forward.st = apply(actFORWARD, current_node.st, mapa);

		if (child_forward.st.jugador.f==final.f && child_forward.st.jugador.c==final.c) {
			child_forward.accion = actFORWARD;
			current_node = child_forward;
			// cout << endl << "Posible solucion encontrada" << endl;
			// cout << "Nodos explorados: " << n << endl;
			// cout << "Coste actual: " << current_node.st.coste << endl;
			// cout << "Mejor coste actual: " << solution_node.st.coste << endl;
			// if (solution_node.padre != nullptr ) cout << "Mejor coste actual padre: " << solution_node.padre->st.coste << endl;
			// cout << "Coste minimo disponible para expandir: " << frontier.top().st.coste << endl;
			if(current_node.st.coste <= solution_node.st.coste){
				solution_node = current_node;	
			}

			if(frontier.top().st.coste > solution_node.st.coste){
				SolutionFound = true;
				cout << "Nodos explorados: " << n << endl;
				cout << "Coste final: " << solution_node.st.coste << endl;
				cout << "Bateria restante: " << 3000 - solution_node.st.coste << endl;
			}
			
		} else if (explored.find(child_forward)==explored.end()) {
			child_forward.accion = actFORWARD;
			frontier.push(child_forward);
		}

		if (!SolutionFound) {
			// Generar hijo actTURN_L
			nodeN2 child_turnl;
			child_turnl.padre = make_shared<nodeN2>(current_node);
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			if (explored.find(child_turnl)==explored.end()){
				child_turnl.accion = actTURN_L;
				frontier.push(child_turnl);
			}
			// Generar hijo actTURN_R
			nodeN2 child_turnr;
			child_turnr.padre = make_shared<nodeN2>(current_node);
			child_turnr.st = apply(actTURN_R, current_node.st, mapa);
			if (explored.find(child_turnr)==explored.end()){
				child_turnr.accion = actTURN_R;
				frontier.push(child_turnr);
			}

		}

		if (!SolutionFound && !frontier.empty()){
			current_node = frontier.top();
			while (!frontier.empty() && explored.find(current_node)!=explored.end()) {
				frontier.pop();
				current_node = frontier.top();
			}
		}
	}

  	if (SolutionFound){
		plan.push_front(solution_node.accion);
		if(solution_node.padre != nullptr) do {
			plan.push_front(solution_node.padre->accion);
			solution_node.padre = solution_node.padre->padre;			
		} while(solution_node.padre->padre != nullptr);
	}
	
	return plan;

}


int dist_manhattan(const ubicacion &a, const ubicacion &b){
	return abs(a.f-b.f) + abs(a.c-b.c);
}

int dist_max(const ubicacion &a, const ubicacion &b){
	return max(abs(a.f-b.f), abs(a.c-b.c));
}

int heuristic(const stateN3 &a, const ubicacion &b, const vector<vector<unsigned char>> &mapa){
	
	int dist_jug = max(0, dist_manhattan(a.jugador, a.sonambulo) - 3);
	int dist_son = dist_max(a.sonambulo, b);

	// if(dist_obj > 0)
	// 	return dist_obj-1 + coste(a, actSON_FORWARD, mapa) + dist_son;
	// else
	// 	return 0;
	return dist_jug + dist_son;
}


list<Action> AStar(const stateN3 &inicio, const ubicacion &final,
							const vector<vector<unsigned char>> &mapa) {
	nodeN3 current_node;
	nodeN3 solution_node;
	priority_queue<nodeN3, vector<nodeN3>, Compare> frontier;
	set<nodeN3> explored;
	list<Action> plan;
	int n = 0;

	solution_node.st.coste = numeric_limits<int>::max();
	solution_node.st.heuristica = heuristic(inicio, final, mapa);
	solution_node.st.suma = solution_node.st.coste ;

	current_node.st = inicio;
	current_node.padre = nullptr;

	bool SolutionFound = (current_node.st.sonambulo.f==final.f &&
							current_node.st.sonambulo.c==final.c);
	if(SolutionFound)
		return plan;

	if(mapa[inicio.jugador.f][inicio.jugador.c] == 'K'){
		current_node.st.bikini = true;
		current_node.st.zapatillas = false;
	}
	if(mapa[inicio.jugador.f][inicio.jugador.c] == 'D'){
		current_node.st.bikini = false;
		current_node.st.zapatillas = true;
	}

	if(mapa[inicio.sonambulo.f][inicio.sonambulo.c] == 'K'){
		current_node.st.bikini_son = true;
		current_node.st.zapatillas_son = false;
	}
	if(mapa[inicio.sonambulo.f][inicio.sonambulo.c] == 'D'){
		current_node.st.bikini_son = false;
		current_node.st.zapatillas_son = true;
	}

	frontier.push(current_node);

	while (!frontier.empty() && !SolutionFound) {
		frontier.pop();
		explored.insert(current_node);
		bool son_en_vision = SonambuloEnVision(current_node.st);
		n++;
		if(n%10000==0){
			cout << endl;
			cout << "Nodos explorados: " << n << endl;
			cout << "Coste actual: " << current_node.st.coste << endl;
			cout << "Suma actual: " << current_node.st.suma << endl;
			
		}
		// cout << "Siguiente suma por explorar: " << frontier.top().st.suma << endl;
		// Generar hijo actSON_FORWARD
		if (son_en_vision) {
			nodeN3 child_son_forward;
			child_son_forward.padre = make_shared<nodeN3>(current_node);
			child_son_forward.st = apply(actSON_FORWARD, current_node.st, mapa);
			child_son_forward.st.heuristica = heuristic(child_son_forward.st, final, mapa);
			child_son_forward.st.suma = child_son_forward.st.coste + child_son_forward.st.heuristica;

			auto it = explored.find(child_son_forward);

			if (child_son_forward.st.sonambulo.f==final.f && child_son_forward.st.sonambulo.c==final.c) {
				child_son_forward.accion = actSON_FORWARD;
				current_node = child_son_forward;
				cout << endl << "Se ha encontrado una posible solucion" << endl;
				cout << "Coste: " << current_node.st.coste << endl;
				cout << "Suma: " << current_node.st.suma << endl;
				cout << "Siguiente suma por explorar: " << frontier.top().st.suma << endl;
				if(current_node.st.suma < solution_node.st.suma){
					solution_node = current_node;	
				}
			} else if (it==explored.end() and it->st.suma < solution_node.st.suma) {
				child_son_forward.accion = actSON_FORWARD;
				frontier.push(child_son_forward);
			}
			else if(it!=explored.end())
				if (it->st.suma > child_son_forward.st.suma){
					explored.erase(it);
					child_son_forward.accion = actSON_FORWARD;
					frontier.push(child_son_forward);
			}

			if(frontier.top().st.suma > solution_node.st.suma){
				SolutionFound = true;
				cout << "Nodos explorados: " << n << endl;
				cout << "Suma final: " << solution_node.st.suma << endl;
				cout << "Bateria restante: " << 3000 - solution_node.st.coste << endl;
			}
		}

		if (!SolutionFound) {
			// Generar hijo actFORWARD
			nodeN3 child_forward;
			child_forward.padre = make_shared<nodeN3>(current_node);
			child_forward.st = apply(actFORWARD, current_node.st, mapa);
			child_forward.st.heuristica = heuristic(child_forward.st, final, mapa);
			child_forward.st.suma = child_forward.st.coste + child_forward.st.heuristica;
			
			auto it = explored.find(child_forward);

			if (it==explored.end() and it->st.suma < solution_node.st.suma) {
				child_forward.accion = actFORWARD;
				frontier.push(child_forward);
			}
			else if(it!=explored.end())
				if (it->st.suma > child_forward.st.suma){
					explored.erase(it);
					child_forward.accion = actFORWARD;
					frontier.push(child_forward);
			}

			// Generar hijo actTURN_L
			nodeN3 child_turnl;
			child_turnl.padre = make_shared<nodeN3>(current_node);
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			child_turnl.st.heuristica = heuristic(child_turnl.st, final, mapa);
			child_turnl.st.suma = child_turnl.st.coste + child_turnl.st.heuristica;

			it = explored.find(child_turnl);

			if (it==explored.end() and it->st.suma < solution_node.st.suma) {
				child_turnl.accion = actTURN_L;
				frontier.push(child_turnl);
			}
			else if(it!=explored.end())
				if (it->st.suma > child_turnl.st.suma){
					explored.erase(it);
					child_turnl.accion = actTURN_L;
					frontier.push(child_turnl);
			}

			// Generar hijo actTURN_R
			nodeN3 child_turnr;
			child_turnr.padre = make_shared<nodeN3>(current_node);
			child_turnr.st = apply(actTURN_R, current_node.st, mapa);
			child_turnr.st.heuristica = heuristic(child_turnr.st, final, mapa);
			child_turnr.st.suma = child_turnr.st.coste + child_turnr.st.heuristica;
			
			it = explored.find(child_turnr);

			if (it==explored.end() and it->st.suma < solution_node.st.suma) {
				child_turnr.accion = actTURN_R;
				frontier.push(child_turnr);
			}
			else if(it!=explored.end())
				if (it->st.suma > child_turnr.st.suma){
					explored.erase(it);
					child_turnr.accion = actTURN_R;
					frontier.push(child_turnr);
			}

			if(son_en_vision){

				// Generar hijo actSON_TURN_SR
				nodeN3 child_son_turnr;
				child_son_turnr.padre = make_shared<nodeN3>(current_node);
				child_son_turnr.st = apply(actSON_TURN_SR, current_node.st, mapa);
				child_son_turnr.st.heuristica = heuristic(child_son_turnr.st, final, mapa);
				child_son_turnr.st.suma = child_son_turnr.st.coste + child_son_turnr.st.heuristica;

				it = explored.find(child_son_turnr);

				if (it==explored.end() and it->st.suma < solution_node.st.suma) {
					child_son_turnr.accion = actSON_TURN_SR;
					frontier.push(child_son_turnr);
				}
				else if(it!=explored.end())
					if (it->st.suma > child_son_turnr.st.suma){
						explored.erase(it);
						child_son_turnr.accion = actSON_TURN_SR;
						frontier.push(child_son_turnr);
				}

				// Generar hijo actSON_TURN_SL
				nodeN3 child_son_turnl;
				child_son_turnl.padre = make_shared<nodeN3>(current_node);
				child_son_turnl.st = apply(actSON_TURN_SL, current_node.st, mapa);
				child_son_turnl.st.heuristica = heuristic(child_son_turnl.st, final, mapa);
				child_son_turnl.st.suma = child_son_turnl.st.coste + child_son_turnl.st.heuristica;

				it = explored.find(child_son_turnl);

				if (it==explored.end() and it->st.suma < solution_node.st.suma) {
					child_son_turnl.accion = actSON_TURN_SL;
					frontier.push(child_son_turnl);
				}
				else if(it!=explored.end())
					if (it->st.suma > child_son_turnl.st.suma){
						explored.erase(it);
						child_son_turnl.accion = actSON_TURN_SL;
						frontier.push(child_son_turnl);
					}
			}

		}

		if (!SolutionFound && !frontier.empty()){
			current_node = frontier.top();
			while (!frontier.empty() && explored.find(current_node)!=explored.end() && current_node.st.suma > solution_node.st.suma) {
				if(current_node.st.suma > solution_node.st.suma){
					explored.insert(current_node);
				}
				
				frontier.pop();
				current_node = frontier.top();

			}
		}
	}

  	if (SolutionFound){
		plan.push_front(solution_node.accion);
		if(solution_node.padre != nullptr) do {
			plan.push_front(solution_node.padre->accion);
			solution_node.padre = solution_node.padre->padre;			
		} while(solution_node.padre->padre != nullptr);
	}
	
	return plan;

}


// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;

	// Incluir aquí el comportamiento del agente jugador
	if (!hayPlan){
	// Invocar al método de búsqueda
		cout << "Calculando plan..." << endl;
		c_state.jugador.f = sensores.posF;
		c_state.jugador.c = sensores.posC;
		c_state.jugador.brujula = sensores.sentido;
		c_state.sonambulo.f = sensores.SONposF;
		c_state.sonambulo.c = sensores.SONposC;
		c_state.sonambulo.brujula = sensores.SONsentido;
		c_state.coste = 0;
		c_state.bikini = false;
		c_state.zapatillas = false;
		c_state.bikini_son = false;
		c_state.zapatillas_son = false;
		goal.f = sensores.destinoF;
		goal.c = sensores.destinoC;	
		
		// mapaResultado[-1][-1] = 'X';
		switch (sensores.nivel)
		{
		case 0:
			plan = AnchuraSoloJugador(c_state, goal, mapaResultado);
			break;
		case 1:
			plan = AnchuraNivel1(c_state, goal, mapaResultado);
			break;
		case 2:
			plan = Dijkstra(c_state, goal, mapaResultado);
			break;
		case 3:
			plan = AStar(c_state, goal, mapaResultado);
			break;
		default:
			break;
		}

		if(plan.size() > 0){
			VisualizaPlan(c_state,plan);
		}

		hayPlan = true;
	}

	if (hayPlan and plan.size()>0){
		cout << "Ejecutando siguiente acción ";
		switch (plan.front())
		{
		case actFORWARD:
			cout << "actFORWARD" << endl;
			break;
		case actTURN_L:
			cout << "actTURN_L" << endl;
			break;
		case actTURN_R:
			cout << "actTURN_R" << endl;
			break;
		case actIDLE:
			cout << "actIDLE" << endl;
			break;
		case actSON_FORWARD:
			cout << "actSON_FORWARD" << endl;
			break;
		case actSON_TURN_SL:
			cout << "actSON_TURN_SL" << endl;
			break;
		case actSON_TURN_SR:
			cout << "actSON_TURN_SR" << endl;
			break;
		
		default:
			break;
		}
		
		accion = plan.front();
		plan.pop_front();
	}

	if (plan.size()== 0){
		cout << "Se completó el plan" << endl;
		hayPlan = false;
	}

	return accion;
}


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}

// ./practica2 mapas/paldea2.map 1 3 84 39 6 79 73 1 63 68
//