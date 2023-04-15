#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>

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


// Calcula el plan para el nivel 0
list<Action> AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final,
							const vector<vector<unsigned char>> &mapa) {
	nodeN0 current_node;
	list<nodeN0> frontier;
	set<nodeN0> explored;
	list<Action> plan;
	current_node.st = inicio;
	bool SolutionFound = (current_node.st.jugador.f==final.f &&
							current_node.st.jugador.c==final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() && !SolutionFound) {
		frontier.pop_front();
		explored.insert(current_node);

		// Generar hijo actFORWARD
		nodeN0 child_forward = current_node;
		child_forward.st = apply(actFORWARD, current_node.st, mapa);
		if (child_forward.st.jugador.f==final.f && child_forward.st.jugador.c==final.c) {
			child_forward.secuencia.push_back(actFORWARD);
			current_node = child_forward;
			SolutionFound = true;
		} else if (explored.find(child_forward)==explored.end()) {
			child_forward.secuencia.push_back(actFORWARD);
			frontier.push_back(child_forward);
		}

		if (!SolutionFound) {
			// Generar hijo actTURN_L
			nodeN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			if (explored.find(child_turnl)==explored.end()){
				child_turnl.secuencia.push_back(actTURN_L);
				frontier.push_back(child_turnl);
			}
			// Generar hijo actTURN_R
			nodeN0 child_turnr = current_node;
			child_turnr.st = apply(actTURN_R, current_node.st, mapa);
			if (explored.find(child_turnr)==explored.end()){
				child_turnr.secuencia.push_back(actTURN_R);
				frontier.push_back(child_turnr);
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

  	if (SolutionFound)
		plan = current_node.secuencia;
	
	return plan;
}

// Calcula el plan para el nivel 1
list<Action> AnchuraNivel1(const stateN1 &inicio, const ubicacion &final,
							const vector<vector<unsigned char>> &mapa) {
	nodeN1 current_node;
	list<nodeN1> frontier;
	set<nodeN1> explored;
	list<Action> plan;
	current_node.st = inicio;
	bool SolutionFound = (current_node.st.sonambulo.f==final.f &&
							current_node.st.sonambulo.c==final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() && !SolutionFound) {
		frontier.pop_front();
		explored.insert(current_node);
		bool son_en_vision = SonambuloEnVision(current_node.st);
		// Generar hijo actSON_FORWARD
		if(son_en_vision){
			nodeN1 child_son_forward = current_node;
			child_son_forward.st = apply(actSON_FORWARD, current_node.st, mapa);
			if (child_son_forward.st.sonambulo.f==final.f && child_son_forward.st.sonambulo.c==final.c) {
				child_son_forward.secuencia.push_back(actSON_FORWARD);
				current_node = child_son_forward;
				SolutionFound = true;
			} else if (explored.find(child_son_forward)==explored.end()) {
				child_son_forward.secuencia.push_back(actSON_FORWARD);
				frontier.push_back(child_son_forward);
			}
		}

		if (!SolutionFound) {
			// Generar hijo actFORWARD
			nodeN1 child_forward = current_node;
			child_forward.st = apply(actFORWARD, current_node.st, mapa);
			if (explored.find(child_forward)==explored.end()) {
				child_forward.secuencia.push_back(actFORWARD);
				frontier.push_back(child_forward);
			}
			// Generar hijo actTURN_L
			nodeN1 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			if (explored.find(child_turnl)==explored.end()){
				child_turnl.secuencia.push_back(actTURN_L);
				frontier.push_back(child_turnl);
			}
			// Generar hijo actTURN_R
			nodeN1 child_turnr = current_node;
			child_turnr.st = apply(actTURN_R, current_node.st, mapa);
			if (explored.find(child_turnr)==explored.end()){
				child_turnr.secuencia.push_back(actTURN_R);
				frontier.push_back(child_turnr);
			}
			if(son_en_vision){
				// Generar hijo actSON_TURN_L
				nodeN1 child_son_turnl = current_node;
				child_son_turnl.st = apply(actSON_TURN_SL, current_node.st, mapa);
				if (explored.find(child_son_turnl)==explored.end()){
					child_son_turnl.secuencia.push_back(actSON_TURN_SL);
					frontier.push_back(child_son_turnl);
				}
				// Generar hijo actSON_TURN_R
				nodeN1 child_son_turnr = current_node;
				child_son_turnr.st = apply(actSON_TURN_SR, current_node.st, mapa);
				if (explored.find(child_son_turnr)==explored.end()){
					child_son_turnr.secuencia.push_back(actSON_TURN_SR);
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

  	if (SolutionFound)
		plan = current_node.secuencia;
	
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
		default:
			break;
		}
		VisualizaPlan(c_state,plan);
		hayPlan = true;
	}

	if (hayPlan and plan.size()>0){
		cout << "Ejecutando siguiente acción del plan" << endl;
		accion = plan.front();
		c_state = apply(accion, c_state, mapaResultado);
		plan.pop_front();
		if(SonambuloEnVision(c_state))
			cout << "Sonambulo en vision!!!11!1!!" << endl;
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