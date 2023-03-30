#ifndef COMPORTAMIENTO_H
#define COMPORTAMIENTO_H

#include <vector>

using namespace std;

enum Orientacion {norte, noreste, este, sureste, sur, suroeste, oeste, noroeste};
enum Action {actFORWARD, actTURN_L, actTURN_R, actTURN_SL, actTURN_SR, actWHEREIS, actSON_FORWARD, actSON_TURN_SL, actSON_TURN_SR, actIDLE};

struct ubicacion{
    int f;
    int c;
    Orientacion brujula;

    bool operator==(const ubicacion &ub)const {
      return (f == ub.f and c == ub.c and brujula == ub.brujula);
    }    
};

struct Sensores {
  int nivel;
  int vida;
  int bateria;
  bool colision;
  bool reset;
  int posF;
  int posC;
  Orientacion sentido; // Orientacion
  int SONposF;
  int SONposC;
  Orientacion SONsentido;
  Action ActionSent;
  double tiempo;
  vector< unsigned char> terreno;
  vector< unsigned char> superficie;
  int destinoF;
  int destinoC;
};



class Comportamiento {
  public:
    vector< vector< unsigned char> > mapaResultado;
    vector< vector< unsigned char> > mapaEntidades;
    std::vector< std::vector<unsigned char> > mapaConPlan;

    Comportamiento(unsigned int size);
    Comportamiento(vector< vector< unsigned char> > mapaR);
    Comportamiento(const Comportamiento & comport) : mapaEntidades(comport.mapaEntidades), mapaResultado(comport.mapaResultado), mapaConPlan(comport.mapaConPlan) {}
    Comportamiento * clone(){return new Comportamiento(*this);}
    ~Comportamiento(){}

    virtual Action think(Sensores sensores);

    virtual int interact(Action accion, int valor);

    vector< vector< unsigned char> > getMapaResultado(){return mapaResultado;}
    vector< vector< unsigned char> > getMapaEntidades(){return mapaEntidades;}
    vector< vector< unsigned char> > getMapaPlan(){return mapaConPlan;}

};

#endif
