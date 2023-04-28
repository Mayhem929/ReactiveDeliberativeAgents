#include "motorlib/monitor.hpp"

#include <fstream>

void MonitorJuego::cambiarOrientacion(unsigned char entidad, unsigned char orientacion)
{
  Orientacion aux;
  switch (orientacion)
  {
  case 0:
    aux = norte;
    break;
  case 1:
    aux = noreste;
    break;
  case 2:
    aux = este;
    break;
  case 3:
    aux = sureste;
    break;
  case 4:
    aux = sur;
    break;
  case 5:
    aux = suroeste;
    break;
  case 6:
    aux = oeste;
    break;
  case 7:
    aux = noroeste;
    break;
  }
  entidades[entidad]->setOrientacion(aux);
}

void MonitorJuego::setMapa(const char *file)
{
  ifstream ifile;
  ifile.open(file, ios::in);

  unsigned int colSize, filSize;
  ifile >> colSize;
  ifile >> filSize;

  vector<vector<unsigned char>> mapAux;
  vector<unsigned char> colAux(colSize);

  for (unsigned int i = 0; i < filSize; i++)
  {
    mapAux.push_back(colAux);
  }

  for (unsigned int i = 0; i < filSize; i++)
  {
    for (unsigned int j = 0; j < colSize; j++)
    {
      ifile >> mapAux[i][j];
    }
  }
  ifile.close();

  mapa = new Mapa(mapAux, &entidades);
}

bool MonitorJuego::inicializarJuego()
{
  bool aux = empezarJuego;
  if (empezarJuego)
  {
    empezarJuego = false;
    resultados = false;
  }
  return aux;
}

void MonitorJuego::generate_a_valid_cell(int &pos_fila, int &pos_col, int &ori)
{
  pos_col = -1;
  pos_fila = -1;
  char celdaRand = '_';
  do
  {
    pos_fila = aleatorio(getMapa()->getNFils() - 1);
    pos_col = aleatorio(getMapa()->getNCols() - 1);

    celdaRand = getMapa()->getCelda(pos_fila, pos_col);
  } while ((celdaRand == 'P' or celdaRand == 'M'));
  ori = aleatorio(7);
}

bool MonitorJuego::is_a_valid_cell_like_goal(int f, int c)
{
  if (f < 3 or f + 3 >= getMapa()->getNFils())
    return false;
  if (c < 3 or c + 3 >= getMapa()->getNCols())
    return false;
  char cell = getMapa()->getCelda(f, c);
  if (cell == 'M' or cell == 'P')
    return false;
  return true;
}

void MonitorJuego::generate_a_objetive()
{
  int pos_col = -1, pos_fila = -1;
  char celdaRand = '_';
  do
  {
    pos_fila = aleatorio(getMapa()->getNFils() - 1);
    pos_col = aleatorio(getMapa()->getNCols() - 1);

    celdaRand = getMapa()->getCelda(pos_fila, pos_col);
  } while ((celdaRand == 'P' or celdaRand == 'M'));

  pair<int, int> punto;
  punto.first = pos_fila;
  punto.second = pos_col;
  objetivos.push_back(punto);
}

void MonitorJuego::put_a_new_objetivo_front(int fila, int columna)
{
  pair<int, int> punto;
  punto.first = fila;
  punto.second = columna;
  objetivos.push_front(punto);
}

// Pasa de la lista de objetivos al vector de objetivos activos.
// 'number' establece el número de objetivos que se fijan simultáneamente
void MonitorJuego::put_active_objetivos(int number)
{
  vector<unsigned int> v;

  if (mapa != 0)
  {
    while (objetivos.size() < number)
    {
      generate_a_objetive();
    }
    for (int i = 0; i < number; i++)
    {
      auto it = objetivos.begin();
      v.push_back(it->first);
      v.push_back(it->second);
      objetivos.erase(objetivos.begin());
    }
  }
  else
  {
    v.push_back(3);
    v.push_back(3);
  }
  objetivosActivos = v;
}

void MonitorJuego::get_n_active_objetivo(int n, int &posFila, int &posCol)
{

    posFila = objetivosActivos[0];
    posCol = objetivosActivos[1];
}

void MonitorJuego::set_n_active_objetivo(int n, int posFila, int posCol)
{

    objetivosActivos[0] = posFila;
    objetivosActivos[1] = posCol;

  if (numero_entidades() > 0)
    get_entidad(0)->setObjetivos(objetivosActivos);
}

bool MonitorJuego::there_are_active_objetivo()
{
  return (objetivosActivos.size() > 0);
}

int MonitorJuego::get_number_active_objetivos()
{
  return (objetivosActivos.size() / 2);
}

vector<unsigned int> MonitorJuego::get_active_objetivos()
{
  vector<unsigned int> copia = objetivosActivos;
  return copia;
}

void MonitorJuego::decPasos()
{
  if (get_entidad(0)->fin())
  {
    if (nivel < 4)
    {
      jugando = false;
      resultados = true;
    }
    else
    {
      // Nivel 4
      if (!get_entidad(0)->vivo() or get_entidad(0)->getBateria() == 0 or get_entidad(0)->getTiempo() > get_entidad(0)->getTiempoMaximo())
      {
        jugando = false;
        resultados = true;
      }
      else
      {
        // Nuevo destino
        if (get_entidad(0)->allObjetivosAlcanzados())
        {
          /*put_active_objetivos(3);
          get_entidad(0)->anularAlcanzados();
          get_entidad(0)->setObjetivos(get_active_objetivos());*/
          // setObjCol(pos_col);
          // setObjFil(pos_fila);
          // if (editPosC != NULL)
          // editPosC->set_int_val(pos_col);
          // if (editPosF != NULL)
          // editPosF->set_int_val(pos_fila);
          // cout << "Nuevo objetivo: (" << pos_fila << "," << pos_col << ")" << endl;
          get_entidad(0)->resetFin();
        }

        if (pasos > 0)
        {
          pasos--;
          pasosTotales++;
        }
      }
    }
  }
  else
  {
    if (pasos > 0)
    {
      pasos--;
      pasosTotales++;
    }

    if (!get_entidad(0)->vivo() or get_entidad(0)->getBateria() == 0)
    {
      jugando = false;
      resultados = true;
    }
  }
}

bool MonitorJuego::CanISeeSonambunlo(int num_entidad)
{
  vector<unsigned char> superficie;
  superficie = getMapa()->vision(num_entidad)[1];
  int i = 1;
  while (i < 16 and superficie[i] != 's')
  {
    i++;
  }

  return (i != 16);
}

void MonitorJuego::inicializar(int pos_filaJ, int pos_colJ, int brujJ, int pos_filaS, int pos_colS, int brujS, int seed)
{
  clear();
  // srand(seed);
  // cout << "Estoy en MonitorJuego::inicializar\n";
  int tama = getMapa()->getNCols();

  int nsonambulos = 1;
  //int naldeanos = 0;
  //int nlobos = 0;
  int naldeanos = tama / 10;
  int nlobos = tama / 10;
  unsigned char celdaRand;

  // Se construye una lisa con 300 objetivos
  if (nivel == 4)
  {
    while (objetivos.size() < 300)
    {
      generate_a_objetive();
    }
  }

  // Primero SIEMPRE se coloca al jugador. SIEMPRE.
  if ((pos_filaJ == -1) or (pos_colJ == -1))
  {
    do
    {
      pos_filaJ = aleatorio(getMapa()->getNFils() - 1);
      pos_colJ = aleatorio(getMapa()->getNCols() - 1);
      celdaRand = getMapa()->getCelda(pos_filaJ, pos_colJ);
    } while ((celdaRand == 'P') or (celdaRand == 'M') or (getMapa()->entidadEnCelda(pos_filaJ, pos_colJ) != '_'));
  }
  else
  {
    // para hacer que la secuencia de recogida de numeros aleatorios sea independiente si da o no las coordenadas al principio
    int kkx = aleatorio(getMapa()->getNFils() - 1);
    int kky = aleatorio(getMapa()->getNCols() - 1);
  }

  // El segundo SIEMPRE es el sonambulo. Siempre. SIEMPRE.
  if ((pos_filaS == -1) or (pos_colS == -1))
  {
    do
    {
      pos_filaS = aleatorio(getMapa()->getNFils() - 1);
      pos_colS = aleatorio(getMapa()->getNCols() - 1);
      celdaRand = getMapa()->getCelda(pos_filaS, pos_colS);
    } while ((celdaRand == 'P') or (celdaRand == 'M') or (getMapa()->entidadEnCelda(pos_filaS, pos_colS) != '_'));
  }
  else
  {
    // para hacer que la secuencia de recogida de numeros aleatorios sea independiente si da o no las coordenadas al principio
    int kkx = aleatorio(getMapa()->getNFils() - 1);
    int kky = aleatorio(getMapa()->getNCols() - 1);
  }

  // Pongo los primeros objetivos objetivosActivos
  put_active_objetivos(1);

  // Para los primeros niveles hago el mapa visible
  if ((nivel == 0) or (nivel == 1) or (nivel == 2) or (nivel == 3))
  {
    vector<vector<unsigned char>> mAux(getMapa()->getNFils(), vector<unsigned char>(getMapa()->getNCols(), '?'));
    for (int i = 0; i < getMapa()->getNFils(); i++)
      for (int j = 0; j < getMapa()->getNCols(); j++)
        mAux[i][j] = getMapa()->getCelda(i, j);
    // Esto coloca al jugador en el mapa
    // cout << "bruj: " << bruj << endl;

    nueva_entidad(new Entidad(jugador, jugador_, static_cast<Orientacion>(brujJ), pos_filaJ, pos_colJ, new Jugador3D(""), new ComportamientoJugador(mAux), 1, objetivosActivos, 3000));
    nueva_entidad(new Entidad(npc, sonambulo, static_cast<Orientacion>(brujS), pos_filaS, pos_colS, new Sonambulo3D(""), new ComportamientoSonambulo(), 1, objetivosActivos, 3000));
  }
  else
  {
    // Esto coloca al jugador en el mapa
    if (brujJ == -1)
      brujJ = rand() % 8;
    if (brujS == -1)
      brujS = rand() % 8;

    nueva_entidad(new Entidad(jugador, jugador_, static_cast<Orientacion>(brujJ), pos_filaJ, pos_colJ, new Jugador3D(""), new ComportamientoJugador(getMapa()->getNFils()), 1, objetivosActivos, 3000));
    nueva_entidad(new Entidad(npc, sonambulo, static_cast<Orientacion>(brujS), pos_filaS, pos_colS, new Sonambulo3D(""), new ComportamientoSonambulo(), 1, objetivosActivos, 3000));
  }
  
  ReAparicionesEntidad( 0, pos_filaJ, pos_colJ, static_cast<Orientacion>(brujJ));
  ReAparicionesEntidad( 1, pos_filaS, pos_colS, static_cast<Orientacion>(brujS));
  get_entidad(0)->SetSonambulo(get_entidad(1));

  // Para el último nivel genero los aldeanos y lobos
  int pos_filaO, pos_colO;
  if (nivel == 4)
  {
    for (int i = 0; i < naldeanos; i++)
    {
      do
      {
        pos_filaO = aleatorio(getMapa()->getNFils() - 1);
        pos_colO = aleatorio(getMapa()->getNCols() - 1);
        celdaRand = getMapa()->getCelda(pos_filaO, pos_colO);
      } while ((celdaRand == 'P') or (celdaRand == 'M') or (getMapa()->entidadEnCelda(pos_filaO, pos_colO) != '_'));
      nueva_entidad(new Entidad(npc, aldeano, static_cast<Orientacion>(aleatorio(7)), pos_filaO, pos_colO, new Aldeano3D(""), new ComportamientoAldeano(), 1, objetivosActivos, 3000));
    }

    for (int i = 0; i < nlobos; i++)
    {
      do
      {
        pos_filaO = aleatorio(getMapa()->getNFils() - 1);
        pos_colO = aleatorio(getMapa()->getNCols() - 1);
        celdaRand = getMapa()->getCelda(pos_filaO, pos_colO);
      } while ((celdaRand == 'P') or (celdaRand == 'M') or (getMapa()->entidadEnCelda(pos_filaO, pos_colO) != '_'));
      nueva_entidad(new Entidad(npc, lobo, static_cast<Orientacion>(aleatorio(7)), pos_filaO, pos_colO, new Perro3D, new ComportamientoPerro(), 1, objetivosActivos, 3000));
    }
  }

  get_entidad(0)->setVision(getMapa()->vision(0));
  if ((nivel == 0) or (nivel == 1) or (nivel == 2) or (nivel == 3))
    get_entidad(0)->notify();

  // srand(seed);
  /* quitar esto una vez verificado */
  // PintaEstadoMonitor();
}

void MonitorJuego::ReAparicionesEntidad(int Entidad, int fila, int columna, Orientacion brujula)
{
  get_entidad(Entidad)->setPosicion(fila, columna);
  get_entidad(Entidad)->setOrientacion(brujula);
  get_entidad(Entidad)->setHitbox(true);
  get_entidad(Entidad)->Cogio_Bikini(false);
  get_entidad(Entidad)->Cogio_Zapatillas(false);
  switch (getMapa()->getCelda(fila, columna))
  {
  case 'X': // Casilla Rosa (Recarga)
    get_entidad(Entidad)->increaseBateria(10);
    break;

  case 'D': // Casilla Morada (Zapatillas)
    get_entidad(Entidad)->Cogio_Zapatillas(true);
    break;
  case 'K': // Casilla Amarilla (Bikini)
    get_entidad(Entidad)->Cogio_Bikini(true);
    break;
  }
}

void MonitorJuego::PintaEstadoMonitor()
{
  cout << "*********************************************\n";
  cout << "Pos Fil: " << get_entidad(0)->getFil() << endl;
  cout << "Pos Col: " << get_entidad(0)->getCol() << endl;
  cout << "Brujula: " << get_entidad(0)->getOrientacion() << endl;
  for (int i = 0; i < get_entidad(0)->getNumObj(); i++)
  {
    cout << "Obj " << i << " : F: " << get_entidad(0)->getObjFil(i) << "  C: " << get_entidad(0)->getObjCol(i) << endl;
  }
  cout << "aldeanos: " << numero_entidades() << endl;
  for (int i = 1; i < numero_entidades(); i++)
  {
    cout << "  Pos Fil: " << get_entidad(i)->getFil() << endl;
    cout << "  Pos Col: " << get_entidad(i)->getCol() << endl;
    cout << "  Brujula: " << get_entidad(i)->getOrientacion() << endl
         << endl;
  }
  cout << "*********************************************\n";
}

double MonitorJuego::CoincidenciaConElMapa()
{
  int aciertos = 0, totalCasillas = 0;

  for (unsigned int i = 0; i < getMapa()->getNFils(); i++)
  {
    for (unsigned int j = 0; j < getMapa()->getNCols(); j++)
    {
      if (getMapa()->getCelda(i, j) == get_entidad(0)->getMapaResultado()[i][j])
      {
        aciertos++;
      }
      else if (get_entidad(0)->getMapaResultado()[i][j] != '?')
      { // Puso un valor distinto de desconocido en mapaResultado y no acertó
        aciertos--;
      }
      totalCasillas++;
    }
  }
  return (aciertos * 100.0 / totalCasillas);
}

string strAccion(int accion)
{
  string out = "";

  switch (accion)
  {
  case 0:
    out = "actFORWARD";
    break;
  case 1:
    out = "actTURN_L";
    break;
  case 2:
    out = "actTURN_R";
    break;
  case 3:
    out = "actTURN_SL";
    break;
  case 4:
    out = "actTURN_SR";
    break;
  case 5:
    out = "actWHEREIS";
    break;
  case 6:
    out = "actSON_FORWARD";
    break;
  case 7:
    out = "actSON_TURN_SL";
    break;
  case 8:
    out = "actSON_TURN_SR";
    break;
  case 9:
    out = "actIDLE";
    break;
  }

  return out;
}

string paraDonde(const Orientacion &x)
{
  string aux;
  switch (x)
  {
  case norte:
    aux = "norte";
    break;
  case noreste:
    aux = "noreste";
    break;
  case este:
    aux = "este ";
    break;
  case sureste:
    aux = "sureste ";
    break;
  case sur:
    aux = "sur  ";
    break;
  case suroeste:
    aux = "suroeste  ";
    break;
  case oeste:
    aux = "oeste";
    break;
  case noroeste:
    aux = "noroeste";
    break;
  }
  return aux;
}

string MonitorJuego::toString()
{
  char aux[20];
  string saux;

  string str;

  sprintf(aux, "%d", get_entidad(0)->getInstantesPendientes());
  saux = aux;
  str += "Tiempo restante: " + saux + "\n";

  sprintf(aux, "%d", get_entidad(0)->getBateria());
  saux = aux;
  str += "Bateria: " + saux + "\n";

  string paDonde = paraDonde(get_entidad(0)->getOrientacion());

  str += "Posicion jugador: (F ";
  sprintf(aux, "%d", get_entidad(0)->getFil());
  saux = aux;
  str += saux;
  str += " , C ";
  sprintf(aux, "%d", get_entidad(0)->getCol());
  saux = aux;
  str += saux;
  str += " | ";
  str += paDonde;
  str += ")";
  str += "\n";

  string paDonde2 = paraDonde(get_entidad(1)->getOrientacion());

  str += "Posicion sonambulo: (F ";
  sprintf(aux, "%d", get_entidad(1)->getFil());
  saux = aux;
  str += saux;
  str += " , C ";
  sprintf(aux, "%d", get_entidad(1)->getCol());
  saux = aux;
  str += saux;
  str += " | ";
  str += paDonde2;
  str += ")";
  str += "\n";

  Action ac = get_entidad(0)->putLastAction();
  str += "Ultima Accion: " + strAccion(ac) + "\n";

  sprintf(aux, "%3.3f", get_entidad(0)->getTiempo() / CLOCKS_PER_SEC);
  saux = aux;

  str += "Tiempo Consumido: " + saux + "\n";

  string saux2;
  if (get_entidad(0)->Has_Zapatillas())
  {
    saux2 = "JUG ZAP ";
  }
  else
  {
    saux2 = "JUG --- ";
  }

  if (get_entidad(0)->Has_Bikini())
  {
    saux2 = saux2 + " BIK | ";
  }
  else
  {
    saux2 = saux2 + " --- | ";
  }

  if (get_entidad(1)->Has_Zapatillas())
  {
    saux2 = saux2 + "SON ZAP ";
  }
  else
  {
    saux2 = saux2 + "SON --- ";
  }

  if (get_entidad(1)->Has_Bikini())
  {
    saux2 = saux2 + " BIK \n";
  }
  else
  {
    saux2 = saux2 + " --- \n";
  }
  str += saux2;

  sprintf(aux, "(%d) %d", get_entidad(0)->getMisiones(), get_entidad(0)->getPuntuacion());
  str += "Puntuacion: ";
  saux = aux;
  str += saux;
  str += "\n";

  vector<vector<unsigned char>> visionAux = getMapa()->vision(0);

  if (visionAux.size() > 0)
  {
    // str += "************ Vision **************\n";

    for (unsigned int i = 0; i < visionAux[1].size(); i++)
    {
      str += visionAux[1][i];
      str += " ";
    }

    str += "\n";

    for (unsigned int i = 0; i < visionAux[0].size(); i++)
    {
      str += visionAux[0][i];
      str += " ";
    }

    str += "\n";
  }

  return str;
}


void MonitorJuego::init_casillas_especiales(unsigned int f, unsigned int c){
  unsigned char celda_inicial;
  celda_inicial = getMapa()->getCelda(f, c);
  if (celda_inicial == 'D'){
    get_entidad(0)->Cogio_Zapatillas(true);
  }
  else if (celda_inicial == 'K'){
    get_entidad(0)->Cogio_Bikini(true);
  }
}