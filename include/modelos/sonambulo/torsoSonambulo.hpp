#ifndef TORSOSONAMBULO3D_HPP
#define TORSOSONAMBULO3D_HPP

#include "obj3dlib.hpp"
#include "modelos/aldeano/cabeza.hpp"
#include "modelos/sonambulo/brazo.hpp"
#include "modelos/aldeano/cintura.hpp"
#include <string.h>


class TorsoSonambulo3D : public Objeto3D{
private:

public:
  TorsoSonambulo3D(const std::string & filename);
  ~TorsoSonambulo3D();

};

#endif
