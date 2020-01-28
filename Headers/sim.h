///Plik naglowkowy z deklaracja funkcji odpowiedzialnej za symulacje.

#ifndef SIM_H_INCLUDED
#define SIM_H_INCLUDED

#include <stdbool.h>
#include <ncurses.h>
#include "../Headers/sim_lib.h"

///Kod ascii dla spacji
#define SPACE 32

///Kod ascii dla escape'a
#define ESCAPE 27

///Funkcja odpowiedzialna za symulacje.
///Jako parametry przyjmuje tablice z danymi o warunkach narodzin komorki, tablice z danymi o warunkach przetrwania komorki oraz typ pliku.
void simulate(bool *birth, bool *survival, FILE *fptr, FILE *fptr2, enum fileType outType);

#endif //SIM_H_INCLUDED
