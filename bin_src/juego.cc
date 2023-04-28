#include <fstream>
#include "motorlib.hpp"

bool fileExists(std::string const &name)
{
  ifstream f(name.c_str());
  return f.good();
}

int main(int argc, char **argv)
{
  EnLinea argumentos;

  if (argc < 2)
  {
    srand(1);
    lanzar_motor_grafico(argc, argv);
  }
  else
  {
    srand(atoi(argv[2]));
    argumentos.ubicacion_mapa = argv[1];
    if (!fileExists(argumentos.ubicacion_mapa))
    {
      std::cout << "El mapa no existe\n";
      exit(1);
    }
    argumentos.semilla = atoi(argv[2]);
    argumentos.level = atoi(argv[3]);
    argumentos.posInicialJugador.f = atoi(argv[4]);
    argumentos.posInicialJugador.c = atoi(argv[5]);
    argumentos.posInicialJugador.brujula = static_cast<Orientacion>(atoi(argv[6]));
    argumentos.posInicialSonanbulo.f = atoi(argv[7]);
    argumentos.posInicialSonanbulo.c = atoi(argv[8]);
    argumentos.posInicialSonanbulo.brujula = static_cast<Orientacion>(atoi(argv[9]));
    
    int i = 10;

    while (argc > i + 1)
    {
      // Reviso si es una posicion valida
      argumentos.listaObjetivos.push_back(pair<int, int>(atoi(argv[i]), atoi(argv[i + 1])));
      i += 2;
    }

    lanzar_motor_grafico_verOnline(argc, argv, argumentos);
  }

  exit(EXIT_SUCCESS);
}
