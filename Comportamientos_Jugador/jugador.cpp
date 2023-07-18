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

unsigned char SuperficieEnUbi(const ubicacion &u, const vector<unsigned char> &superficie, const stateN0 &st){

	int radio = 4;
	int i = st.jugador.f; int j = st.jugador.c;
	int add;

	switch(st.jugador.brujula){
		case norte: 	add=-1; 	break;
		case este:	 	add= 1; 	break;
		case sur: 		add= 1; 	break;
		case oeste: 	add=-1; 	break;
		case noreste:	add=-1;		break;
		case suroeste:	add= 1;		break;
		case noroeste:	add=-1;		break;
		case sureste:	add= 1;		break;
	}

	if(st.jugador.brujula == norte or st.jugador.brujula == sur){

		for (int k=0; k < radio; k++){

			for(int l=k*k; l < (k+1)*(k+1); l++){
				if(i == u.f && j == u.c) return superficie[l];
				j-=add;
			}
			j+= 2*(k+1)*add;
			i+=add;
		}
	}

	else if(st.jugador.brujula == este or st.jugador.brujula == oeste){

		for (int k=0; k < radio; k++){

			for(int l=k*k; l < (k+1)*(k+1); l++){
				if(i == u.f && j == u.c) return superficie[l];
				i+=add;
			}
			i-= 2*(k+1)*add;
			j+=add;
		}
	}

	return '_';
}

vector <unsigned char> PosAVision(const ubicacion &u, const vector<vector<unsigned char>> &mapa){
	int i = u.f; int j = u.c;
	int add;
	int radio = 4;
	Orientacion o = u.brujula;
	vector <unsigned char> vision;

	switch(o){
		case este:	 	
		case sur: 		add= 1;		break;
		case norte: 	
		case oeste: 	add=-1;		break;
	}

	if(o == norte or o == sur){
		for (int k=0; k < radio; k++){
			for(int l=k*k; l < (k+1)*(k+1); l++){
				vision.push_back(mapa[i][j]);
				j-=add;
			}
			j+= 2*(k+1)*add;
			i+=add;
		}
	}

	else if(o == este or o == oeste){
		for (int k=0; k < radio; k++){
			for(int l=k*k; l < (k+1)*(k+1); l++){
				vision.push_back(mapa[i][j]);
				i+=add;
			}
			i-= 2*(k+1)*add;
			j+=add;
		}
	}

	return vision;
}

template <typename T>
bool Equals(const vector<T> &v1, const vector<T> &v2){
	if(v1.size() != v2.size()) return false;
	for(int i=0; i < v1.size(); i++){
		if((v1[i] != v2[i]) and (v1[i] != '?' and v2[i] != '?')) return false;
	}
	return true;
}

bool BienSituado(const ubicacion &u, const vector<unsigned char> &vision, const vector<vector<unsigned char>> &mapa){
	
	vector<unsigned char> vision2 = PosAVision(u, mapa);

	if(Equals(vision, vision2)) return true;
	return false;
}

/** Devuelve si una ubicación en el mapa es transitable para el agente */
bool CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char>> &mapa)
{
	if(x.f < 0 or x.f >= mapa.size() or x.c < 0 or x.c >= mapa[0].size()) return false;
	
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
		case '?':
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
		case '?':
			return 2;
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
		case '?':
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

stateN3 applyN4(const Action &a, const stateN3 &st, const vector<vector<unsigned char>> &mapa, const ubicacion &goal, bool planSon = false)
{
  stateN3 st_result = st;
  ubicacion sig_ubicacion;
  stateN3 aux = st_result;

  if(a != actFORWARD and a != actSON_FORWARD)
    st_result.coste += coste(st_result, a, mapa);

  switch (a){
  case actFORWARD: // si casilla delante es transitable y no está ocupada por el sonámbulo ni la meta
	sig_ubicacion = NextCasilla(st.jugador);
	if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.sonambulo.f && sig_ubicacion.c == st.sonambulo.c)
												and !(sig_ubicacion.f == goal.f && sig_ubicacion.c == goal.c && planSon)){
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
	aux.sonambulo = sig_ubicacion;
	if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.jugador.f && sig_ubicacion.c == st.jugador.c)
			and SonambuloEnVision(aux)){
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
		n++;

		if(n%10000 == 0) cout << "Nodos explorados: " << n << endl;

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
	priority_queue<nodeN2, vector<nodeN2>, CompareN2> frontier;
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
 		auto it = explored.find(child_forward);

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
			}
			
		} else if (it==explored.end() and child_forward.st.coste < solution_node.st.coste) {
			child_forward.accion = actFORWARD;
			frontier.push(child_forward);
		}
		else if(it!=explored.end())
			if (it->st.coste > child_forward.st.coste){
				explored.erase(it);
				child_forward.accion = actFORWARD;
				frontier.push(child_forward);
		}

		if (!SolutionFound) {
			// Generar hijo actTURN_L
			nodeN2 child_turnl;
			child_turnl.padre = make_shared<nodeN2>(current_node);
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			auto it = explored.find(child_turnl);
			if (explored.find(child_turnl)==explored.end() && child_turnl.st.coste < solution_node.st.coste){
				child_turnl.accion = actTURN_L;
				frontier.push(child_turnl);
			}
			else if(it!=explored.end())
				if (it->st.coste > child_turnl.st.coste){
					explored.erase(it);
					child_turnl.accion = actTURN_L;
					frontier.push(child_turnl);
			}

			// Generar hijo actTURN_R
			nodeN2 child_turnr;
			child_turnr.padre = make_shared<nodeN2>(current_node);
			child_turnr.st = apply(actTURN_R, current_node.st, mapa);
			it = explored.find(child_turnr);
			if (explored.find(child_turnr)==explored.end() && child_turnr.st.coste < solution_node.st.coste){
				child_turnr.accion = actTURN_R;
				frontier.push(child_turnr);
			}
			else if(it!=explored.end())
				if (it->st.coste > child_turnr.st.coste){
					explored.erase(it);
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

	cout << "Nodos explorados: " << n << endl;
	cout << "Coste final: " << solution_node.st.coste << endl;
	cout << "Bateria restante: " << 3000 - solution_node.st.coste << endl;
	
  	plan.push_front(solution_node.accion);
	while(solution_node.padre->padre != nullptr) {
		solution_node = *solution_node.padre;
		plan.push_front(solution_node.accion);		
	}	

	return plan;

}


int dist_manhattan(const ubicacion &a, const ubicacion &b){
	return abs(a.f-b.f) + abs(a.c-b.c);
}

int dist_max(const ubicacion &a, const ubicacion &b){
	return max(abs(a.f-b.f), abs(a.c-b.c));
}

int dist_euclidea(const ubicacion &a, const ubicacion &b){
	return sqrt(pow(a.f-b.f,2) + pow(a.c-b.c,2));
}

int heuristic(const stateN3 &a, const ubicacion &b, const vector<vector<unsigned char>> &mapa){
	
	int dist_jug = max(0, dist_manhattan(a.jugador, a.sonambulo) - 7);
	int dist_obj = max(0, dist_manhattan(a.jugador, b) - 7);
	int dist_son = dist_max(a.sonambulo, b);

	// if(dist_obj > 0)
	// 	return dist_obj-1 + coste(a, actSON_FORWARD, mapa) + dist_son;
	// else
		// return 0;
	return dist_jug + dist_obj + dist_son;
}

int heuristicN4Son(const stateN3 &a, const ubicacion &b, const vector<vector<unsigned char>> &mapa){
	
	// int dist_jug = max(0, abs(a.jugador.f - a.sonambulo.f)-3 + abs(a.jugador.c - a.sonambulo.c)-3);
	// int dist_obj = max(0, abs(a.jugador.f - b.f)-4 + abs(a.jugador.c - b.c)-4);
	int dist_jug = max(0, dist_manhattan(a.jugador, a.sonambulo) - 6);
	int dist_obj = max(0, dist_manhattan(a.jugador, b) - 7);
	int dist_son = 3*dist_max(a.sonambulo, b);

	int coste_fin = 0;
	if(a.sonambulo.f == b.f && a.sonambulo.c == b.c)
		coste_fin = coste(a, actSON_FORWARD, mapa);
	
	return dist_jug + dist_son + dist_obj;
}

int heuristicN4Jug(const stateN3 &a, const ubicacion &b, const vector<vector<unsigned char>> &mapa){

	int dist_obj = dist_manhattan(a.jugador, b);
	// if(dist_obj > 0)
	// 	return dist_obj-1 + coste(a, actSON_FORWARD, mapa) + dist_son;
	// else
		// return 0;
	int coste_fin = 0;
	if(a.jugador.f == b.f && a.jugador.c == b.c)
		coste_fin = coste(a, actFORWARD, mapa);
	return dist_obj + coste_fin;
}

list<Action> AStar(const stateN3 &inicio, const ubicacion &final,
							const vector<vector<unsigned char>> &mapa) {
	nodeN3 current_node;
	nodeN3 solution_node;
	priority_queue<nodeN3, vector<nodeN3>, CompareN3> frontier;
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
		if(n%100000==0){
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
			} else if (it==explored.end() and child_son_forward.st.suma < solution_node.st.suma) {
				child_son_forward.accion = actSON_FORWARD;
				frontier.push(child_son_forward);
			}
			// else if(it!=explored.end())
			// 	if (it->st.suma > child_son_forward.st.suma){
			// 		// explored.erase(it);
			// 		child_son_forward.accion = actSON_FORWARD;
			// 		frontier.push(child_son_forward);
			// }

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

			if (it==explored.end() and child_forward.st.suma < solution_node.st.suma) {
				child_forward.accion = actFORWARD;
				frontier.push(child_forward);
			}
			// else if(it!=explored.end())
			// 	if (it->st.suma > child_forward.st.suma){
			// 		// explored.erase(it);
			// 		child_forward.accion = actFORWARD;
			// 		frontier.push(child_forward);
			// }

			// Generar hijo actTURN_L
			nodeN3 child_turnl;
			child_turnl.padre = make_shared<nodeN3>(current_node);
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			child_turnl.st.heuristica = heuristic(child_turnl.st, final, mapa);
			child_turnl.st.suma = child_turnl.st.coste + child_turnl.st.heuristica;

			it = explored.find(child_turnl);

			if (it==explored.end() and child_turnl.st.suma < solution_node.st.suma) {
				child_turnl.accion = actTURN_L;
				frontier.push(child_turnl);
			}
			// else if(it!=explored.end())
			// 	if (it->st.suma > child_turnl.st.suma){
			// 		// explored.erase(it);
			// 		child_turnl.accion = actTURN_L;
			// 		frontier.push(child_turnl);
			// }

			// Generar hijo actTURN_R
			nodeN3 child_turnr;
			child_turnr.padre = make_shared<nodeN3>(current_node);
			child_turnr.st = apply(actTURN_R, current_node.st, mapa);
			child_turnr.st.heuristica = heuristic(child_turnr.st, final, mapa);
			child_turnr.st.suma = child_turnr.st.coste + child_turnr.st.heuristica;
			
			it = explored.find(child_turnr);

			if (it==explored.end() and child_turnr.st.suma < solution_node.st.suma) {
				child_turnr.accion = actTURN_R;
				frontier.push(child_turnr);
			}
			// else if(it!=explored.end())
			// 	if (it->st.suma > child_turnr.st.suma){
			// 		// explored.erase(it);
			// 		child_turnr.accion = actTURN_R;
			// 		frontier.push(child_turnr);
			// }

			if(son_en_vision){

				// Generar hijo actSON_TURN_SR
				nodeN3 child_son_turnr;
				child_son_turnr.padre = make_shared<nodeN3>(current_node);
				child_son_turnr.st = apply(actSON_TURN_SR, current_node.st, mapa);
				child_son_turnr.st.heuristica = heuristic(child_son_turnr.st, final, mapa);
				child_son_turnr.st.suma = child_son_turnr.st.coste + child_son_turnr.st.heuristica;

				it = explored.find(child_son_turnr);

				if (it==explored.end() and child_son_turnr.st.suma < solution_node.st.suma) {
					child_son_turnr.accion = actSON_TURN_SR;
					frontier.push(child_son_turnr);
				}
				// else if(it!=explored.end())
				// 	if (it->st.suma > child_son_turnr.st.suma){
				// 		// explored.erase(it);
				// 		child_son_turnr.accion = actSON_TURN_SR;
				// 		frontier.push(child_son_turnr);
				// }

				// Generar hijo actSON_TURN_SL
				nodeN3 child_son_turnl;
				child_son_turnl.padre = make_shared<nodeN3>(current_node);
				child_son_turnl.st = apply(actSON_TURN_SL, current_node.st, mapa);
				child_son_turnl.st.heuristica = heuristic(child_son_turnl.st, final, mapa);
				child_son_turnl.st.suma = child_son_turnl.st.coste + child_son_turnl.st.heuristica;

				it = explored.find(child_son_turnl);

				if (it==explored.end() and child_son_turnl.st.suma < solution_node.st.suma) {
					child_son_turnl.accion = actSON_TURN_SL;
					frontier.push(child_son_turnl);
				}
				// else if(it!=explored.end())
				// 	if (it->st.suma > child_son_turnl.st.suma){
				// 		// explored.erase(it);
				// 		child_son_turnl.accion = actSON_TURN_SL;
				// 		frontier.push(child_son_turnl);
				// 	}
			}
		}

		if (!SolutionFound && !frontier.empty()){
			current_node = frontier.top();
			while ((frontier.size() > 1 && (explored.find(current_node)!=explored.end())) and current_node.st.suma < solution_node.st.suma) {
				
				frontier.pop();
				current_node = frontier.top();

			}
		}
	}

	cout << "Nodos explorados: " << n << endl;
	cout << "Suma final: " << solution_node.st.suma << endl;
	cout << "Bateria restante: " << 3000 - solution_node.st.coste << endl;

	plan.push_front(solution_node.accion);
	while(solution_node.padre->padre != nullptr) {
		solution_node = *solution_node.padre;
		plan.push_front(solution_node.accion);		
	}

	
	
	return plan;

}

list<Action> Nivel4Jug(const stateN3 &inicio, const ubicacion &final,
							const vector<vector<unsigned char>> &mapa){
	nodeN3 current_node;
	nodeN3 solution_node;
	priority_queue<nodeN3, vector<nodeN3>, CompareN3> frontier;
	set<nodeN3> explored;
	list<Action> plan;
	int n = 0;

	solution_node.st.coste = numeric_limits<int>::max();
	solution_node.st.heuristica = heuristicN4Jug(inicio, final, mapa);
	solution_node.st.suma = solution_node.st.coste ;

	current_node.st = inicio;
	current_node.padre = nullptr;

	bool haySolucion = false;
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
		if(n%100000==0){
			cout << endl;
			cout << "Nodos explorados: " << n << endl;
			cout << "Coste actual: " << current_node.st.coste << endl;
			cout << "Suma actual: " << current_node.st.suma << endl;
			if(n>=1000000){
				return {};
			}
		}
		// cout << "Siguiente suma por explorar: " << frontier.top().st.suma << endl;
		// Generar hijo actFORWARD

		nodeN3 child_forward;
		child_forward.padre = make_shared<nodeN3>(current_node);
		child_forward.st = applyN4(actFORWARD, current_node.st, mapa, final);
		child_forward.st.heuristica = heuristicN4Jug(child_forward.st, final, mapa);
		child_forward.st.suma = child_forward.st.coste + child_forward.st.heuristica;

		auto it = explored.find(child_forward);

		if (child_forward.st.jugador.f==final.f && child_forward.st.jugador.c==final.c) {
			child_forward.accion = actFORWARD;
			current_node = child_forward;
			haySolucion = true;
			cout << endl << "Se ha encontrado una posible solucion" << endl;
			cout << "Coste: " << current_node.st.coste << endl;
			cout << "Suma: " << current_node.st.suma << endl;
			if(current_node.st.suma < solution_node.st.suma){
				solution_node = current_node;	
			}
		} else if (it==explored.end() and child_forward.st.suma < solution_node.st.suma) {
			child_forward.accion = actFORWARD;
			frontier.push(child_forward);
		}
		else if(it!=explored.end())
			if (it->st.suma > child_forward.st.suma and n > 1){
				explored.erase(it);
				child_forward.accion = actFORWARD;
				frontier.push(child_forward);
		}

		if(frontier.top().st.suma > solution_node.st.suma){
			SolutionFound = true;
			cout << "Nodos explorados: " << n << endl;
			cout << "Suma final: " << solution_node.st.suma << endl;
			cout << "Bateria restante: " << 3000 - solution_node.st.coste << endl;
		}

		if (!SolutionFound) {
			
			// Generar hijo actTURN_L
			nodeN3 child_turnl;
			child_turnl.padre = make_shared<nodeN3>(current_node);
			child_turnl.st = applyN4(actTURN_L, current_node.st, mapa, final);
			child_turnl.st.heuristica = heuristicN4Jug(child_turnl.st, final, mapa);
			child_turnl.st.suma = child_turnl.st.coste + child_turnl.st.heuristica;

			it = explored.find(child_turnl);

			if (it==explored.end() and child_turnl.st.suma < solution_node.st.suma) {
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
			child_turnr.st = applyN4(actTURN_R, current_node.st, mapa, final);
			child_turnr.st.heuristica = heuristicN4Jug(child_turnr.st, final, mapa);
			child_turnr.st.suma = child_turnr.st.coste + child_turnr.st.heuristica;
			
			it = explored.find(child_turnr);

			if (it==explored.end() and child_turnr.st.suma < solution_node.st.suma) {
				child_turnr.accion = actTURN_R;
				frontier.push(child_turnr);
			}
			else if(it!=explored.end())
				if (it->st.suma > child_turnr.st.suma){
					explored.erase(it);
					child_turnr.accion = actTURN_R;
					frontier.push(child_turnr);
			}

			if (!SolutionFound && !frontier.empty()){
				current_node = frontier.top();
				while (!frontier.empty() && explored.find(current_node)!=explored.end()) {
					frontier.pop();
					current_node = frontier.top();
				}
			}
		}
	}

  	if(haySolucion)
	{
		plan.push_front(solution_node.accion);
		while(solution_node.padre->padre != nullptr) {
			solution_node = *solution_node.padre;
			plan.push_front(solution_node.accion);		
		}

		return plan;
	}
	else
	{
		plan.push_front(actIDLE);
		return plan;
	}

}
							

list<Action> Nivel4Son(const stateN3 &inicio, const ubicacion &final,
							const vector<vector<unsigned char>> &mapa, bool &stop) {
	nodeN3 current_node;
	nodeN3 solution_node;
	priority_queue<nodeN3, vector<nodeN3>, CompareN3> frontier;
	set<nodeN3> explored;
	list<Action> plan;
	int n = 0;

	solution_node.st.coste = numeric_limits<int>::max();
	solution_node.st.heuristica = heuristicN4Son(inicio, final, mapa);
	solution_node.st.suma = solution_node.st.coste ;

	current_node.st = inicio;
	current_node.padre = nullptr;

	bool haySolucion = false;
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

	while (!frontier.empty() && !SolutionFound and !stop) {
		frontier.pop();
		explored.insert(current_node);
		bool son_en_vision = SonambuloEnVision(current_node.st);
		n++;
		if(n%100000==0){
			cout << endl;
			cout << "Nodos explorados: " << n << endl;
			cout << "Coste actual: " << current_node.st.coste << endl;
			cout << "Suma actual: " << current_node.st.suma << endl;
			if(n>=1400000){
				stop = true;
			}
		}
		// cout << "Siguiente suma por explorar: " << frontier.top().st.suma << endl;
		// Generar hijo actSON_FORWARD
		if (son_en_vision) {
			nodeN3 child_son_forward;
			child_son_forward.padre = make_shared<nodeN3>(current_node);
			child_son_forward.st = applyN4(actSON_FORWARD, current_node.st, mapa, final, true);
			child_son_forward.st.heuristica = heuristicN4Son(child_son_forward.st, final, mapa);
			child_son_forward.st.suma = child_son_forward.st.coste + child_son_forward.st.heuristica;

			auto it = explored.find(child_son_forward);

			if (child_son_forward.st.sonambulo.f==final.f && child_son_forward.st.sonambulo.c==final.c) {
				child_son_forward.accion = actSON_FORWARD;
				current_node = child_son_forward;
				haySolucion = true;
				cout << endl << "Se ha encontrado una posible solucion" << endl;
				cout << "Coste: " << current_node.st.coste << endl;
				cout << "Suma: " << current_node.st.suma << endl;
				if(current_node.st.suma < solution_node.st.suma){
					solution_node = current_node;	
				}
			} else if (it==explored.end() and child_son_forward.st.suma < solution_node.st.suma) {
				child_son_forward.accion = actSON_FORWARD;
				frontier.push(child_son_forward);
			}
			else if(it!=explored.end())
				if (it->st.suma > child_son_forward.st.suma and n > 1){
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
			child_forward.st = applyN4(actFORWARD, current_node.st, mapa, final, true);
			child_forward.st.heuristica = heuristicN4Son(child_forward.st, final, mapa);
			child_forward.st.suma = child_forward.st.coste + child_forward.st.heuristica;
			
			auto it = explored.find(child_forward);

			if (it==explored.end() and child_forward.st.suma < solution_node.st.suma and
				(SonambuloEnVision(child_forward.st) or dist_max(child_forward.st.jugador, child_forward.st.sonambulo) < 3)) {
				child_forward.accion = actFORWARD;
				frontier.push(child_forward);
			}
			else if(it!=explored.end())
				if (it->st.suma > child_forward.st.suma and n > 1){
					explored.erase(it);
					child_forward.accion = actFORWARD;
					frontier.push(child_forward);
			}

			// Generar hijo actTURN_L
			nodeN3 child_turnl;
			child_turnl.padre = make_shared<nodeN3>(current_node);
			child_turnl.st = applyN4(actTURN_L, current_node.st, mapa, final, true);
			child_turnl.st.heuristica = heuristicN4Son(child_turnl.st, final, mapa);
			child_turnl.st.suma = child_turnl.st.coste + child_turnl.st.heuristica;

			it = explored.find(child_turnl);

			if (it==explored.end() and child_turnl.st.suma < solution_node.st.suma and
				(SonambuloEnVision(child_turnl.st) or dist_max(child_turnl.st.jugador, child_turnl.st.sonambulo) < 3)) {
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
			child_turnr.st = applyN4(actTURN_R, current_node.st, mapa, final, true);
			child_turnr.st.heuristica = heuristicN4Son(child_turnr.st, final, mapa);
			child_turnr.st.suma = child_turnr.st.coste + child_turnr.st.heuristica;
			
			it = explored.find(child_turnr);

			if (it==explored.end() and child_turnr.st.suma < solution_node.st.suma and
				(SonambuloEnVision(child_turnr.st) or dist_max(child_turnr.st.jugador, child_turnr.st.sonambulo) < 3)) {
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
				child_son_turnr.st = applyN4(actSON_TURN_SR, current_node.st, mapa, final, true);
				child_son_turnr.st.heuristica = heuristicN4Son(child_son_turnr.st, final, mapa);
				child_son_turnr.st.suma = child_son_turnr.st.coste + child_son_turnr.st.heuristica;

				it = explored.find(child_son_turnr);

				if (it==explored.end() and child_son_turnr.st.suma < solution_node.st.suma) {
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
				child_son_turnl.st = applyN4(actSON_TURN_SL, current_node.st, mapa, final, true);
				child_son_turnl.st.heuristica = heuristicN4Son(child_son_turnl.st, final, mapa);
				child_son_turnl.st.suma = child_son_turnl.st.coste + child_son_turnl.st.heuristica;

				it = explored.find(child_son_turnl);

				if (it==explored.end() and child_son_turnl.st.suma < solution_node.st.suma) {
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
			while (frontier.size() > 1 && (explored.find(current_node)!=explored.end())) {
				
				frontier.pop();
				current_node = frontier.top();

			}
		}
	}

	cout << "Nodos explorados: " << n << endl;
	cout << "Suma final: " << solution_node.st.suma << endl;
	cout << "Bateria restante: " << 3000 - solution_node.st.coste << endl;

	if(haySolucion)
	{
		stop = false;
		plan.push_front(solution_node.accion);
		while(solution_node.padre->padre != nullptr) {
			solution_node = *solution_node.padre;
			plan.push_front(solution_node.accion);		
		}

		return plan;
	}
	else
	{
		stop = true;
		plan.push_front(actIDLE);
		return plan;
	}
}

list<Action> VeASonambulo(const stateN2 &inicio, const vector<vector<unsigned char>> &mapa, bool &sonambulo_visible){

	nodeN2 current_node;
	nodeN2 solution_node;
	priority_queue<nodeN2, vector<nodeN2>, CompareN2> frontier;
	set<nodeN2> explored;
	list<Action> plan;
	int n = 0;
	solution_node.st.coste = numeric_limits<int>::max();
	current_node.st = inicio;
	current_node.padre = nullptr;
	solution_node.padre = nullptr;
	bool SolutionFound = (SonambuloEnVision(current_node.st));
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
		if(n%100000==0){
			cout << "Nodos explorados: " << n << endl;
			cout << "Coste actual: " << current_node.st.coste << endl;
		}
		// Generar hijo actFORWARD

		nodeN2 child_forward;
		child_forward.padre = make_shared<nodeN2>(current_node);
		child_forward.st = apply(actFORWARD, current_node.st, mapa);
 		auto it = explored.find(child_forward);

		if (dist_max(child_forward.st.jugador, child_forward.st.sonambulo) < 3 or 
			(dist_manhattan(child_forward.st.jugador, child_forward.st.sonambulo) <= 4 and SonambuloEnVision(child_forward.st))) {
			child_forward.accion = actFORWARD;
			current_node = child_forward;
			if(current_node.st.coste <= solution_node.st.coste){
				solution_node = current_node;	
			}

			if(frontier.top().st.coste > solution_node.st.coste){
				SolutionFound = true;
			}
			
		} else if (it==explored.end() and child_forward.st.coste < solution_node.st.coste) {
			child_forward.accion = actFORWARD;
			frontier.push(child_forward);
		}
		else if(it!=explored.end())
			if (it->st.coste > child_forward.st.coste){
				explored.erase(it);
				child_forward.accion = actFORWARD;
				frontier.push(child_forward);
		}

		if (!SolutionFound) {
			// Generar hijo actTURN_L
			nodeN2 child_turnl;
			child_turnl.padre = make_shared<nodeN2>(current_node);
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			auto it = explored.find(child_turnl);
			
			if (dist_max(child_turnl.st.jugador, child_turnl.st.sonambulo) < 3 or
				(dist_manhattan(child_turnl.st.jugador, child_turnl.st.sonambulo) <= 4 and SonambuloEnVision(child_turnl.st))) {
				child_turnl.accion = actTURN_L;
				current_node = child_turnl;
				if(current_node.st.coste <= solution_node.st.coste){
					solution_node = current_node;	
				}

				if(frontier.top().st.coste > solution_node.st.coste){
					SolutionFound = true;
				}
			}
			else if (explored.find(child_turnl)==explored.end() && child_turnl.st.coste < solution_node.st.coste){
				child_turnl.accion = actTURN_L;
				frontier.push(child_turnl);
			}
			else if(it!=explored.end())
				if (it->st.coste > child_turnl.st.coste){
					explored.erase(it);
					child_turnl.accion = actTURN_L;
					frontier.push(child_turnl);
			}

			// Generar hijo actTURN_R
			nodeN2 child_turnr;
			child_turnr.padre = make_shared<nodeN2>(current_node);
			child_turnr.st = apply(actTURN_R, current_node.st, mapa);
			it = explored.find(child_turnr);
			if (dist_max(child_turnr.st.jugador, child_turnr.st.sonambulo) < 3 or 
				(dist_manhattan(child_turnr.st.jugador, child_turnr.st.sonambulo) <= 4 and SonambuloEnVision(child_turnr.st))){
				child_turnr.accion = actTURN_R;
				current_node = child_turnr;
				if(current_node.st.coste <= solution_node.st.coste){
					solution_node = current_node;	
				}

				if(frontier.top().st.coste > solution_node.st.coste){
					SolutionFound = true;
				}
			}
			else if (explored.find(child_turnr)==explored.end() && child_turnr.st.coste < solution_node.st.coste){
				child_turnr.accion = actTURN_R;
				frontier.push(child_turnr);
			}
			else if(it!=explored.end())
				if (it->st.coste > child_turnr.st.coste){
					explored.erase(it);
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

	if(!SonambuloEnVision(solution_node.st)) sonambulo_visible = false;

  	plan.push_front(solution_node.accion);
	if(solution_node.padre != nullptr)
		while(solution_node.padre->padre != nullptr) {
			solution_node = *solution_node.padre;
			plan.push_front(solution_node.accion);		
		}
	

	
	cout << "Nodos explorados: " << n << endl;
	cout << "Coste final: " << solution_node.st.coste << endl;
	cout << "Bateria restante: " << 3000 - solution_node.st.coste << endl;
	
	

	return plan;

}

list<Action> VeARecarga(const stateN2 &inicio, const vector<vector<unsigned char>> &mapa){

	nodeN2 current_node;
	nodeN2 solution_node;
	priority_queue<nodeN2, vector<nodeN2>, CompareN2> frontier;
	set<nodeN2> explored;
	list<Action> plan;
	int n = 0;
	solution_node.st.coste = numeric_limits<int>::max();
	current_node.st = inicio;
	current_node.padre = nullptr;
	solution_node.padre = nullptr;
	bool SolutionFound = (mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'X');
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
		if(n%100000==0){
			cout << "Nodos explorados: " << n << endl;
			cout << "Coste actual: " << current_node.st.coste << endl;
		}
		// Generar hijo actFORWARD

		nodeN2 child_forward;
		child_forward.padre = make_shared<nodeN2>(current_node);
		child_forward.st = apply(actFORWARD, current_node.st, mapa);
 		auto it = explored.find(child_forward);

		if (mapa[child_forward.st.jugador.f][child_forward.st.jugador.c] == 'X') {
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
			}
			
		} else if (it==explored.end() and child_forward.st.coste < solution_node.st.coste) {
			child_forward.accion = actFORWARD;
			frontier.push(child_forward);
		}
		else if(it!=explored.end())
			if (it->st.coste > child_forward.st.coste){
				explored.erase(it);
				child_forward.accion = actFORWARD;
				frontier.push(child_forward);
		}

		if (!SolutionFound) {
			// Generar hijo actTURN_L
			nodeN2 child_turnl;
			child_turnl.padre = make_shared<nodeN2>(current_node);
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			auto it = explored.find(child_turnl);
			if (explored.find(child_turnl)==explored.end() && child_turnl.st.coste < solution_node.st.coste){
				child_turnl.accion = actTURN_L;
				frontier.push(child_turnl);
			}
			else if(it!=explored.end())
				if (it->st.coste > child_turnl.st.coste){
					explored.erase(it);
					child_turnl.accion = actTURN_L;
					frontier.push(child_turnl);
			}

			// Generar hijo actTURN_R
			nodeN2 child_turnr;
			child_turnr.padre = make_shared<nodeN2>(current_node);
			child_turnr.st = apply(actTURN_R, current_node.st, mapa);
			it = explored.find(child_turnr);
			if (explored.find(child_turnr)==explored.end() && child_turnr.st.coste < solution_node.st.coste){
				child_turnr.accion = actTURN_R;
				frontier.push(child_turnr);
			}
			else if(it!=explored.end())
				if (it->st.coste > child_turnr.st.coste){
					explored.erase(it);
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

  	plan.push_front(solution_node.accion);
	if(solution_node.padre != nullptr)
		while(solution_node.padre->padre != nullptr) {
			solution_node = *solution_node.padre;
			plan.push_front(solution_node.accion);		
		}
	
	cout << "Nodos explorados: " << n << endl;
	cout << "Coste final: " << solution_node.st.coste << endl;
	cout << "Bateria restante: " << 3000 - solution_node.st.coste << endl;

	return plan;

}

list<Action> Explorar(const stateN2 &inicio, const vector<vector<unsigned char>> &mapa){

	nodeN2 current_node;
	nodeN2 solution_node;
	priority_queue<nodeN2, vector<nodeN2>, CompareN2> frontier;
	set<nodeN2> explored;
	list<Action> plan;
	int n = 0;
	solution_node.st.coste = numeric_limits<int>::max();
	current_node.st = inicio;
	current_node.padre = nullptr;
	solution_node.padre = nullptr;
	bool SolutionFound = (mapa[current_node.st.jugador.f][current_node.st.jugador.c] == '?');
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
		if(n%100000==0){
			cout << "Nodos explorados: " << n << endl;
			cout << "Coste actual: " << current_node.st.coste << endl;
		}
		// Generar hijo actFORWARD

		nodeN2 child_forward;
		child_forward.padre = make_shared<nodeN2>(current_node);
		child_forward.st = apply(actFORWARD, current_node.st, mapa);
 		auto it = explored.find(child_forward);

		if (mapa[child_forward.st.jugador.f][child_forward.st.jugador.c] == '?') {
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
			}
			
		} else if (it==explored.end() and child_forward.st.coste < solution_node.st.coste) {
			child_forward.accion = actFORWARD;
			frontier.push(child_forward);
		}
		else if(it!=explored.end())
			if (it->st.coste > child_forward.st.coste){
				explored.erase(it);
				child_forward.accion = actFORWARD;
				frontier.push(child_forward);
		}

		if (!SolutionFound) {
			// Generar hijo actTURN_L
			nodeN2 child_turnl;
			child_turnl.padre = make_shared<nodeN2>(current_node);
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			auto it = explored.find(child_turnl);
			if (explored.find(child_turnl)==explored.end() && child_turnl.st.coste < solution_node.st.coste){
				child_turnl.accion = actTURN_L;
				frontier.push(child_turnl);
			}
			else if(it!=explored.end())
				if (it->st.coste > child_turnl.st.coste){
					explored.erase(it);
					child_turnl.accion = actTURN_L;
					frontier.push(child_turnl);
			}

			// Generar hijo actTURN_R
			nodeN2 child_turnr;
			child_turnr.padre = make_shared<nodeN2>(current_node);
			child_turnr.st = apply(actTURN_R, current_node.st, mapa);
			it = explored.find(child_turnr);
			if (explored.find(child_turnr)==explored.end() && child_turnr.st.coste < solution_node.st.coste){
				child_turnr.accion = actTURN_R;
				frontier.push(child_turnr);
			}
			else if(it!=explored.end())
				if (it->st.coste > child_turnr.st.coste){
					explored.erase(it);
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

  	plan.push_front(solution_node.accion);
	if(solution_node.padre != nullptr)
		while(solution_node.padre->padre != nullptr) {
			solution_node = *solution_node.padre;
			plan.push_front(solution_node.accion);		
		}
	
	cout << "Nodos explorados: " << n << endl;
	cout << "Coste final: " << solution_node.st.coste << endl;
	cout << "Bateria restante: " << 3000 - solution_node.st.coste << endl;

	return plan;

}


bool RecalcularJug(const ubicacion &x, const vector<vector<unsigned char>> &mapaRes, bool &bikini, bool &zapatillas, char superficie, bool planJug)
{
	bool res =  (mapaRes[x.f][x.c] == 'P' or mapaRes[x.f][x.c] == 'M' or 
		   		(mapaRes[x.f][x.c] == 'A' and (!bikini or planJug) ) or (mapaRes[x.f][x.c] == 'B' and (!zapatillas or planJug))) or 
		   		superficie == 'a' or superficie == 'l';
	
	return res;
}

bool RecalcularSon(const stateN3 &x, const vector<vector<unsigned char>> &mapaRes, bool &bikini, bool &zapatillas, const vector<unsigned char> &superficie, bool planJug)
{
	ubicacion n_pos = NextCasilla(x.sonambulo);
	unsigned char s = SuperficieEnUbi(n_pos, superficie, x);
	bool res =  (mapaRes[n_pos.f][n_pos.c] == 'P' or mapaRes[n_pos.f][n_pos.c] == 'M' or 
		   		(mapaRes[n_pos.f][n_pos.c] == 'A' and (!bikini or planJug) ) or 
				(mapaRes[n_pos.f][n_pos.c] == 'B' and (!zapatillas or planJug))) or 
		   		s == 'a' or s == 'l';
	
	return res;
}

void PonerTerrenoEnMatriz(const vector<unsigned char> &relleno, const vector<unsigned char> &superficie, const stateN0 &st, vector<vector<unsigned char>> &matriz, bool recalcula=false){

	int radio = 4;
	int i = st.jugador.f; int j = st.jugador.c;
	int add;
	int size = matriz.size();

	switch(st.jugador.brujula){
		case norte: 	add=-1; 	break;
		case este:	 	add= 1; 	break;
		case sur: 		add= 1; 	break;
		case oeste: 	add=-1; 	break;
		case noreste:	add=-1;		break;
		case suroeste:	add= 1;		break;
		case noroeste:	add=-1;		break;
		case sureste:	add= 1;		break;
	}

	if(st.jugador.brujula == norte or st.jugador.brujula == sur){

		for (int k=0; k < radio; k++){

			for(int l=k*k; l < (k+1)*(k+1); l++){
				if((superficie[l] == 'l' or superficie[l] == 'a') and recalcula)
					matriz[i][j] = 'M';
				else
					matriz[i][j] = relleno[l];

				j-=add;
			}
			j+= 2*(k+1)*add;
			i+=add;
		}
	}

	else if(st.jugador.brujula == este or st.jugador.brujula == oeste){

		for (int k=0; k < radio; k++){

			for(int l=k*k; l < (k+1)*(k+1); l++){
				if((superficie[l] == 'l' or superficie[l] == 'a') and recalcula)
					matriz[i][j] = 'M';
				else
					matriz[i][j] = relleno[l];

				i+=add;
			}
			i-= 2*(k+1)*add;
			j+=add;
		}
	}
}

bool ComportamientoJugador::HayRecarga(){

	for(auto i:mapaResultado){
		for(auto j: i){
			if(j == 'X')
				return true;
		}
	}
	
	return false;
}

// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"

Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;

	// TODO
	// Hay que cambiar el apply para que no cambie el estado cuando tengamos una entidad delante.
	// Hay que recalcular el plan cuando los sensores den colision

	if (sensores.nivel == 4){
		
	
		if (sensores.tiempo > 290)
			return actIDLE;

		if(2*sensores.bateria < sensores.vida) 
			necesita_recarga = true;
		else necesita_recarga = false;

		if(necesita_recarga and !hay_recarga){
			hay_recarga = HayRecarga();
			if(!hay_recarga and 4*sensores.bateria < sensores.vida)
				planExplorar = true;
			else
				planExplorar = false;
		}

		if(necesita_recarga and sensores.terreno[0] == 'X' and !recargando){
			recargando = true;
			// plan.clear();
		}

		if(sensores.bateria > 1.7*sensores.vida or sensores.bateria == 3000){
			necesita_recarga = false;
			recargando = false;
		}

		if(recargando and !situando){
			terreno_anterior = sensores.terreno;
			return actIDLE;
		}

		// if(sensores.tiempo < 1 and sensores.vida > 2800)
		// 	radio_son = 5;
		// if(size >= 75){
		// 	if(sensores.tiempo < 100) 
		// 	radio_son = 75;
		// 	else if(sensores.tiempo > 100 and sensores.tiempo < 180) 
		// 		radio_son = 50;
		// 	else if(sensores.tiempo > 180 and sensores.tiempo < 230)
		// 		radio_son = 25;
		// 	else if(sensores.tiempo > 230 and sensores.tiempo < 270)
		// 		radio_son = 8;
		// 	else if(sensores.tiempo > 270)
		// 		radio_son = 2;
		// }
		// else if(size >= 50){
		// 	if(sensores.tiempo < 100) 
		// 	radio_son = 50;
		// 	else if(sensores.tiempo > 100 and sensores.tiempo < 180) 
		// 		radio_son = 35;
		// 	else if(sensores.tiempo > 180 and sensores.tiempo < 230)
		// 		radio_son = 15;
		// 	else if(sensores.tiempo > 230 and sensores.tiempo < 270)
		// 		radio_son = 5;
		// 	else if(sensores.tiempo > 270)
		// 		radio_son = 2;
		// }
		// else if(size >= 30){
		// 	if(sensores.tiempo < 100) 
		// 	radio_son = 25;
		// 	else if(sensores.tiempo > 100 and sensores.tiempo < 180) 
		// 		radio_son = 20;
		// 	else if(sensores.tiempo > 180 and sensores.tiempo < 230)
		// 		radio_son = 15;
		// 	else if(sensores.tiempo > 230 and sensores.tiempo < 270)
		// 		radio_son = 5;
		// 	else if(sensores.tiempo > 270)
		// 		radio_son = 2;
		// }
		// else{
		// 	if(sensores.tiempo < 100) 
		// 	radio_son = 25;
		// 	else if(sensores.tiempo > 100 and sensores.tiempo < 180) 
		// 		radio_son = 20;
		// 	else if(sensores.tiempo > 180 and sensores.tiempo < 230)
		// 		radio_son = 15;
		// 	else if(sensores.tiempo > 230 and sensores.tiempo < 270)
		// 		radio_son = 5;
		// 	else if(sensores.tiempo > 270)
		// 		radio_son = 2;
		// }

		if ((hayPlan and plan.size()== 0 and bien_situado) or ((c_state.jugador.f == goal.f and c_state.jugador.c == goal.c)
											  or (c_state.sonambulo.f == goal.f and c_state.sonambulo.c == goal.c)) ){
			cout << "Se completó el plan" << endl;
			hayPlan = false;
			goal.f = sensores.destinoF;
			goal.c = sensores.destinoC;
			c_state.coste = 0;

			if(plan_completo){
				bien_situado = false;
				sonambulo_accesible = true;
			}
			plan_completo = true;
			stop = false;
		}
		else {
			plan_completo = false;
		}
	
		if(sensores.reset){
			plan.clear();
			hayPlan = false;
			bien_situado = false;
		}
		
		if(ultima_accion != actIDLE){
			c_state = apply(ultima_accion, c_state, mapaResultado);
		}

		if(sensores.colision or situando_cont > 0){
			plan.clear();
			hayPlan = false;
			if(BienSituado(c_state.jugador, sensores.terreno, mapaResultado)){
				situando_cont++;
				ultima_accion = actTURN_L;
				if(situando_cont == 4){
					situando_cont = 0;
					situando = false;
					bien_situado = true;
				}
				else return actTURN_L;
			}
				
			else
				bien_situado = false;
			

		}

		if (hayPlan){
			
			accion = plan.front();
			plan.pop_front();
			Action siguiente_accion = plan.front();
			plan.push_front(accion);

			stateN3 next_st = applyN4(accion, c_state, mapaResultado, goal);

			PonerTerrenoEnMatriz(sensores.terreno, sensores.superficie, c_state, mapaResultado);
			
			if((accion == actFORWARD and RecalcularJug(NextCasilla(c_state.jugador), mapaResultado, c_state.bikini, c_state.zapatillas, sensores.superficie[2], planJug))
				or (necesita_recarga and sensores.bateria < 200 and !planRecarga)){
				plan.clear();
				hayPlan = false;
			}
			else if((accion == actSON_FORWARD and RecalcularSon(c_state, mapaResultado, c_state.bikini_son, c_state.zapatillas_son, sensores.superficie, planJug))
				or (necesita_recarga and sensores.bateria < 200 and !planRecarga)){
				plan.clear();
				hayPlan = false;
			}
			else if(siguiente_accion == actSON_FORWARD and RecalcularSon(next_st, mapaResultado, next_st.bikini_son, next_st.zapatillas_son,  sensores.superficie, planJug)){
				plan.clear();
				hayPlan = false;
			}
		}
		
		if (!hayPlan){

			if(bien_situado){
				PonerTerrenoEnMatriz(sensores.terreno, sensores.superficie, c_state, mapaResultado, true);

				if(planExplorar){
					plan = Explorar(c_state, mapaResultado);
					hayPlan = true;
				}
				else if(necesita_recarga and hay_recarga and mapaResultado[c_state.sonambulo.f][c_state.sonambulo.c == goal.c] != 'X'){
					plan = VeARecarga(c_state, mapaResultado);
					planRecarga = true;
					planJug = true;
				} else if ((SonambuloEnVision(c_state) or dist_max(c_state.jugador, c_state.sonambulo) < 4) and sensores.tiempo < 270){
					plan = Nivel4Son(c_state, goal, mapaResultado, stop);
					planJug = false;
					planRecarga = false;
				} else if(sonambulo_accesible and sensores.tiempo < 270){
					plan = VeASonambulo(c_state, mapaResultado, sonambulo_accesible);
					planJug = true;
					planRecarga = false;
				}
				
				// if(dist_euclidea(c_state.sonambulo, goal) > radio_son or stop){
				// 	plan = Nivel4Jug(c_state, goal, mapaResultado);
				// 	planJug = true;
				// 	planRecarga = false;
				// }
				// else {
				// 	plan = Nivel4Son(c_state, goal, mapaResultado, stop);
				// 	planJug = false;
				// 	planRecarga = false;
				// }

				// if(stop and sensores.tiempo < 50){
				// 	stop = false;
				// 	plan = Nivel4Son(c_state, PuntoMedio(c_state.sonambulo, goal), mapaResultado, stop);
				// }

				if(stop or !sonambulo_accesible or sensores.tiempo > 270)
					plan = Nivel4Jug(c_state, goal, mapaResultado);
				
				hayPlan = true;
			}
			else if (situando){
				c_state.jugador.f = sensores.posF;
				c_state.jugador.c = sensores.posC;
				c_state.jugador.brujula = sensores.sentido;
				c_state.sonambulo.f = sensores.SONposF;
				c_state.sonambulo.c = sensores.SONposC;
				c_state.sonambulo.brujula = sensores.SONsentido;
				c_state.coste = 0;
				goal.f = sensores.destinoF;
				goal.c = sensores.destinoC;	
				situando = false;
				bien_situado = true;
				
				PonerTerrenoEnMatriz(sensores.terreno, sensores.superficie, c_state, mapaResultado, true);
				
				if(planExplorar){
					plan = Explorar(c_state, mapaResultado);
					hayPlan = true;
				}
				else if(necesita_recarga and hay_recarga and mapaResultado[c_state.sonambulo.f][c_state.sonambulo.c == goal.c] != 'X'){
					plan = VeARecarga(c_state, mapaResultado);
					planRecarga = true;
					planJug = true;
				} else if (SonambuloEnVision(c_state) or dist_max(c_state.jugador, c_state.sonambulo) < 4){
					plan = Nivel4Son(c_state, goal, mapaResultado, stop);
					planJug = false;
					planRecarga = false;
				} else if(sonambulo_accesible){
					plan = VeASonambulo(c_state, mapaResultado, sonambulo_accesible);
					planJug = true;
					planRecarga = false;
				}
				
				// if(stop and sensores.tiempo < 50){
				// 	stop = false;
				// 	plan = Nivel4Son(c_state, PuntoMedio(c_state.sonambulo, goal), mapaResultado, stop);
				// }
				if(stop or !sonambulo_accesible or sensores.tiempo > 270)
					plan = Nivel4Jug(c_state, goal, mapaResultado);
				
				hayPlan = true;
			}
			else {
				situando = true;
				vecesWhereIs++;
				cout << "vecesWhereIs: " << vecesWhereIs << endl;
				cout << "Bateria gastada por actWHEREIS: " << vecesWhereIs*200 << endl;
				terreno_anterior = sensores.terreno;
				ultima_accion = actIDLE;
				return actWHEREIS;
			}

			if(plan.size() > 0){
				VisualizaPlan(c_state,plan);
			}
		}

		if(hayPlan and plan.size() > 0){
			PonerTerrenoEnMatriz(sensores.terreno, sensores.superficie, c_state, mapaResultado);
			accion = plan.front();
			plan.pop_front();
			terreno_anterior = sensores.terreno;
			ultima_accion = accion;
			return accion;
		}
	}
	else {
		if (!hayPlan){
		// Invocar al método de búsqueda
			cout << "Calculando plan..." << endl;
			Initialize(sensores, c_state);
			c_state.coste = 0;
			goal.f = sensores.destinoF;
			goal.c = sensores.destinoC;	
			hayPlan = true;

			if(sensores.vida == 2999){
				c_state.bikini = false;
				c_state.zapatillas = false;
				c_state.bikini_son = false;
				c_state.zapatillas_son = false;

				if(mapaResultado[c_state.jugador.f][c_state.jugador.c] == 'K'){
					c_state.bikini = true;
					c_state.zapatillas = false;
				}
				if(mapaResultado[c_state.jugador.f][c_state.jugador.c] == 'D'){
					c_state.bikini = false;
					c_state.zapatillas = true;
				}

				if(mapaResultado[c_state.sonambulo.f][c_state.sonambulo.c] == 'K'){
					c_state.bikini_son = true;
					c_state.zapatillas_son = false;
				}
				if(mapaResultado[c_state.sonambulo.f][c_state.sonambulo.c] == 'D'){
					c_state.bikini_son = false;
					c_state.zapatillas_son = true;
				}
			}

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
			case 4:
				
			default:
				break;
			}

			if(plan.size() > 0){
				VisualizaPlan(c_state,plan);
			}
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
			c_state = apply(accion, c_state, mapaResultado);
		}

		if (plan.size()== 0){
			cout << "Se completó el plan" << endl;
			hayPlan = false;
		}
	}

	return accion;
}


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}

// ./practica2 mapas/paldea2.map 1 3 84 39 6 79 73 1 63 68


// ./practica2SG mapas/mapa30.map 1 4 4 4 6 12 12 2 8 8 16 6 26 9 25 16 3 3 5 10 16 14 18 10 21 4 7 4 24 4 12 3 6 15 25 18 8 19 15 14 23 15 26 8 10 24 19 26 25 7 16 11 22 15 20 15 22 19 10 20 4 13 26 24 9 6 26 10 17 19 25 13 24 20 26 19 12 18 8 23 9 13 6 5 8 16 12 5 3 14 11 22 11 8 6 17 7 4 21 3 23 4 15 5 7 23 21 19 4 15 6 13 24 17 6 26 5 4 24 10 16 17 13 20 22 9 26 22 22 24 14 4 24 26 7 18 6 21 9 9 18 6 19 15 16 21 3 14 13 10 25 13 17 24 7 20 14 14 9 21 5 18 20 20 19 10 5 18 18 21 9 22 20 19 7 15 26 20 10 17 17 19 8 23 8 9 9 5 20 3 20 11 6 23 16 8 26 14 17 4 8 25 14 13 25 14 8 5 20 21 4 18 14 25
// ./practica2SG mapas/mapa50.map 1 4 28 25 4 28 20 2 36 23 39 8 46 26 39 34 26 37 18 46 3 46 3 3 10 17 39 45 9 16 38 13 27 23 31 18 45 31 35 7 12 6 40 7 20 6 10 25 41 30 14 31 26 24 38 26 38 20 44 14 17 40 45 3 4 9 33 44 17 3 3 11 42 13 26 18 38 25 33 26 46 46 36 14 36 31 17 34 8 22 44 41 16 11 44 17 29 32 42 21 46 19 40 34 45 24 46 7 44 32 21 30 14 39 15 22 11 9 13 27 20 8 45 5 6
// ./practica2SG mapas/mapa75.map 1 4 11 47 0 6 47 4 9 51 19 33 29 31 33 31 54 43 51 67 59 68 57 29 47 16 68 6 29 4 3 32 14 25 57 20 41 35 47 27 38 71 68 47 53 10 18 10 60 11 16 38 62 45 22 47 39 37 57 39 57 30 66 21 26 60 68 5 6 14 49 66 26 4 4 17 63 19 40 28 57 38 50 39 69 69 54 21 55 47 26 51 12 33 66 62 24 17 67 26 51 71 44 49 64 32 69 29
// ./practica2SG mapas/mapa100.map 1 4 63 31 6 63 32 2 66 40 75 24 85 36 83 6 60 10 33 11 84 7 86 40 68 77 79 91 19 33 76 27 55 47 62 36 51 95 91 63 71 14 24 13 80 15 21 51 83 61 29 63 52 49 77 52 76 40 89 28 35 80 91 6 94 52 8 19 66 89 34 6 6 23 85 26 53 37 76 51 66 53 3 43
// ./practica2SG mapas/pinkworld.map 1 4 46 26 2 41 27 2 44 46 26 59 26 10 59 10 59 63 70 38 41 35 47 27 38 71 68 47 18 10 60 11 30 9 66 21 68 5 71 39 6 14 49 66 4 17 40 28 50 39 69 69 12 33 66 62 67 26 51 71 44 49 64 32 69 29 68 36
// ./practica2SG mapas/islas.map 1 4 47 53 2 49 53 2 41 56 52 53 74 54 74 47 46 42 71 56 83 52 58 65 85 43 92 39 81 68 91 48 21 95 92 14 88 64 43 61 28 78 30 44 22 18 27 55 41 16 90 10 12 49 76 68 38 74
// ./practica2SG mapas/marymonte.map 1 4 46 26 2 41 27 2 44 46 26 59 26 10 59 10 59 63 70 38 41 35 47 27 38 71 68 47 18 10 60 11 30 9 66 21 68 5 71 39 6 14 49 66 4 17 40 28 50 39 69 69 12 33 66 62 67 26 51 71 44 49 64 32 69 29 68 36
// ./practica2SG mapas/medieval.map 1 4 3 3 2 9 20 2 9 88 21 75 34 62 48 16 57 28 72 24 89 27 84 51 61 93 13 48 9 87 84 39 91 19 33 76 87 39 57 29 84 53 96 96 63 3 88 54 32 44 19 75 29 95 96 48 21 33 87 41 9 20 13 48 9 87 84 39 91 19 33 76 87 39
