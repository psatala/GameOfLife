///////////////////////////////////////////////////////////////////////////
///Piotr Satala                                                          //
///                                                                      //
///Projekt 3.4                                                           //
///Gra w zycie - rozwiniecie                                             //
///Program realizuje prosty automat komorkowy - symulator zycia "Life"   //                                 //
///                                                                      //
///Przyjete oznaczenia                                                   //
///h - wysokosc planszy                                                  //
///w - szerokosc planszy                                                 //
///tab - plansza                                                         //
///copy - kopia planszy                                                  //
///c - zmienna do pobierania znaku z klawiatury za pomoca getch()        //
///max_y - wysokosc ekranu                                               //
///max_x - szerokosc ekranu                                              //
///////////////////////////////////////////////////////////////////////////

///Glowny plik zrodlowy obslugujacy menu glowne.

#include "../Headers/sim.h"
#include "../Headers/menu.h"

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

///Funkcja glowna.
int main(int argc, char **argv)
{
	initscr();                               //Inicjalizacja ekranu
	curs_set(0);                             //Wylaczenie kursora
	keypad(stdscr, TRUE);                    //Wlaczenie strzalek kierunkowych

	bool conditionsForBirth [] = {0, 0, 0, 1, 0, 0, 0, 0, 0};    //tablica z liczbami sasiadow pozwalajacymi na powstanie komorki
	bool conditionsForSurvival [] = {0, 0, 1, 1, 0, 0, 0, 0, 0}; //tablica z liczbami sasiadow pozwalajacymi na utrzymanie sie przy zyciu komorki


	FILE *inputFilePointer = NULL;
	FILE *outputFilePointer = NULL;
	enum fileType outputFileType = NONE;

	if(argc >= 2)
	{
		inputFilePointer = fopen(argv[1], "r");
		if(NULL == inputFilePointer)
		{
			printw("Nie ma takiego pliku wejsciowego.\nNacisnij dowolny przycisk, aby kontynuowac");
			getch();
			clear();
		}
	}

	if(argc >= 3)
	{
		outputFilePointer = fopen(argv[2], "w");
		outputFileType = checkFileType(argv[2]);
	}
	if(argc >= 2 && inputFilePointer)
		simulate(conditionsForBirth, conditionsForSurvival, inputFilePointer, outputFilePointer, outputFileType);

	char title[] = "GRA W ZYCIE";
	int max_y;
	int max_x;

	getmaxyx(stdscr, max_y, max_x);          //Pobranie rozmiarow ekranu
	WINDOW *titleWin = newwin(3, strlen(title) + 2, max_y / 2, (max_x - strlen(title)) / 2);             //Utworzenie okna na tytul
	refresh();
	box(titleWin, 0, 0);                     //Obramowanie okna na tytul
	mvwprintw(titleWin, 1, 1, "%s", title);  //Wyswietlenie tytulu
	wrefresh(titleWin);
	char message[] = "Wcisnij dowolny przycisk, aby rozpoczac";
	mvprintw(max_y * 3 / 4, (max_x - strlen(message)) / 2, "%s", message);
	getch();

	char *mainMenu[] = { 
	                		        "Nowa  Gra",
						"Opcje",
						"Wyjdz"
					   };

	int mainMenuChoice;
	while(1)
	{
		//Wybor w menu glownym
		mainMenuChoice = menuSimulation(stdscr, 3, mainMenu);
		switch(mainMenuChoice)
		{
			case 0:
				//Symulacja
				simulate(conditionsForBirth, conditionsForSurvival, NULL, NULL, NONE);
				break;
			case 1:
				//Menu opcji
				clear();
				optionsMenu(stdscr, conditionsForBirth, conditionsForSurvival);
				break;
			case 2:
				//Zakonczenie dzialania programu
				if(NULL != inputFilePointer)
					fclose(inputFilePointer);
				if(NULL != outputFilePointer)
					fclose(outputFilePointer);
				endwin();
				return 0;
		}
	}
}

