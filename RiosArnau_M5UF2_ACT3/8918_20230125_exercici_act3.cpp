/*
Nom: Arnau Rios Buil
S'ha d'optimitzar aquest codi tot el que pugueu.
Fer un document PDF on s'explica detalladament com estava el codi i quina millora heu fet.
Podeu modificar el codi tot el que vulgueu, però ha de mantenir les mateixes funcionalitats, no pot tenir noves ni tenir menys, el programa ha de fer el mateix.
*/

#include <iostream>
#include <cstdlib> //for rand and srand
#include <ctime> // time function for seed value
#include <chrono>
#include <thread>

using namespace std;

// Estructura carta
struct carta
{
  int numero;
  string palo;
};

// Capçaleres de les funcions utilitzades
void mostrar_resultats(carta mano[], int contador);
void resultat_punts(int puntos, bool repetir);
int puntuacio(carta mano[], int contadorMano, int contadorAs, int puntos, int num_cartas_mano);

int main()
{
  // Variable baralla amb 52 posicions, una per cada carta.
  struct carta baraja[52];
  // Variable necessària perquè no surtin cartes repetides amb la funció respectiva.
  time_t t;
  srand((unsigned)time(&t));
  // Array amb els possibles pals de les cartes.
  const string palos[4] = {"Corazones", "Diamantes", "Picas", "Treboles"};

  int contador = 0;

  // Es guarda a l'array baralla totes les cartes
  for (int i = 0; i < 13; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      baraja[contador].numero = i + 1;
      baraja[contador].palo = palos[j];
      contador++;
    }
  }

  // Variable per escollir opció al menú
  int opcion;
  // Variable boolean per saber si s'ha de repetir el menú
  bool repetir = true;
  // Variables per la mà de l'usuari i la màquina
  struct carta mano[52], manoMaquina[52];
  // Variables auxiliars
  int num_cartas_mano, carta_mano_maquina, puntos, contadorAs, puntosMaquina;

  // Inicialment es reparteixen 2 cartes a l'usuari de les 52 que té la baralla
  for (int contadorBaraja = 0; contadorBaraja < 2; contadorBaraja++)
  {
    mano[contadorBaraja] = baraja[rand() % 52];
  }

  // Comença el menú
  do
  {
    cout << "\033[1;31m \n\nMenu de Opciones \033[0m" << endl;
    cout << "\033[1;32m 1. Robar carta \033[0m" << endl;
    cout << "\033[1;32m 2. Mostrar mano \033[0m" << endl;
    cout << "\033[1;32m 3. Plantarse \033[0m" << endl;
    cout << "\033[1;32m 4. Ver baraja \033[0m" << endl;
    cout << "\033[1;32m 0. SALIR \033[0m" << endl;

    cout << "\033[1;31m \nTus cartas son: \033[0m" << endl;
    // Variable que porta la compta dels punts del jugador.
    puntos = 0;
    // Variable que porta la compta dels AS.
    contadorAs = 0;

    // Recorre la mà de l'usuari
    for (int contadorMano = 0; contadorMano < num_cartas_mano + 1; contadorMano++)
    {
      // Si la carta de l'usuari és un pal dels quatre pals
      if (mano[contadorMano].palo != "")
      {
        // Calulem els punts i sumem una carta a la mà de l'usuari.
        puntos = puntuacio(mano, contadorMano,contadorAs,puntos,num_cartas_mano);
        num_cartas_mano++;
      }
    }

    // Mostrem resultats
    resultat_punts(puntos, repetir);

    // Si tens 21 punts o més ja no pots jugar (robar cartes).
    if (puntos >= 21)
    {
      break;
    }

    cout << "\nIngrese una opcion: ";
    cin >> opcion;

    switch (opcion)
    {
    // Robar carta
    case 1:
      system("clear");
      // Afegeixes una carta aleatòria a la mà
      mano[num_cartas_mano + 1] = baraja[rand() % 52];
      cout << "\033[1;32mTu carta es: \033[0m" << endl;

      // Es torna a comprovar la suma de punts depenent de la carta que et toca
      puntos = puntuacio(mano, num_cartas_mano, contadorAs, puntos, num_cartas_mano);

      // Tornem a mostrar els resultats
      resultat_punts(puntos, repetir);
      if (puntos >= 21)
      {
        break;
      }

    // Mostrar cartes
    case 2:
      system("clear");
      cout << "\033[1;31m \nTus cartas son: \033[0m" << endl;
      puntos = 0;
      contadorAs = 0;
      for (int contadorMano = 0; contadorMano < num_cartas_mano; contadorMano++)
      {
        if (mano[contadorMano].palo != "")
        {
          puntos = puntuacio(mano, contadorMano, contadorAs, puntos, num_cartas_mano);
        }
      }
      cout << "\033[1;32mTienes " << puntos << " puntos \033[0m" << endl;
      break;

    // Plantar-se (la màquina jugarà fins igualar-te o superar-te en punts)
    case 3:
      system("clear");
      cout << "\033[1;32mHas conseguido " << puntos << " puntos \033[0m" << endl;
      cout << "\033[1;36mAhora juega la máquina\033[0m" << endl;
      carta_mano_maquina = 0;
      puntosMaquina = 0;
      while (puntosMaquina < puntos)
      {
        cout << "\033[1;36mLa maquina roba carta\033[0m" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        manoMaquina[carta_mano_maquina] = baraja[rand() % 52];
        mostrar_resultats(manoMaquina, carta_mano_maquina);
        puntosMaquina = puntosMaquina + manoMaquina[carta_mano_maquina].numero; // La forma en la que està calculat els punts de la màquina és una mica trampós. Si li toco el rey, suma 13 punts però l'usuari només en suma 10. No toco res per si afecta canviar a la funcionalitat del programa.
        cout << "\033[1;37m Puntos maquina: " << puntosMaquina << endl;
        carta_mano_maquina++;
      }
      if (puntosMaquina > 21)
      {
        cout << "\033[1;31mLa máquina se ha pasado\033[0m" << endl;
        cout << "\033[1;33mFELICIDADES, HAS GANADO!\033[0m" << endl;
      }
      else if (puntosMaquina == puntos)
      {
        cout << "\033[1;33mLa máquina se planta\033[0m" << endl;
        cout << "\033[1;33mEmpate\033[0m" << endl;
      }
      else
      {
        cout << "\033[1;33mLa máquina se planta\033[0m" << endl;
        cout << "\033[1;31mHas perdido...\033[0m" << endl;
      }
      cout << "\033[1;35mTu has conseguido " << puntos << " puntos y la máquina " << puntosMaquina << " puntos\033[0m" << endl;
      repetir = false;
      break;
    // Veure la baralla senceras
    case 4:
      for (int contadorBaraja = 0; contadorBaraja < 52; contadorBaraja++)
      {
        mostrar_resultats(baraja, contadorBaraja);
      }
      break;
    case 0:
      repetir = false;
      break;
    }
  } while (repetir);

  return 0;
}

// Funcions
// Amb aquesta funció mostraràs les cartes.
void mostrar_resultats(carta mano[], int contador)
{
  if (mano[contador].numero == 1)
  {
    cout << "\033[1;33m A de " << mano[contador].palo << "\033[0m" << endl;
  }
  else if (mano[contador].numero == 11)
  {
    cout << "\033[1;33m Sota de " << mano[contador].palo << "\033[0m" << endl;
  }
  else if (mano[contador].numero == 12)
  {
    cout << "\033[1;33m Caballo de " << mano[contador].palo << "\033[0m" << endl;
  }
  else if (mano[contador].numero == 13)
  {
    cout << "\033[1;33m Rey de " << mano[contador].palo << "\033[0m" << endl;
  }
  else
  {
    cout << "\033[1;33m " << mano[contador].numero << " de " << mano[contador].palo << "\033[0m" << endl;
  }
}

// Amb aquesta funció mostraràs el resultat de la partida
void resultat_punts(int puntos, bool repetir)
{
  // Més de 21 punts perds la partida
  if (puntos > 21)
  {
    cout << "\033[1;31mTe has pasado... puntos: " << puntos << "\033[0m" << endl;
    cout << "\033[1;31mHas perdido\033[0m" << endl;
    repetir = false;
  }

  // 21 punts guanyes o empates
  else if (puntos == 21)
  {
    cout << "\033[1;33mFELICIDADES tienes " << puntos << " puntos!(Blackjack)\033[0m" << endl;
    repetir = false;
  }
  // Menys de 21 punts mostres el punts que tens i continues jugant
  else
  {
    cout << "\033[1;32mTienes " << puntos << " puntos \033[0m" << endl;
  }
}

// Amb aquesta funció calcularàs la puntuació de les cartes
int puntuacio(carta mano[], int contadorMano, int contadorAs, int puntos, int num_cartas_mano)
{
  if (mano[contadorMano].numero == 1)
  {
    cout << "\033[1;33m A de " << mano[contadorMano].palo << "\033[0m" << endl;
    contadorAs++;
    if (contadorAs >= 1 && puntos + 11 > 21)
    {
      puntos += 1;
    }
    else
    {
      puntos += 11;
    }
    num_cartas_mano++;
  }
  else if (mano[contadorMano].numero == 11)
  {
    cout << "\033[1;33m Sota de " << mano[contadorMano].palo << "\033[0m" << endl;
    puntos += 10;
    num_cartas_mano++;
  }
  else if (mano[contadorMano].numero == 12)
  {
    cout << "\033[1;33m Caballo de " << mano[contadorMano].palo << "\033[0m" << endl;
    puntos += 10;
    num_cartas_mano++;
  }
  else if (mano[contadorMano].numero == 13)
  {
    cout << "\033[1;33m Rey de " << mano[contadorMano].palo << "\033[0m" << endl;
    puntos += 10;
    num_cartas_mano++;
  }
  else
  {
    cout << "\033[1;33m " << mano[contadorMano].numero << " de " << mano[contadorMano].palo << "\033[0m" << endl;
    puntos += mano[contadorMano].numero;
    num_cartas_mano++;
  }
  return puntos;
}