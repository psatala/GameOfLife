///Plik naglowkowy zawierajacy deklaracje funkcji obslugujacych menu typu pionowego.

#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <string.h>
#include <ncurses.h>

///Liczba sasiadow komorki
#define NUMBER_OF_NEIGHBOURS 8

///Kod ascii dla entera
#define ENTER 10


///Funkcja odpowiada za interakcje uzytkownika z menu.
///Jako parametry prayjmuje okno, w ktorym menu bedzie wyswietlane, ilosc pozycji w menu, a takze tablice zmiennych typu string - poszczegolne pozycje w menu.
///Funkcja zwraca numer pozycji wybrane przez uzytkownika.
int menuSimulation(WINDOW * menu, int numberOfOptions, char *options[]);


///Funkcja odpowiada za wyswietlania menu.
///Parametry to okno, w ktorym menu bedzie wyswietlane, ilosc pozycji w menu, tablica z poszczegolnymi pozycjami w menu, a takze wybrana aktualnie przez uzytkownika opcja.
void printMenu(WINDOW * menu, int numberOfOptions, char *options[], int chosenOption);


///Funkcja odpowiadajaca za menu opcji.
///Parametry to okno, w ktorym menu bedzie wyswietlane, tablica z danymi o warunkach narodzin komorki i tablica z danymi o warunkach przetrwania komorki.
void optionsMenu(WINDOW * menu, bool *birth, bool *survival);


///Funkcja odpowiada za zmiane opcji.
///Parametry to okno, w ktorym menu bedzie wyswietlane, tablica z danymi do zmiany oraz zmienna typu bool, ktora przyjmuje wartosc FALSE, jesli zmieniamy warunki narodzin i TRUE, gdy zmieniamy wartosci przetrwania.
void changeConditions(WINDOW * menu, bool *array, bool birthOrSurvival);

#endif // LIB_H_INCLUDED
