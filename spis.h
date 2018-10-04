#define LICZBA_DL 6	/*maksmalna dlugosc liczby*/
#define MAX_NAZWA 30	/*maksymalna dlugosc nazwy ciastka*/

typedef struct ciasteczko{
	struct ciasteczko *poprz;
	int liczba;
}ciasteczko;

typedef struct stosik{
	char nazwa[MAX_NAZWA];
	ciasteczko *na_wierzchu; /*szczyt stosu ciastek*/
	struct stosik *poprz;
	struct stosik *nast;
}stosik;

char *wczytaj_wyrazy(char *wyraz, int ile);
int wczytaj_liczbe();
stosik *sprawdz_czy_istnieje(stosik *pozycja, char *nazwa);
stosik *szukaj_miejsca(stosik *pocz, char *nazwa);
stosik *stworz_stosik(stosik *pocz, char *nazwa);
ciasteczko *dodaj_ciasteczko(stosik *aktualny);
void wyswietl_co_na_tacy(stosik *pocz, int liczba_stosikow);
stosik *zjedz_ciastka(stosik *aktualny, int *liczba, int *liczba_stosikow);
stosik *znajdz_pocz(stosik *pozycja, int liczba_stosikow);
void sprzatanie(stosik *zjadacz);
void zapisz(stosik *aktualny, int liczba_stosikow);
int wczytaj(stosik **dokladacz, stosik **pocz, stosik **zjadacz, int ilosc, int wysokosc);
