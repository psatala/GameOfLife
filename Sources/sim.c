///Plik zrodlowy z definicja funkcji odpowiedzialnej za symulacje.

#include "../Headers/sim.h"
#include "../Headers/sim_lib.h"

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


///Funkcja odpowiedzialna za symulacje.
///Jako parametry przyjmuje tablice z danymi o warunkach narodzin komorki, tablice z danymi o warunkach przetrwania komorki oraz typ pliku.
void simulate(bool *birth, bool *survival, FILE *fptr, FILE *fptr2, enum fileType outType)
{
	srand(time(NULL));  //Wybranie ziarna do generowania liczb pseudolosowych
	clear();
	int height;
	int width;
	int numberOfIterations;
	int delayBetweenScreenshots = INFINITY; //Odstep pomiedzy zapisaniem stanu planszy do pliku wyjsciowego
	int typeOfPlatform = false;    //true - torus, false - zamknieta
	double delayBetweenIterations;
	bool choiceOfInputMethod;
	bool **platform;
	bool **copyOfPlatform;
	int **ageOfCells;

	//Wczytanie danych
	if(!fptr)
		initAndRead(&height, &width, &numberOfIterations, &delayBetweenIterations, &choiceOfInputMethod, &typeOfPlatform);
	else
		readFromFile(fptr, fptr2, &height, &width, &numberOfIterations, &delayBetweenIterations, &delayBetweenScreenshots, &typeOfPlatform);
	//Alokacja pamieci
	platform = calloc(height, sizeof(bool*));
	copyOfPlatform = calloc(height, sizeof(bool*));
	ageOfCells = calloc(height, sizeof(int*));
	for(int i = 0; i < height; i++)
	{
	    	platform[i] = calloc(width, sizeof(bool));
		copyOfPlatform[i] = calloc(width, sizeof(bool));
		ageOfCells[i] = calloc(width, sizeof(int));
	}
	
	//Ustalenie wartosci komorek tablicy
	if(fptr)
		readArrayFromFile(height, width, platform, copyOfPlatform, fptr); //Wypelnienie komorek wartosciami z pliku wejsciowego
	else if(!choiceOfInputMethod)
	        read_array(height, width, platform, copyOfPlatform, 0, INIT, 0, 0); //Wypelnienie komorek przez uzytkownika
	else
	        generate_array(height, width, platform, copyOfPlatform); //Pseudolosowe wypelnienie komorek

	clear();
	printw("Aby chwilowo zatrzymac symulacje, wcisnij spacje.\nUzyj strzalek, aby poruszac sie po planszy.\nWcisnij enter, zeby zmienic stan komorki.\nAby wznowic, ponownie nacisnij spacje.\nNacisnij dowolny przycisk, aby rozpoczac symulacje.");
	getch();

	int start_x = 0;     //Zmienna okreslajaca wspolrzedna odcieta lewego gornego rogu ekranu
	int start_y = 0;     //Zmienna okreslajaca wsoplrzedna rzedna lewego gornego rogu ekranu
	int max_x;
	int max_y;

	print(height, width, platform, 0, start_x, start_y);  //Wyswietlenie planszy na ekran
	
	nodelay(stdscr, TRUE);
	noecho();
	int c;
	for(int i = 0; i < numberOfIterations; i++)
	{
		usleep(delayBetweenIterations * 1e6);  //Uspienie programu na czas odstepu pomiedzy iteracjami
		c = getch();
		getmaxyx(stdscr, max_y, max_x);
		max_y -= 2;
		--max_x;
		switch(c)
		{
			case SPACE:     //Obsluga pauzy
				nodelay(stdscr, FALSE);
				read_array(height, width, platform, copyOfPlatform, i + 1, RUNNING, start_x, start_y);
				nodelay(stdscr, TRUE);
				break;
			case KEY_UP:    //Przesuniecie ramki w gore
				start_y -= max_y;
				if(start_y < 0)
					start_y = (height / max_y) * max_y;
				if(start_y == height)
					start_y -= max_y;
				break;
			case KEY_DOWN:  //Przesuniecie ramki w dol
				start_y += max_y;
				if(start_y >= height)
					start_y = 0;
				break;
			case KEY_LEFT:  //Przesuniecie ramki w lewo
				start_x -= max_x;
				if(start_x < 0)
					start_x = (width / max_x) * max_x;
				if(start_x == width)
					start_x -= max_x;
				break;
			case KEY_RIGHT: //Przesuniecie ramki w prawo
				start_x += max_x;
				if(start_x >= width)
					start_x = 0;
				break;
			case ESCAPE:    //Obsluga szybszego wyjscia z programu
				delayBetweenIterations = 0;
				break;
				
		} 

		next_step(height, width, platform, copyOfPlatform, birth, survival, ageOfCells, typeOfPlatform); //Wygenerowanie nastepnej iteracji
		
		
		if(delayBetweenIterations)
			print(height, width, platform, i + 1, start_x, start_y);          //Wyswietlenie planszy na ekran
		if(delayBetweenScreenshots && !((i + 1) % delayBetweenScreenshots) && fptr2)
		{
			if(outType == TXT)
				printToTXT(height, width, platform, i + 1, fptr2);     //Zapisanie planszy do pliku txt
			else
				printToHTML(height, width, platform, ageOfCells, i + 1, fptr2);     //Zapisanie planszy do pliku html
		}
	}
	nodelay(stdscr, FALSE);
	
	//Zwolnienie pamieci	
	for(int i = 0; i < height; i++)
	{
		free(platform[i]);
		free(copyOfPlatform[i]);
		free(ageOfCells[i]);
	}
	free(platform);
	free(copyOfPlatform);
	free(ageOfCells);
	printw("Symulacja zakonczyla sie. Nacisnij dowolny przycisk, aby wyjsc do menu glownego.");
	getch();
	clear();
}
