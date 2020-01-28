///Plik zrodlowy zawierajacy definicje funkcji obslugujacych menu typu pionowego.

#include "../Headers/menu.h"


///Funkcja odpowiada za interakcje uzytkownika z menu.
///Jako parametry prayjmuje okno, w ktorym menu bedzie wyswietlane, ilosc pozycji w menu, a takze tablice zmiennych typu string - poszczegolne pozycje w menu
///Funkcja zwraca numer pozycji wybrane przez uzytkownika
int menuSimulation(WINDOW * menu, int numberOfOptions, char *options[])
{
	noecho();              //Wylaczenie wyswietlanie wpisywanych wartosci
	cbreak();
	keypad(menu, TRUE);    //Wlaczenie strzalek kierunkowych
	int chosenOption = 0;  //Opcja, na ktora najechal kursor
	int finalChoice = -1;  //Ostateczny wybor
	int c;

	printMenu(menu, numberOfOptions, options, chosenOption); //Wyswietlenie menu
	while(1)
	{
		c = wgetch(menu);  //Wczytanie znaku z klawiatury
		switch(c)
		{
			case KEY_UP:  //Przesuniecie kursora do gory
				if(0 == chosenOption)
					chosenOption = numberOfOptions - 1;
				else
					chosenOption--;
				break;
			case KEY_DOWN: //Przesuniecie kursora do dolu
				if(numberOfOptions - 1 == chosenOption)
					chosenOption = 0;
				else
					chosenOption++;
				break;
			case ENTER:    //Zatwierdzenie
				finalChoice = chosenOption;
				break;
		}
		printMenu(menu, numberOfOptions, options, chosenOption); //Wywietlenie menu
		if(finalChoice != -1) //Wyjscie z nieskonczonej petli
		{
			echo();
			return finalChoice;
		}
	}
}


///Funkcja odpowiada za wyswietlania menu.
///Parametry to okno, w ktorym menu bedzie wyswietlane, ilosc pozycji w menu, tablica z poszczegolnymi pozycjami w menu, a takze wybrana aktualnie przez uzytkownika opcja
void printMenu(WINDOW * menu, int numberOfOptions, char *options[], int chosenOption)
{
	clear();
	int max_y;
	int max_x;
	getmaxyx(menu, max_y, max_x); //Pobranie rozmiarow ekranu, w celu wyswietlenia menu na jego srodku

	for(int i = 0; i < numberOfOptions; i++)
	{

		if(i == chosenOption) //Dla wybranej opcji
		{
			wattron(menu, A_REVERSE); //Wlaczenie atrybutu podswietlania
			mvwprintw(menu, (max_y - numberOfOptions) / 2 + i, (max_x - strlen(options[i])) / 2, "%s\n", options[i]);
			wattroff(menu, A_REVERSE); //Wylaczenie atrybutu podswietlania
		}
		else  //Dla pozostalych opcji
			mvwprintw(menu, (max_y - numberOfOptions) / 2 + i, (max_x - strlen(options[i])) / 2, "%s\n", options[i]);

	}
	wrefresh(menu);
}


///Funkcja odpowiadajaca za menu opcji.
///Parametry to okno, w ktorym menu bedzie wyswietlane, tablica z danymi o warunkach narodzin komorki i tablica z danymi o warunkach przetrwania komorki
void optionsMenu(WINDOW * menu, bool *birth, bool *survival)
{
	clear();
	while(1)
	{
		char *possibleChoices[] = {
									"Zmien warunki narodzin",
									"Zmien warunki przetrwania",
									"Powrot do menu glownego"
								  };
		int choice = menuSimulation(menu, 3, possibleChoices); //Wybranie opcji przez uzytkownika
		switch(choice)
		{
			case 0: //Zmiana warunkow narodzin
				changeConditions(menu, birth, 0);
				break;
			case 1: //Zmiana warunkow przetrwania
				changeConditions(menu, survival, 1);
				break;
			case 2: //Powrot do menu glownego
				return;
		}
	}
}


///Funkcja odpowiada za zmiane opcji.
///Parametry to okno, w ktorym menu bedzie wyswietlane, tablica z danymi do zmiany oraz zmienna typu bool, ktora przyjmuje wartosc FALSE, jesli zmieniamy warunki narodzin i TRUE, gdy zmieniamy wartosci przetrwania
void changeConditions(WINDOW * menu, bool *array, bool birthOrSurvival)
{
	int c;
	clear();
	//Wyswietlenie komunikatu dla uzytkownika
	wprintw(menu, "Jesli chcesz, aby dla k sasiadow komorka ");
	if(!birthOrSurvival)
		wprintw(menu, "rodzila sie, ");
	else
		wprintw(menu, "przezywala, ");
	wprintw(menu, "wcisnij k.\nAnalogicznie w przeciwnym wypadku.\nEnter, aby wyjsc z opcji.\nNacisnij dowolny przycisk, aby kontynuowac.");
	getch();

	while(1)
	{
		clear();
		//Wyswietlenie aktualnych warunkow
		wprintw(menu, "Aktualne warunki ");
		if(!birthOrSurvival)
			wprintw(menu, "narodzin to: ");
		else
			wprintw(menu, "przetrwania to: ");
		for(int i = 0; i <= NUMBER_OF_NEIGHBOURS; i++)
			if(array[i])
				wprintw(menu, "%d ", i);
		
		
		c = getch();
		if(c >= 48 && c <= 56) //Zmiana opcji przez uzytkownika
		{
			c -= 48;
			array[c] = !array[c];
		}
		else if(10 == c)  //Powrot do menu opcji
			break;
	}
} 
