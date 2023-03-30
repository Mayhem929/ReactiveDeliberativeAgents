#ifndef COMPORTAMIENTOSONAMBULO_H
#define COMPORTAMIENTOSONAMBULO_H

#include <chrono>
#include <time.h>
#include <thread>

#include "comportamientos/comportamiento.hpp"

class ComportamientoSonambulo : public Comportamiento{

  public:
    ComportamientoSonambulo(unsigned int size = 0) : Comportamiento(size){
      paso=0;
    }
    ComportamientoSonambulo(const ComportamientoSonambulo & comport) : Comportamiento(comport){}
    ~ComportamientoSonambulo(){}

    Action think(Sensores sensores);

    int interact(Action accion, int valor);


    ComportamientoSonambulo * clone(){return new ComportamientoSonambulo(*this);}

  private:
    int paso;

};



#endif
