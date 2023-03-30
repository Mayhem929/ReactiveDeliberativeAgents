#include "motorlib.hpp"

#include <unistd.h>

extern MonitorJuego monitor;

bool actuacionJugador(unsigned char celdaJ_inicial, unsigned char celdaJ_fin, unsigned char celdaS_inicial, unsigned char celdaS_fin, Action accion, unsigned int x, unsigned int y)
{
  unsigned int mix, miy;
  unsigned char celdaRand;
  bool salida = false;
  int gasto = 0;
  bool SonambuloEnVision = false;

  if (accion == actSON_FORWARD or accion == actSON_TURN_SL or accion == actSON_TURN_SR)
  {
    if (monitor.CanISeeSonambunlo(0))
    {
      gasto = monitor.get_entidad(0)->fixBateria_sig_accion(celdaJ_inicial, celdaS_inicial, accion);
      SonambuloEnVision = true;
    }
  }
  else
    gasto = monitor.get_entidad(0)->fixBateria_sig_accion(celdaJ_inicial, celdaS_inicial, accion);

  if (gasto > monitor.get_entidad(0)->getBateria())
  {
    monitor.get_entidad(0)->setBateria(0);
    return false;
  }

  switch (accion)
  {
  case actFORWARD:
    if (monitor.getMapa()->casillaOcupada(0) == -1) // Casilla destino desocupada
    {
      switch (celdaJ_fin)
      {
      case 'M': // Muro
        monitor.get_entidad(0)->seAostio();
        break;
      case 'P': // Precipicio
        cout << "Se cayó por un precipicio\n";
        monitor.get_entidad(0)->resetEntidad();
        monitor.get_entidad(0)->setHitbox(true);
        monitor.finalizarJuego();
        monitor.setMostrarResultados(true);
        break;
      case 'X': // Casilla Rosa (Recarga)
        // monitor.get_entidad(0)->increaseBateria(10);
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      case 'D': // Casilla Morada (Zapatillas)
        // tomo la zapatilla
        monitor.get_entidad(0)->Cogio_Zapatillas(true);
        // pierdo el bikini
        monitor.get_entidad(0)->Cogio_Bikini(false);
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      case 'K': // Casilla Amarilla (Bikini)
        // tomo el bikini
        monitor.get_entidad(0)->Cogio_Bikini(true);
        // pierdo las zapatillas
        monitor.get_entidad(0)->Cogio_Zapatillas(false);
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      case 'G': // Casilla celeste claro (GPS)
        monitor.get_entidad(0)->notify();
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;

      case 'B': // Arbol
      case 'A': // Agua
      default:
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      }
      if (monitor.get_entidad(0)->isMemberObjetivo(x, y) != -1 and monitor.get_entidad(0)->allLessOneObjetivosAlcanzados())
      {
        // acaba de completar todos los objetivos.
        monitor.get_entidad(0)->setCompletoLosObjetivos();
        if (monitor.getLevel() == 4)
        {
          monitor.put_active_objetivos(1);
          monitor.get_entidad(0)->anularAlcanzados();
          monitor.get_entidad(0)->setObjetivos(monitor.get_active_objetivos());
          monitor.get_entidad(0)->incrMisiones();
          monitor.get_entidad(0)->incrPuntuacion(1);
        }
      }
      // monitor.get_entidad(0)->fixBateria_sig_accion(celdaJ_inicial, accion);
    }
    else
    {
      // Choca contra una entidad
      monitor.get_entidad(0)->seAostio();
      if (monitor.get_entidad(monitor.getMapa()->casillaOcupada(0))->getSubTipo() == aldeano)
      {
        monitor.get_entidad(0)->perderPV(1);
      }
      else if (monitor.get_entidad(monitor.getMapa()->casillaOcupada(0))->getSubTipo() == sonambulo)
      {
        monitor.get_entidad(0)->perderPV(1);
      }
      else if (monitor.get_entidad(monitor.getMapa()->casillaOcupada(0))->getSubTipo() == lobo)
      {
        // Opcion reiniciarse en otro punto del mapa
        /*do
        {
          mix = aleatorio(monitor.getMapa()->getNFils() - 1);
          miy = aleatorio(monitor.getMapa()->getNCols() - 1);

          celdaRand = monitor.getMapa()->getCelda(mix, miy);
        } while (celdaRand == 'P' or celdaRand == 'M' or monitor.getMapa()->entidadEnCelda(mix, miy) != '_');

        monitor.get_entidad(0)->resetEntidad();
        monitor.get_entidad(0)->setPosicion(mix, miy);
        monitor.get_entidad(0)->setOrientacion(static_cast<Orientacion>(aleatorio(7)));
        monitor.get_entidad(0)->setHitbox(true);
        monitor.get_entidad(0)->Cogio_Bikini(false);
        monitor.get_entidad(0)->Cogio_Zapatillas(false);*/

        // Opcion simplemente choca contra el lobo
        monitor.get_entidad(0)->perderPV(1);
      }
      salida = false;
    }
    break;

  case actTURN_R:
    monitor.get_entidad(0)->giroDch();
    monitor.girarJugadorDerecha(90);
    // monitor.get_entidad(0)->fixBateria_sig_accion(celdaJ_inicial, accion);
    salida = true;
    break;

  case actTURN_SR:
    // monitor.get_entidad(0)->giro45Dch();
    // monitor.girarJugadorDerecha(45);
    cout << "actTURN_SR no es una acción que pueda realizar el jugador\n";
    salida = true;
    break;

  case actTURN_L:
    monitor.get_entidad(0)->giroIzq();
    monitor.girarJugadorIzquierda(90);
    // monitor.get_entidad(0)->fixBateria_sig_accion(celdaJ_inicial, accion);
    salida = true;
    break;

  case actTURN_SL:
    // monitor.get_entidad(0)->giro45Izq();
    // monitor.girarJugadorIzquierda(45);
    cout << "actTURN_SL no es una acción que pueda realizar el jugador\n";
    salida = true;
    break;

  case actWHEREIS:
    monitor.get_entidad(0)->notify();
    // monitor.get_entidad(0)->fixBateria_sig_accion(celdaJ_inicial, celdaS_inicial, accion);
    salida = true;
    break;

  case actIDLE:
    if (celdaJ_inicial == 'X')
    { // Casilla Rosa (Recarga)
      monitor.get_entidad(0)->increaseBateria(10);
    }
    // monitor.get_entidad(0)->fixBateria_sig_accion(celdaJ_inicial, accion);

    salida = true;
    break;

  case actSON_FORWARD:
    if (SonambuloEnVision)
      monitor.get_entidad(1)->SetActionSent(actFORWARD);
    else
    {
      cout << "El sonambulo no está en la vision: actSON_FORWARD\n";
    }
    salida = true;
    break;

  case actSON_TURN_SL:
    if (SonambuloEnVision)
      monitor.get_entidad(1)->SetActionSent(actTURN_SL);
    else
    {
      cout << "El sonambulo no está en la vision: actSON_TURN_SL\n";
    }
    salida = true;
    break;

  case actSON_TURN_SR:
    if (SonambuloEnVision)
      monitor.get_entidad(1)->SetActionSent(actTURN_SR);
    else
    {
      cout << "El sonambulo no está en la vision: actSON_TURN_SR\n";
    }
    salida = true;
    break;
  }

  return salida;
}

bool actuacionNPC(unsigned int entidad, unsigned char celda, Action accion, unsigned int x, unsigned int y)
{
  bool out = false;
  switch (monitor.get_entidad(entidad)->getSubTipo())
  {
  case aldeano: // Acciones para el aldeano
    switch (accion)
    {
    case actFORWARD:
      if ((celda != 'P' and celda != 'M') and monitor.getMapa()->casillaOcupada(entidad) == -1)
      {
        monitor.get_entidad(entidad)->setPosicion(x, y);
        out = true;
      }
      break;

    case actTURN_R:
      monitor.get_entidad(entidad)->giroDch();
      out = true;
      break;

    case actTURN_L:
      monitor.get_entidad(entidad)->giroIzq();
      out = true;
      break;

    case actTURN_SR:
      monitor.get_entidad(entidad)->giro45Dch();
      out = true;
      break;

    case actTURN_SL:
      monitor.get_entidad(entidad)->giro45Izq();
      out = true;
      break;
    }
    break;

  case sonambulo: // Acciones para el sonambulo
    switch (accion)
    {
    case actFORWARD:
      if (monitor.getMapa()->casillaOcupada(1) == -1) // Casilla destino desocupada
      {
        switch (celda)
        {
        case 'M': // Muro
          monitor.get_entidad(0)->seAostio();
          break;
        case 'P': // Precipicio
          cout << "Se cayó por un precipicio el sonámbulo\n";
          monitor.get_entidad(1)->resetEntidad();
          monitor.get_entidad(1)->setHitbox(true);
          monitor.finalizarJuego();
          monitor.setMostrarResultados(true);
          break;
        case 'X': // Casilla Rosa (Recarga)
          monitor.get_entidad(0)->increaseBateria(10);
          monitor.get_entidad(1)->setPosicion(x, y);
          out = true;
          break;
        case 'D': // Casilla Morada (Zapatillas)
          // tomo la zapatilla
          monitor.get_entidad(1)->Cogio_Zapatillas(true);
          // pierdo el bikini
          monitor.get_entidad(1)->Cogio_Bikini(false);
          monitor.get_entidad(1)->setPosicion(x, y);
          out = true;
          break;
        case 'K': // Casilla Amarilla (Bikini)
          // tomo el bikini
          monitor.get_entidad(1)->Cogio_Bikini(true);
          // pierdo las zapatillas
          monitor.get_entidad(1)->Cogio_Zapatillas(false);
          monitor.get_entidad(1)->setPosicion(x, y);
          out = true;
          break;
        case 'G': // Casilla celeste claro (GPS)
          monitor.get_entidad(1)->notify();
          monitor.get_entidad(1)->setPosicion(x, y);
          out = true;
          break;

        case 'B': // Arbol
        case 'A': // Agua
        default:
          monitor.get_entidad(1)->setPosicion(x, y);
          out = true;
          break;
        }
        if (monitor.get_entidad(0)->isMemberObjetivo(x, y) != -1 and monitor.get_entidad(0)->allLessOneObjetivosAlcanzados())
        {
          // acaba de completar todos los objetivos.
          monitor.get_entidad(0)->setCompletoLosObjetivos();
          if (monitor.getLevel() == 4)
          {
            monitor.put_active_objetivos(1);
            monitor.get_entidad(0)->anularAlcanzados();
            monitor.get_entidad(0)->setObjetivos(monitor.get_active_objetivos());
            monitor.get_entidad(0)->incrMisiones();
            monitor.get_entidad(0)->incrPuntuacion(10);
          }
        }
      }
      else
      {
        // Choca contra una entidad
        monitor.get_entidad(1)->seAostio();

        SubTipo agente = monitor.get_entidad(monitor.getMapa()->casillaOcupada(1))->getSubTipo();

        if (agente == aldeano or agente == jugador_ or agente == lobo)
        {
          monitor.get_entidad(0)->perderPV(1);
          switch (agente)
          {
          case aldeano:
            cout << "El sonámbulo chocó contra un aldeano\n";
            break;
          case jugador_:
            cout << "El sonámbulo chocó contra el jugador\n";
            break;
          case lobo:
            cout << "El sonámbulo chocó contra un lobo\n";
            break;
          }
          // Opcion reiniciarse en otro punto del mapa
          int mix, miy;
          unsigned char celdaRand;
          do
          {
            mix = aleatorio(monitor.getMapa()->getNFils() - 1);
            miy = aleatorio(monitor.getMapa()->getNCols() - 1);

            celdaRand = monitor.getMapa()->getCelda(mix, miy);
          } while (celdaRand == 'P' or celdaRand == 'M' or monitor.getMapa()->entidadEnCelda(mix, miy) != '_');

          monitor.get_entidad(0)->resetSignal(); // Envía señal de reseto del sonambulo
          monitor.get_entidad(1)->resetEntidad();
          monitor.get_entidad(1)->setPosicion(mix, miy);
          monitor.get_entidad(1)->setOrientacion(static_cast<Orientacion>(aleatorio(7)));
          monitor.get_entidad(1)->setHitbox(true);
          monitor.get_entidad(1)->Cogio_Bikini(false);
          monitor.get_entidad(1)->Cogio_Zapatillas(false);

          // Opcion simplemente choca contra el lobo
          monitor.get_entidad(0)->perderPV(1);
        }
        out = false;
      }
      break;

    case actTURN_R:
      // monitor.get_entidad(1)->giroDch();
      cout << "actTURN_R no es una acción que pueda realizar el sonámbulo\n";

      out = true;
      break;

    case actTURN_L:
      // monitor.get_entidad(1)->giroIzq();
      cout << "actTURN_L no es una acción que pueda realizar el sonámbulo\n";
      out = true;
      break;

    case actTURN_SR:
      monitor.get_entidad(1)->giro45Dch();
      out = true;
      break;

    case actTURN_SL:
      monitor.get_entidad(1)->giro45Izq();
      out = true;
      break;

    case actWHEREIS:
      monitor.get_entidad(1)->notify();
      out = true;
      break;

    case actIDLE:
      if (celda == 'X')
      { // Casilla Rosa (Recarga)
        monitor.get_entidad(0)->increaseBateria(10);
      }

      out = true;
      break;
    }
    break;
  case lobo: // Acciones para el lobo
    switch (accion)
    {
    case actFORWARD:
      if ((celda != 'P' and celda != 'M') and monitor.getMapa()->casillaOcupada(entidad) == -1)
      {
        monitor.get_entidad(entidad)->setPosicion(x, y);
        out = true;
      }
      break;

    case actTURN_R:
      monitor.get_entidad(entidad)->giroDch();
      out = true;
      break;

    case actTURN_L:
      monitor.get_entidad(entidad)->giroIzq();
      out = true;
      break;

    case actTURN_SR:
      monitor.get_entidad(entidad)->giro45Dch();
      out = true;
      break;

    case actTURN_SL:
      monitor.get_entidad(entidad)->giro45Izq();
      out = true;
      break;

    case actWHEREIS: // Esta accion para un lobo es empujar equivalente a un actPUSH
      cout << "Recibido un empujon por un lobo\n";
      bool esta_jugador_delante = monitor.getMapa()->casillaOcupada(entidad) == 0;
      monitor.get_entidad(0)->seAostio();
      if (esta_jugador_delante)
      {
        pair<int, int> casilla = monitor.getMapa()->NCasillasDelante(entidad, 2);
        if (monitor.getMapa()->QuienEnCasilla(casilla.first, casilla.second) == -1 and
            monitor.getMapa()->getCelda(casilla.first, casilla.second) != 'M' and
            monitor.getMapa()->getCelda(casilla.first, casilla.second) != 'P')
          if (aleatorio(2) == 1)
          { // Solo ocurre la mitad de las veces que el lobo lo intenta.
            monitor.get_entidad(0)->setPosicion(casilla.first, casilla.second);
            monitor.get_entidad(0)->Increment_Empujones();
            monitor.get_entidad(entidad)->giroIzq();
            monitor.get_entidad(entidad)->giro45Izq();
          }
      }
      out = true;
      break;
    }
  }

  return out;
}

pair<int, int> NextCoordenadas(int f, int c, Orientacion brujula)
{
  switch (brujula)
  {
  case norte:
    f = f - 1;
    c = c;
    break;

  case noreste:
    f = f - 1;
    c = c + 1;
    break;

  case este:
    f = f;
    c = c + 1;
    break;

  case sureste:
    f = f + 1;
    c = c + 1;
    break;

  case sur:
    f = f + 1;
    c = c;
    break;

  case suroeste:
    f = f + 1;
    c = c - 1;
    break;

  case oeste:
    f = f;
    c = c - 1;
    break;

  case noroeste:
    f = f - 1;
    c = c - 1;
    break;
  }
  pair<int, int> coordenadas;
  coordenadas.first = f;
  coordenadas.second = c;
  return coordenadas;
}

bool actuacion(unsigned int entidad, Action accion)
{
  bool out = false;
  unsigned char celdaJ_inicial, celdaJ_fin, celdaS_inicial, celdaS_fin, objetivo;
  unsigned int f, c;

  f = monitor.get_entidad(entidad)->getFil();
  c = monitor.get_entidad(entidad)->getCol();

  celdaJ_inicial = monitor.getMapa()->getCelda(f, c);

  // Calculamos cual es la celda justo frente a la entidad
  pair<unsigned int, unsigned int> coord = NextCoordenadas(f, c, monitor.get_entidad(entidad)->getOrientacion());
  f = coord.first;
  c = coord.second;
  celdaJ_fin = monitor.getMapa()->getCelda(f, c);

  // Dependiendo el tipo de la entidad actuamos de una forma u otra
  pair<unsigned int, unsigned int> coordS;
  unsigned int sf, sc;
  switch (monitor.get_entidad(entidad)->getSubTipo())
  {
  case jugador_:
    sf = monitor.get_entidad(1)->getFil();
    sc = monitor.get_entidad(1)->getCol();

    celdaS_inicial = monitor.getMapa()->getCelda(sf, sc);

    // Calculamos cual es la celda justo frente a la entidad
    coordS = NextCoordenadas(sf, sc, monitor.get_entidad(1)->getOrientacion());
    sf = coordS.first;
    sc = coordS.second;
    celdaS_fin = monitor.getMapa()->getCelda(sf, sc);
    out = actuacionJugador(celdaJ_inicial, celdaJ_fin, celdaS_inicial, celdaS_fin, accion, f, c);
    break;

  case sonambulo:
    out = actuacionNPC(entidad, celdaJ_fin, accion, f, c);
    break;

  case aldeano:
    out = actuacionNPC(entidad, celdaJ_fin, accion, f, c);
    break;

  case lobo:
    out = actuacionNPC(entidad, celdaJ_fin, accion, f, c);
    break;
  }

  return out;
}

void nucleo_motor_juego(MonitorJuego &monitor, int acc)
{
  Action accion;
  unsigned char celdaRand;

  vector<vector<vector<unsigned char>>> estado;

  estado.clear();

  for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
  {
    estado.push_back(monitor.getMapa()->vision(i));
  }

  if (monitor.get_entidad(0)->ready())
  {

    monitor.init_casillas_especiales(monitor.get_entidad(0)->getFil(), monitor.get_entidad(0)->getCol());

    clock_t t0 = clock();
    accion = monitor.get_entidad(0)->think(acc, estado[0], monitor.getLevel());
    clock_t t1 = clock();

    monitor.get_entidad(0)->addTiempo(t1 - t0);
    monitor.get_entidad(0)->getLastAction(accion);
    actuacion(0, accion);
  }
  else
  {
    monitor.get_entidad(0)->perderPV(1);
  }

  for (unsigned int i = 1; i < monitor.numero_entidades(); i++)
  {
    clock_t t0 = clock();
    accion = monitor.get_entidad(i)->think(-1, estado[i], monitor.getLevel());
    clock_t t1 = clock();

    monitor.get_entidad(i)->addTiempo(t1 - t0);
    actuacion(i, accion);
  }

  for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
  {
    monitor.get_entidad(i)->setVision(monitor.getMapa()->vision(i));
  }

  monitor.get_entidad(0)->decBateria_sig_accion();
  monitor.decPasos();

  if (acc != -1)
  {
    // sleep(monitor.getRetardo() / 10);
  }
}

bool lanzar_motor_juego(int &colisiones, int acc)
{
  bool out = false;

  if (monitor.continuarBelkan())
  {
    if (monitor.jugar())
    {
      if ((monitor.getPasos() != 0) and (!monitor.finJuego()))
      {
        nucleo_motor_juego(monitor, acc);
      }
    }

    if (monitor.mostrarResultados())
    {
      cout << "Instantes de simulacion no consumidos: " << monitor.get_entidad(0)->getInstantesPendientes() << endl;
      cout << "Tiempo Consumido: " << 1.0 * monitor.get_entidad(0)->getTiempo() / CLOCKS_PER_SEC << endl;
      cout << "Nivel Final de Bateria: " << monitor.get_entidad(0)->getBateria() << endl;
      cout << "Colisiones: " << monitor.get_entidad(0)->getColisiones() << endl;
      cout << "Empujones: " << monitor.get_entidad(0)->getEmpujones() << endl;
      cout << "Porcentaje de mapa descubierto: " << monitor.CoincidenciaConElMapa() << endl;
      cout << "Objetivos encontrados: " << monitor.get_entidad(0)->getMisiones() << endl;
      monitor.setMostrarResultados(false);

      out = true;
    }
  }
  return out;
}

void lanzar_motor_juego2(MonitorJuego &monitor)
{

  // monitor.get_entidad(0)->setObjetivo(monitor.getObjFil(), monitor.getObjCol());

  while (!monitor.finJuego() && monitor.jugar())
  {
    nucleo_motor_juego(monitor, -1);
  }

  if (monitor.mostrarResultados() and monitor.getLevel() < 2)
  {
    cout << "Longitud del camino: " << 3001 - monitor.get_entidad(0)->getInstantesPendientes() << endl;
    monitor.setMostrarResultados(false);
  }
  else if (monitor.mostrarResultados() and monitor.getLevel() < 4)
  {
    cout << "Coste de Bateria: " << 3000 - monitor.get_entidad(0)->getBateria() << endl;
    monitor.setMostrarResultados(false);
  }
  else if (monitor.mostrarResultados())
  {
    cout << "Instantes de simulacion no consumidos: " << monitor.get_entidad(0)->getInstantesPendientes() << endl;
    cout << "Tiempo Consumido: " << 1.0 * monitor.get_entidad(0)->getTiempo() / CLOCKS_PER_SEC << endl;
    cout << "Nivel Final de Bateria: " << monitor.get_entidad(0)->getBateria() << endl;
    cout << "Colisiones: " << monitor.get_entidad(0)->getColisiones() << endl;
    cout << "Empujones: " << monitor.get_entidad(0)->getEmpujones() << endl;
    cout << "Porcentaje de mapa descubierto: " << monitor.CoincidenciaConElMapa() << endl;
    cout << "Objetivos encontrados: " << monitor.get_entidad(0)->getMisiones() << endl;
    monitor.setMostrarResultados(false);
  }
}
