///Plik zrodlowy z definicjami funkcji wykorzystywanymi do przeprowadzenia symulacji.

#include "../Headers/sim_lib.h"
#include "../Headers/menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>


///Funkcja sprawdzajaca typ pliku.
///Jako parametr przyjmuje nazwe pliku.
///Funkcja zwraca typ enumeryczny okreslajacy typ pliku.
enum fileType checkFileType(char *fileName)
{
	int lentgh = strlen(fileName);
	if(lentgh > 5 && fileName[lentgh - 1] == 'l' && fileName[lentgh - 2] == 'm' && fileName[lentgh - 3] == 't' && fileName[lentgh - 4] == 'h' && fileName[lentgh - 5] == '.')
		return HTML;
	return TXT;
}


///Funkcja wczytujaca dane z pliku.
///Jej parametry to wskazniki odpowiednio do: pliku wejsciowego, pliku wyjsciowego, wysokosci planszy, szerokosci planszy, ilosci iteracji symulacji, odstepu pomiedzy iteracjami, typu planszy oraz odstepu pomiedzy zapisaniem stanu planszy do pliku wyjsciowego.
void readFromFile(FILE *fptr, FILE *fptr2, int *h, int *w, int *n, double *delay, int *N, int *tabType)
{
	if(!readIntProperly(h, 0, INFINITY, fptr))
	{
		printw("Podaj wysokosc planszy: ");
		readIntProperly(h, 0, INFINITY, NULL);
	}
	if(!readIntProperly(w, 0, INFINITY, fptr))
	{
		printw("Podaj szerokosc planszy: ");
		readIntProperly(w, 0, INFINITY, NULL);
	}
	if(!readIntProperly(n, 0, INFINITY, fptr))
	{
		printw("Podaj ilosc krokow symulacji: ");
		readIntProperly(n, 0, INFINITY, NULL);
	}
	if(!readDoubleProperly(delay, 0, INFINITY, fptr))
	{
		printw("Podaj czas zwloki pomiedzy kolejnymi iteracjami (w sekundach): ");
		readDoubleProperly(delay, 0, INFINITY, NULL);
	}
	if(!readIntProperly(tabType, 0, 1, fptr))
	{
		printw("Podaj typ planszy: ");
		readIntProperly(tabType, 0, 1, NULL);
	}
	if(fptr2 && !readIntProperly(N, 0, INFINITY, fptr))
	{
		printw("Podaj odstep pomiedzy zapisaniem stanu planszy do pliku wyjsciowego: ");
		readIntProperly(N, 0, INFINITY, NULL);
	}
}

///Funkcja wczytujaca dane ze standardowego wejscia.
///Jej parametry to wskazniki do zmiennych odpowiadajacych za: szerokosc planszy, wysokosc planszy, ilosc iteracji symulacji, odstepu pomiedzy iteracjami, wyboru metody wypelniania wartosci komorek tablicy oraz typu planszy.
void initAndRead(int *h, int *w, int *n, double *delay, bool *choice, int *tabType)
{	
	printw("Wprowadzanie danych.\n");

	printw("Podaj wysokosc planszy:  ");
    readIntProperly(h, 0, INFINITY, NULL);

	printw("Podaj szerokosc planszy: ");
	readIntProperly(w, 0, INFINITY, NULL);

	printw("Podaj ilosc krokow symulacji: ");
	readIntProperly(n, 0, INFINITY, NULL);
	
	printw("Podaj czas zwloki pomiedzy kolejnymi iteracjami (w sekundach): ");
	readDoubleProperly(delay, 0, INFINITY, NULL);

	printw("Podaj typ planszy: ");
	readIntProperly(tabType, 0, 1, NULL);
	
	char *optionsForInput [] = {
	                                                         "Wprowadz dane recznie",
															 "Wygeneruj losowa plansze"
							   };
	int temp = menuSimulation(stdscr, 2, optionsForInput); //Wybor metody wypelnienia tablicy
	*choice = (bool)temp;
	clear();
	refresh();
}


///Funkcja odpowiedzialna za prawidlowe wczytanie zmiennej typu calkowitego.
///Jej parametry to wskaznik na zmienna, ktora ma zostac wczytana, minimalna wartosc zmiennej, maksymalna wartosc zmiennej oraz wskaznik do pliku, z ktorego zmienna bedzie pobierana.
///Funkcja zwraca TRUE, gdy udalo jej sie wczytac liczbe i FALSE w przeciwnym wypadku.
bool readIntProperly(int *variableToScan, int minValue, int maxValue, FILE *fptr)
{
	int temp;
	char c;
	bool guard = false;    //Poprawnosc wpisywanej danej
	int digits;            //Liczba cyfr
	
	bool exit = false;     //Zmienna obslugujaca ewentualne wczesniejsze wyjscie z petli
	if(fptr)
		exit = true;
	do
	{
		temp = 0;
		guard = true;
		c = 0;
		digits = 0;
		while(c != ENTER)
		{
			if(!fptr)                         //Wczytanie znaku:
				c = getch();                  //ze standardowego wejscia
			else
				c = fgetc(fptr);              //z pliku

			if(-1 == c)                       //Wyjscie z funkcji w przypadku dotarcia do konca pliku
				return false;
			if(ENTER == c)                    //Zakonczenie wczytywania
				break;
			if(c < MIN_NUM || c > MAX_NUM)    //Wczytanie niewlasciwego znaku
				guard = false;
			else                              //Konstrukcja liczby
			{
				c -= MIN_NUM;
				digits++;
				temp = temp * 10 + c;
			}
		}
		if(temp < minValue || temp > maxValue || !digits)   //Sprawdzenie warunkow koncowych
			guard = false;
		if(!guard)
		{
			clear();
			printw("Sprobuj jeszcze raz: ");
		}
	}while(!guard && !exit);
	clear();
	*variableToScan = temp;
	return guard;
}


///Funkcja odpowiedzialna za prawidlowe wczytanie zmiennej typu zmiennoprzecinkowego.
///Jej parametry to wskaznik na zmienna, ktora ma zostac wczytana, minimalna wartosc zmiennej, maksymalna wartosc zmiennej oraz wskaznik do pliku, z ktorego zmienna bedzie pobierana.
///Funkcja zwraca TRUE, gdy udalo jej sie wczytac liczbe i FALSE w przeciwnym wypadku.
bool readDoubleProperly(double *variableToScan, double minValue, double maxValue, FILE *fptr)
{
	double temp;
	double res;           //Zmienna chwilowo przetrzymujaca wynik
	char c;
	bool guard = false;   //Poprawnosc wpisywanej danej
	bool dot;             //Wystapienie kropki w zapisie
	int digits;           //Liczba cyfr
	int divider;          //Podzielnik do czesci ulamkowej

	bool exit = false;     //Zmienna obslugujaca ewentualne wczesniejsze wyjscie z petli
	if(fptr)
		exit = true;
	do
	{
		temp = 0;
		res = 0;
		guard = true;
		dot = false;
		c = 0;
		digits = 0;
		divider = 1;
		while(c != ENTER)
		{
			if(!fptr)
				c = getch();
			else
				c = fgetc(fptr);
			if(-1 == c)                      //Wyjscie z funkcji w przypadku dotarcia do konca pliku
				return false;
			if(ENTER == c)                    //Zakonczenie wczytywania
				break;
			else if('.' == c)                 //Wczytanie kropki
			{
				if(!dot)
				{
					dot = true;
					res = temp;
					temp = 0;
					digits = 0;
				}
				else
					guard = false;
			}
			else if(c < MIN_NUM || c > MAX_NUM)   //Wczytanie niewlasciwego znaku
				guard = false;
			else if(digits < 6 || !dot)           //Konstrukcja liczby
			{
				c -= MIN_NUM;
				temp = temp * 10 + c;
				digits++;
			}
		}
		for(int i = 0; i < digits; i++)           //Przeksztalcenie liczby do jej docelowej postaci
			divider *= 10;
		if(dot)
			res += (temp / divider);
		else
			res = temp;
		if(res < minValue || res > maxValue || !digits)   //Sprawdzenie warunkow koncowych
			guard = false;
		if(!guard)
		{
			clear();
			printw("Sprobuj jeszcze raz: ");
		}
	}while(!guard && !exit);
	*variableToScan = res;
	clear();
	return guard;
}


///Funkcja pozwalajaca uzytkownikowi na wlasnoreczne wypelnienie planszy.
///Jako parametry przyjmuje wysokosc planszy, szerokosc planszy, plansze, jej kopie, numer iteracji, sposob wywolania funkcji oraz wspolrzedne lewego gornego rogu.
void read_array(int h, int w, bool **tab, bool **copy, int number, enum callOptions chosenOption, int start_x, int start_y)
{
	int x = 0;
	int y = 0;
	int max_y;
	int max_x;
	getmaxyx(stdscr, max_y, max_x);
	max_y = MIN(h - start_y, max_y - 2);
	max_x = MIN(w - start_x, max_x - 1);
	int c;
	noecho();
	if(!chosenOption)
	{
		printw("Uzyj strzalek, aby poruszac sie po planszy.\nWcisnij enter, zeby zmienic stan komorki.\nSpacja, aby zakonczyc wprowadzanie danych i rozpoczac symulacje.\nNacisnij dowolny przycisk,aby kontynuowac.");
		getch();
	}
	printPlatform(h, w, tab, number, y + start_y, x + start_x, start_x, start_y); //Wyswietlenie aktualnego stanu planszy
	while(1)
	{
		c = getch();            //Wczytanie znaku
		switch(c)
		{
			case KEY_UP:    //Przesuniecie kursora w gore
				if(y == 0)
					y = max_y - 1;
				else
					y--;
				break;
			case KEY_DOWN:  //Przesuniecie kursora w dol
				if(y >= max_y - 1)
					y = 0;
				else
					y++;
				break;
			case KEY_RIGHT: //Przesuniecie kursora w prawo
				if(x >= max_x - 1)
					x = 0;
				else
					x++;
				break;
			case KEY_LEFT:  //Przesuniecie kursora w lewo
				if(x == 0)
					x = max_x - 1;
				else
					x--;
				break;
			case ENTER:     //Zatwierdzenie zmiany stanu danej komorki
				tab[y + start_y][x + start_x] = !tab[y + start_y][x + start_x];
				break;
			case SPACE:     //Zakonczenie wprowadzania modyfikacji
				echo();
				for(int i = 0; i < h; i++)
					for(int j = 0; j < w; j++)
						copy[i][j] = tab[i][j];
				return;
		}
		printPlatform(h, w, tab, number, y + start_y, x + start_x, start_x, start_y);  //Wyswietlenie aktualnego stanu planszy
	}
}


///Funkcja generujaca pseudolosowe ustawienie planszy.
///Jej parametry to wysokosc planszy, szerokosc planszy, plansza, jej kopia.
void generate_array(int h, int w, bool **tab, bool **copy)
{
    int temp; //Zmienna pomocnicza
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            temp = rand() % 2;  //Losowanie wartosci komorki
            tab[i][j] = temp;
            copy[i][j] = temp;
        }
    }
}

///Funkcja wczytujaca plansze z pliku wejsciowego.
///Jej parametry to wysokosc planszy, szerokosc planszy, plansza, jej kopia oraz wskaznik do pliku.
void readArrayFromFile(int h, int w, bool **tab, bool **copy, FILE *fptr)
{
	int index_h = 0;
	int index_w = 0;
	int numberOfAliveCells = 0;
	char c;
	int min_y_index = h - 1;
	int min_x_index = w - 1;
	int max_y_index = 0;
	int max_x_index = 0;
	while((c = fgetc(fptr)) != EOF)
	{
		if(index_w == w && c != ENTER)               //Automatyczne przejscie do nowej linii
		{
			index_h++;
			index_w = 0;
		}
		if(index_h == h)                             //Zakonczenie wczytywania skutkujace zbyt duza iloscia danych
		{
			printw("Podane dane nie mieszcza sie w zadanej planszy.\nCzesc, ktora sie nie zmiescila, nie zostala wczytana przez program.\nWcisnij dowolny przycisk, aby kontynuowac.");
			getch();
			break;
		}
		if(SPACE == c)                               //Ujednolicenie zpisu dla martwwej komorki
			c = MIN_NUM;
		switch(c)
		{
			case ENTER:                          //Nowa linia
				index_h++;
				index_w = 0;
				break;
			case MIN_NUM:                        //Martwa komorka
				copy[index_h][index_w] = false;
				index_w++;
				break;
			default:                            //Zywa komorka
				copy[index_h][index_w] = true;
				min_y_index = MIN(min_y_index, index_h);
				min_x_index = MIN(min_x_index, index_w);
				max_y_index = MAX(max_y_index, index_h);
				max_x_index = MAX(max_x_index, index_w);
				index_w++;
				++numberOfAliveCells;
				break;
		}
	}
	for(int i = 0; i <= max_y_index - min_y_index; i++)
		for(int j = 0; j <= max_x_index - min_x_index; j++)
			tab[i + (h - max_y_index + min_y_index) / 2][j + (w - max_x_index + min_x_index) / 2] = copy[i + min_y_index][j + min_x_index];
	for(int i = 0; i < h; i++)
		for(int j = 0; j < w; j++)
			copy[i][j] = tab[i][j];
	if(!numberOfAliveCells)
		generate_array(h, w, tab, copy);
}



///Funkcja generujaca natepna iteracje planszy.
///Jako parametry przyjmuje wysokosc planszy, szerokosc planszy, plansze, jej kopie, tablica z warunkami na narodziny komorki, tablice z warunkami na przetrwanie komorki, tablice z wiekiem komorek oraz typ planszy.
void next_step(int h, int w, bool **tab, bool **copy, bool *birth, bool *survival, int **age, int tabType)
{
	//Roza wiatrow to tablica pomagajaca poruszac sie po dwuwymiarowej planszy
	int x [] = {1, 1, 1, 0, -1, -1, -1, 0}; //Roza wiatrow dla osi x
	int y [] = {-1, 0, 1, 1, 1, 0, -1, -1}; //Roza wiatrow dla osi y
	int sum = 0; //Suma zywych sasiadow
	int temp_x;
	int temp_y;
	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{
			sum = 0;
			for(int k = 0; k < NUMBER_OF_NEIGHBOURS; k++)
			{
				temp_y = i + y[k];   //Skorzystanie z rozy wiatrow dla y
				temp_x = j + x[k];   //Skorzystanie z rozy wiatrow dla x

				if(((temp_y + h) % h == temp_y && (temp_x + w) % w == temp_x) || tabType)   //Sprawdzenie, czy mozna wziac dana komorke pod uwage przy liczeniu sasiadow
				{
					temp_y = (temp_y + h) % h; //Ewentualna korekta wspolrzednych (jesli te wykraczaja poza plansze)
					temp_x = (temp_x + w) % w; //Ewentualna korekta wspolrzednych (jesli te wykraczaja poza plansze)
					if(tab[temp_y][temp_x])
						sum++;
				}
			}
			if(tab[i][j]) //Sprawdzenie, czy w nastepnej iteracji zywa komorka nadal bedzie zyc
			{
				if(survival[sum])
					copy[i][j] = 1;
				else
					copy[i][j] = 0;
			}
			else  //Sprawdzenie, czy w nastepnej iteracji niezywa komorka sie narodzi
			{
				if(birth[sum])
					copy[i][j] = 1;
				else
					copy[i][j] = 0;
			}

		}
	}
	//Przepisanie zawartosci kopii planszy na oryginal i zaktualizowanie wieku komorek
	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{
			tab[i][j] = copy[i][j];
			if(tab[i][j])
				age[i][j]++;
			else
				age[i][j] = 0;
		}
	}
}


///Funkcja wyswietlajaca plansze na ekran.
///Jako parametry przyjmuje wysokosc plaszy, szerokosc planszy, plansze, numer iteracji oraz wspolrzedne lewego gornego rogu ekranu.
void print(int h, int w, bool **tab, int number, int start_x, int start_y)
{
	clear();
	int max_y;
	int max_x;
	getmaxyx(stdscr, max_y, max_x); //Pobranie rozmiarow ekranu
	max_y -= 2;
	--max_x;
	for(int i = 0; i < max_y; i++)
	{
		if(i + start_y >= h)
			break;
		for(int j = 0; j < max_x; j++)
		{
			if(j + start_x >= w)
				break;			
			else if(tab[i + start_y][j + start_x])
					printw("#");  //Wyswietlenie zywej komorki
			else
					printw(" ");  //Wyswietlenie martwej komorki
		}
		printw("\n");
	}
	printw("Numer iteracji: %d, wspolrzedne lewego gornego rogu: (%d, %d)\n", number, start_x, start_y); //Wyswietlenie numeru iteracji i wspolrzednych lewego gornego rogu
	refresh();
}


///Funkcja zapisujaca plansze do pliku txt.
///Jako parametry przyjmuje wysokosc planszy, szerokosc planszy, plansze, numer iteracji oraz wskaznik do pliku, do ktorego plansza ma zostac zapisana.
void printToTXT(int h, int w, bool **tab, int number, FILE *fptr2)
{
	for(int i = 0; i < h; i++)
	{
		for(int j = 0; j < w; j++)
		{
			if(tab[i][j])
				fprintf(fptr2, "#");    //Zapisanie zywej komorki do pliku
			else
				fprintf(fptr2, " ");    //Zapisanie martwej komorki do pliku
		}
		fprintf(fptr2, "\n");
	}
	fprintf(fptr2, "Numer iteracji: %d\n", number); //Zapisanie numeru iteracji
	for(int i = 0; i < w; i++)
		fprintf(fptr2, "*");                    //Zapisanie linii odstepu
	fprintf(fptr2, "\n");
}


///Funkcja zapisujaca plansze do pliku html.
///Jako parametry przyjmuje wysokosc planszy, szerokosc planszy, plansze, wiek komorek planszy, numer iteracji oraz wskaznik do pliku, do ktorego plansza ma zostac zapisana.
void printToHTML(int h, int w, bool **tab, int **age, int number, FILE *fptr2)
{
	char *colors [] = { "#ffffff", "#ffff00", "#ffa500", "#ff0000", "#000000" };     //Mozliwe kolory
	int colorCondition[] = { 0, 1, 5, 25, 125 };                                     //Progi wiekowe dla kolejnych kolorow
	const int numberOfColors = sizeof(colorCondition) / sizeof(int);
	fprintf(fptr2, "<html>\n<body>\n\n<style>\ntable, th, td {\n	border: 1px solid black;\n	border-collapse:collapse;\n}\n</style>\n\n<p>\n<table>");
	int temp;
	for(int i = 0; i < h; i++)
	{
		fprintf(fptr2, "<tr>\n");
		for(int j = 0; j < w; j++)
		{
			temp = 0;
			while(temp + 1 < numberOfColors && colorCondition[temp + 1] <= age[i][j])     //Sprawdzenie koloru komorki
				temp++;
			fprintf(fptr2, "<td bgcolor = \"%s\">&nbsp</td>\n", colors[temp]);                //Zapisanie komorki do pliku
		}
		fprintf(fptr2, "</tr>\n");
	}
	fprintf(fptr2, "</table>\n\n</body>\n</html>\n");
	fprintf(fptr2, "Numer iteracji: %d</p>", number); //Zapisanie numeru iteracji
	refresh();
}


///Funkcja odpowiedzialna za wyswietlenie planszy w trakcie modyfikacji wartosci przez uzytkownika.
///Jej parametry to wysokosc planszy, szerokosc planszy, plansza, numer iteracji, wspolrzedna rzedna kursora, wspolrzedna odcieta kursora oraz wspolrzedne lewwgo gornego rogu ekranu.
void printPlatform(int h, int w, bool **tab, int number, int y, int x, int start_x, int start_y)
{
	clear();
	int max_y;
	int max_x;
	getmaxyx(stdscr, max_y, max_x); //Pobranie szerokosci ekranu
	max_y -= 2;
	--max_x;	
	for(int i = 0; i < max_y; i++)
	{
		if(i + start_y >= h)
			break;
		for(int j = 0; j < max_x; j++)
		{
			if(j + start_x >= w)
				break;			
			else if(i + start_y == y && j + start_x == x) //Wyswietlenie pola, na ktorym znajduje sie kursor
			{
				attron(A_REVERSE);  //Wlaczenie podswietlenia
				if(tab[i + start_y][j + start_x])
					printw("#");
				else
					printw(" ");
				attroff(A_REVERSE); //Wylaczenie podswietlenia
			}
			else //Wyswietlenie pozostalych pol
			{
				if(tab[i + start_y][j + start_x])
					printw("#");
				else
					printw(" ");
			}
		}
		printw("\n");
	}
	printw("Numer iteracji: %d, wspolrzedne lewego gornego rogu: (%d, %d)\n", number, start_x, start_y); //Wyswietlenie numeru iteracji i wspolrzednych lewego gornego rogu
}

