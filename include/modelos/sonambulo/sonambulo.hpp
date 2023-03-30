#ifndef SONAMBULO3D_HPP
#define SONAMBULO3D_HPP

#include "modelos/sonambulo/torsoSonambulo.hpp"
#include "obj3dlib.hpp"
#include <string.h>


class Sonambulo3D : public Objeto3D{
private:

public:
  Sonambulo3D(const std::string & filename);
  ~Sonambulo3D();

};

#endif
