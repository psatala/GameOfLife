///Plik naglowkowy z deklaracjami funkcji wykorzystywanymi do przeprowadzenia symulacji

#ifndef SIM_LIB_H_INCLUDED
#define SIM_LIB_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>

///Liczba sasiadow komorki
#define NUMBER_OF_NEIGHBOURS 8

///Maksymalna wysokosc planszy
#define MAX_H 25

///Maksymalna szerokosc planszy
#define MAX_W 80

///Liczba zblizona do kranca zakresu typu calkowitego int
#define INFINITY 2e9

///Kod ascii dla spacji
#define SPACE 32

///Kod ascii dla entera
#define ENTER 10

///Kod ascii dla zera
#define MIN_NUM 48

///Kod ascii dla dziewiatki
#define MAX_NUM 57

///Deklaracja minimum z dwoch liczb
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

///Deklaracja maksimum z dwoch liczb
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

///Typ enumeryczny sluzacy wywolaniu funkcji read_array
enum callOptions {INIT, RUNNING};


///Typ enumeryczny okreslajacy typ pliku
enum fileType {HTML, TXT, NONE};


///Funkcja sprawdzajaca typ pliku.
///Jako parametr przyjmuje nazwe pliku.
///Funkcja zwraca typ enumeryczny okreslajacy typ pliku.
enum fileType checkFileType(char *fileName);


///Funkcja wczytujaca dane.
///Jej parametry to wskazniki do zmiennych odpowiadajacych za: szerokosc planszy, wysokosc planszy, ilosc iteracji symulacji, odstepu pomiedzy iteracjami, wyboru metody wypelniania wartosci komorek tablicy oraz typu planszy.
void initAndRead(int *h, int *w, int *n, double *delay, bool *choice, int *tabType);


///Funkcja wczytujaca dane z pliku.
///Jej parametry to wskazniki odpowiednio do: pliku wejsciowego, pliku wyjsciowego, wysokosci planszy, szerokosci planszy, ilosci iteracji symulacji, odstepu pomiedzy iteracjami, odstepu pomiedzy zapisaniem stanu planszy do pliku wyjsciowego oraz typu planszy.
void readFromFile(FILE *fptr, FILE *fptr2, int *h, int *w, int *n, double *delay, int *N, int *tabType);


///Funkcja generujaca natepna iteracje planszy.
///Jako parametry przyjmuje wysokosc planszy, szerokosc planszy, plansze, jej kopie, tablica z warunkami na narodziny komorki, tablice z warunkami na przetrwanie komorki, tablice z wiekiem komorek oraz typ planszy.
void next_step(int h, int w, bool **tab, bool **copy, bool *birth, bool *survival, int **age, int tabType);


///Funkcja pozwalajaca uzytkownikowi na wlasnoreczne wypelnienie planszy.
///Jako parametry przyjmuje wysokosc planszy, szerokosc planszy, plansze, jej kopie, numer oteracji, sposob wywolania funkcji oraz wspolrzedne lewego gornego rogu ekranu.
void read_array(int h, int w, bool **tab, bool **copy, int number, enum callOptions chosenOption, int start_x, int start_y);


///Funkcja generujaca pseudolosowe ustawienie planszy.
///Jej parametry to wysokosc planszy, szerokosc planszy, plansza, jej kopia.
void generate_array(int h, int w, bool **tab, bool **copy);


///Funkcja wczytujaca plansze z pliku wejsciowego.
///Jej parametry to wysokosc planszy, szerokosc planszy, plansza, jej kopia oraz wskaznik do pliku.
void readArrayFromFile(int h, int w, bool **tab, bool **copy, FILE *fptr);


///Funkcja odpowiedzialna za prawidlowe wczytanie zmiennej typu calkowitego.
///Jej parametry to wskaznik na zmienna, ktora ma zostac wczytana, minimalna wartosc zmiennej, maksymalna wartosc zmiennej oraz wskaznik do pliku, z ktorego zmienna bedzie pobierana.
///Funkcja zwraca TRUE, gdy udalo jej sie wczytac liczbe i FALSE w przeciwnym wypadku.
bool readIntProperly(int *variableToScan, int minValue, int maxValue, FILE *fptr);


///Funkcja odpowiedzialna za prawidlowe wczytanie zmiennej typu zmiennoprzecinkowego.
///Jej parametry to wskaznik na zmienna, ktora ma zostac wczytana, minimalna wartosc zmiennej, maksymalna wartosc zmiennej oraz wskaznik do pliku, z ktorego zmienna bedzie pobierana.
///Funkcja zwraca TRUE, gdy udalo jej sie wczytac liczbe i FALSE w przeciwnym wypadku.
bool readDoubleProperly(double *variableToScan, double minValue, double maxValue, FILE *fptr);


///Funkcja wyswietlajaca plansze na ekran.
///Jako parametry przyjmuje wysokosc plaszy, szerokosc planszy, plansze, numer iteracji oraz wspolrzedne lewego gornego rogu ekranu.
void print(int h, int w, bool **tab, int number, int start_x, int start_y);


///Funkcja zapisujaca plansze do pliku txt.
///Jako parametry przyjmuje wysokosc planszy, szerokosc planszy, plansze, numer iteracji oraz wskaznik do pliku, do ktorego plansza ma zostac zapisana.
void printToTXT(int h, int w, bool **tab, int number, FILE *fptr2);


///Funkcja zapisujaca plansze do pliku html.
///Jako parametry przyjmuje wysokosc planszy, szerokosc planszy, plansze, wiek komorek planszy, numer iteracji oraz wskaznik do pliku, do ktorego plansza ma zostac zapisana.
void printToHTML(int h, int w, bool **tab, int **age, int number, FILE *fptr2);


///Funkcja odpowiedzialna za wyswietlenie planszy w trakcie modyfikacji wartosci przez uzytkownika.
void printPlatform(int h, int w, bool **tab, int number, int y, int x, int start_x, int start_y);

#endif // SIM_LIB_H_INCLUDED

