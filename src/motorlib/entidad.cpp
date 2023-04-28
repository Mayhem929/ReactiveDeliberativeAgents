#include "motorlib/entidad.hpp"

void Entidad::resetEntidad()
{
  hitbox = false;
  if (tipo == jugador)
  {
    desactivado = 0;
  }
  else
  {
    desactivado = aleatorio(7) + 3;
  }

  colision = false;
  colisiones = 0;
  reset = true;
  if (vida != 0)
    muertes_innecesarias++;
  // vida = 0;
  done = false;

  mensaje = true;
}

int Entidad::isMemberObjetivo(unsigned int paramF, unsigned int paramC)
{
  int pos = -1;
  for (int i = 0; i < num_destinos and pos == -1; i++)
  {
    if (destino[2 * i] == paramF and destino[2 * i + 1] == paramC)
    {
      pos = i;
    }
  }
  if (pos != -1 and alcanzados[pos])
    pos = -1;

  return pos;
}

bool Entidad::allObjetivosAlcanzados()
{
  bool conseguido = true;
  for (int i = 0; i < num_destinos and conseguido; i++)
  {
    conseguido = conseguido and alcanzados[i];
  }
  return conseguido;
}

bool Entidad::allLessOneObjetivosAlcanzados()
{
  int num = 0;
  for (int i = 0; i < num_destinos; i++)
    if (alcanzados[i])
      num++;

  return (num == num_destinos - 1);
}

void Entidad::actualizarAlcanzados()
{
  int pos = isMemberObjetivo(f, c);
  if (pos != -1 and !alcanzados[pos])
  {
    alcanzados[pos] = true;
  }
}

void Entidad::anularAlcanzados()
{
  for (int i = 0; i < num_destinos; i++)
  {
    alcanzados[i] = false;
  }
}

void Entidad::setObjetivos(vector<unsigned int> v)
{
  destino = v;
  num_destinos = destino.size() / 2;
  for (int i = 0; i < num_destinos; i++)
  {
    alcanzados[i] = false;
  }
}

unsigned int Entidad::getObjFil(int pos)
{
  if (pos < num_destinos)
    return destino[2 * pos];
  else
  {
    std::cout << "Error en getObjFil: intento de acceso a una posición de objetivo que no existe\n";
    exit(1);
  }
}

unsigned int Entidad::getObjCol(int pos)
{
  if (pos < num_destinos)
    return destino[2 * pos + 1];
  else
  {
    std::cout << "Error en getObjFil: intento de acceso a una posición de objetivo que no existe\n";
    exit(1);
  }
}

Action Entidad::think(int acc, vector<vector<unsigned char>> vision, int level)
{
  Action accion = actIDLE;
  Sensores sensor;
  bool activ = false;

  if (desactivado == 0)
  {
    vida--;
    hitbox = true;

    sensor.nivel = level;

    sensor.vida = vida;
    sensor.bateria = bateria;

    sensor.destinoF = destino[0];
    sensor.destinoC = destino[1];

    sensor.colision = colision;
    sensor.reset = reset;

    if (tipo == jugador)
    {
      if (EntidadSonambulo->f == destino[0] and EntidadSonambulo->c == destino[1] and (level == 1 or level == 3 or level == 4))
      {
        if (level != 4)
        {
          done = true;
          misiones = 1;
        }
        else
        {
          misiones += 4;
          completoLosObjetivos = false;
        }
        cout << "Mision alcanzada por el sonámbulo!" << endl;
      }
      else if (f == destino[0] and c == destino[1] and (level == 0 or level == 2 or level == 4))
      {
        if (level != 4)
        {
          done = true;
          misiones = 1;
        }
        else
        {
          misiones++;
          completoLosObjetivos = false;
        }
        cout << "Mision alcanzada por el jugador!" << endl;
      }
      else if (sensor.bateria == 0)
      {
        done = true;
        cout << "Se agoto la bateria!" << endl;
      }
      else if (sensor.vida == 0)
      {
        done = true;
        cout << "Se agotaron los instantes de simulacion" << endl;
      }
      else if (getTiempo() > getTiempoMaximo())
      {
        done = true;
        cout << "Se agoto el tiempo de deliberacion!" << endl;
      }
    }

    sensor.ActionSent = actIDLE;

    if (subtipo == sonambulo)
    {
      // Poner en el sensor ActionSent lo que alguno de los otros agentes haya ordenado al sonambulo
      sensor.ActionSent = GetActionSent();
      SetActionSent(actIDLE);
    }

    activ = false;

    if (tipo == jugador)
    {
      if (!hasToNotify() and level == 4)
      {
        sensor.posF = -1;
        sensor.posC = -1;
        sensor.sentido = norte;
        sensor.SONposF = -1;
        sensor.SONposC = -1;
        sensor.SONsentido = norte;
        setNotifyOff();
      }
      else
      {
        sensor.posF = f;
        sensor.posC = c;
        sensor.sentido = orient;
        sensor.SONposF = EntidadSonambulo->f;
        sensor.SONposC = EntidadSonambulo->c;
        sensor.SONsentido = EntidadSonambulo->orient;
      }
    }
    else
    {
      sensor.posF = f;
      sensor.posC = c;
      sensor.sentido = orient;
      sensor.destinoF = destino[0];
      sensor.destinoC = destino[1];
    }

    sensor.superficie = vision[1];
    sensor.terreno = vision[0];

    sensor.tiempo = getTiempo() / CLOCKS_PER_SEC;

    visionAux = vision;

    if (!done)
    {
      if (acc == -1)
        accion = comportamiento->think(sensor);
      else
        accion = static_cast<Action>(acc);
    }
    colision = false;
    reset = false;
    mensaje = false;
  }
  else
  {
    desactivado--;
  }

  return accion;
}

bool Entidad::interact(Action accion, int valor)
{
  bool out = false;
  int retorno = comportamiento->interact(accion, valor);

  if (retorno == 1)
  { // Desaparición temporal
    resetEntidad();
    out = true;
  }

  if (retorno == 2)
  { // Muerte
    hitbox = false;
    desactivado = -1;
    out = true;
  }

  if (retorno == 3)
  { // Revivir
    hitbox = true;
    desactivado = 0;
    out = true;
  }

  return out;
}

unsigned char Entidad::getSubTipoChar()
{
  unsigned char out = ' ';

  switch (subtipo)
  {
  case jugador_:
    out = 'j';
    break;
  case aldeano:
    out = 'a';
    break;
  case sonambulo:
    out = 's';
    break;
  case lobo:
    out = 'l';
    break;
  }

  return out;
}

void Entidad::fixTiempo_sig_accion(unsigned char celda)
{
  tiempo_sig_accion = 1;
}

int Entidad::fixBateria_sig_accion(unsigned char celdaJugador, unsigned char celdaSonambulo, Action accion)
{
  bateria_sig_accion = 1;
  switch (accion)
  {
  case actIDLE:
    bateria_sig_accion = 0;
    break;
  case actFORWARD:
    switch (celdaJugador)
    {
    case 'A':
      if (Has_Bikini()) // Bikini
        bateria_sig_accion = 10;
      else
        bateria_sig_accion = 100;
      break;
    case 'B':
      if (Has_Zapatillas()) // Zapatillas
        bateria_sig_accion = 15;
      else
        bateria_sig_accion = 50;
      break;
    case 'T':
      bateria_sig_accion = 2;
      break;
    default:
      bateria_sig_accion = 1;
      break;
    } // Fin switch celdaJugador
    break;
  case actTURN_L:
  case actTURN_R:
    switch (celdaJugador)
    {
    case 'A':
      if (Has_Bikini()) // Bikini
        bateria_sig_accion = 5;
      else
        bateria_sig_accion = 25;
      break;
    case 'B':
      if (Has_Zapatillas()) // Zapatillas
        bateria_sig_accion = 1;
      else
        bateria_sig_accion = 5;
      break;
    case 'T':
      bateria_sig_accion = 2;
      break;
    default:
      bateria_sig_accion = 1;
      break;
    } // Fin switch celdaJugador
    break;
  case actTURN_SL:
  case actTURN_SR:
    switch (celdaSonambulo)
    {
    case 'A':
      if (Has_Bikini()) // Bikini
        bateria_sig_accion = 2;
      else
        bateria_sig_accion = 7;
      break;
    case 'B':
      if (Has_Zapatillas()) // Zapatillas
        bateria_sig_accion = 1;
      else
        bateria_sig_accion = 3;
      break;
    default:
      bateria_sig_accion = 1;
      break;
    } // Fin switch celdaJugador
    break;
  case actWHEREIS:
    bateria_sig_accion = 200;
    break;
  case actSON_FORWARD:
    switch (celdaSonambulo)
    {
    case 'A':
      if (EntidadSonambulo->Has_Bikini()) // Bikini sonámbulo
        bateria_sig_accion = 10;
      else
        bateria_sig_accion = 100;
      break;
    case 'B':
      if (EntidadSonambulo->Has_Zapatillas()) // Zapatillas sonámbulo
        bateria_sig_accion = 15;
      else
        bateria_sig_accion = 50;
      break;
    case 'T':
      bateria_sig_accion = 2;
      break;
    default:
      bateria_sig_accion = 1;
      break;
    } // Fin switch celdaJugador
    break;
  case actSON_TURN_SL:
  case actSON_TURN_SR:
    switch (celdaSonambulo)
    {
    case 'A':
      if (EntidadSonambulo->Has_Bikini()) // Bikini sonámbulo
        bateria_sig_accion = 2;
      else
        bateria_sig_accion = 7;
      break;
    case 'B':
      if (EntidadSonambulo->Has_Zapatillas()) // Zapatillas sonámbulo
        bateria_sig_accion = 1;
      else
        bateria_sig_accion = 3;
      break;
    default:
      bateria_sig_accion = 1;
      break;
    } // Fin switch celdaJugador
    break;
  }
  return bateria_sig_accion;
}

void Entidad::decBateria_sig_accion()
{
  bateria -= bateria_sig_accion;
  if (bateria < 0)
    bateria = 0;
}
