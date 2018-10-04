#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"spis.h"

char *wczytaj_wyrazy(char *wyraz, int ile){
	char * wynik;
	char * koniec;

	wynik = fgets(wyraz, ile, stdin);
	if(wyraz != NULL){
		koniec = strchr(wyraz, '\n');
		if(koniec != NULL)
			*koniec = '\0';
		else
			while(getchar() != '\n');
	}

	return wynik;
}

int wczytaj_liczbe(){
    	char liczba[LICZBA_DL];
    	int i;
    	int wynik = 0, pozycja = 1;
        wczytaj_wyrazy(liczba, LICZBA_DL);

	for(i = 0 ; i < strlen(liczba) ; i++)
		pozycja *= 10;
	for(i = 0 ; i < strlen(liczba) ; i++){
        	if((liczba[i] - '\0') >= 0 && (liczba[i] - '0') <=9)
          	 	wynik += (liczba[i] - '0')*(pozycja/= 10);
        	else
            		return -1;
	}
	return wynik;
}

/*sprawdza, czy stosik o zadanej nazwie istnieje, jesli tak zwraca wskaznik na stosik*/
stosik *sprawdz_czy_istnieje(stosik *pocz, char *nazwa){
	stosik *aktualny;
	if(pocz == NULL){
		return NULL;
	}
	else{
		if(strcmp(pocz->nazwa, nazwa) == 0)
			return pocz;
		aktualny = pocz->nast;

		while(aktualny != pocz){
			if(strcmp(aktualny->nazwa, nazwa) == 0)
				return aktualny;
			aktualny = aktualny->nast;
		}
		return NULL;
	}
}

/*zwraca wskaznik na stosik, po ktorym powinien sie znalezc stosik*/
stosik *szukaj_miejsca(stosik *pocz, char *nazwa){
	stosik *pozniejszy;
	if(strcmp(nazwa, pocz->nazwa) < 0)
		return pocz->poprz;

	pozniejszy = pocz->nast;
	while((strcmp(pozniejszy->nazwa, nazwa) < 0) && pozniejszy != pocz)
		pozniejszy = pozniejszy->nast;
	return pozniejszy->poprz;
}

/*tworzy stosik*/
stosik *stworz_stosik(stosik *pocz, char *nazwa){
	stosik *aktualny, *wczesniejszy, *pozniejszy;

	if((aktualny = (stosik *)malloc(sizeof(stosik))) != NULL){
		strcpy(aktualny->nazwa, nazwa);

		if(pocz == NULL){
			aktualny->nast = aktualny;
			aktualny->poprz = aktualny;
		}

		else{
			wczesniejszy = szukaj_miejsca(pocz, nazwa);
			pozniejszy = wczesniejszy->nast;

			if(wczesniejszy == pocz->poprz && strcmp(aktualny->nazwa, pocz->nazwa) < 0)
				pocz = aktualny;
			aktualny->poprz = wczesniejszy;
			aktualny->nast = pozniejszy;
			pozniejszy->poprz = aktualny;
			wczesniejszy->nast = aktualny;
		}

		aktualny->na_wierzchu = NULL;
	}

	return aktualny;
}

/*tworzy nowe ciasteczko*/
ciasteczko *dodaj_ciasteczko(stosik *aktualny){
	ciasteczko *nowe;
	if((nowe = (ciasteczko *)malloc(sizeof(ciasteczko))) != NULL){
		if(aktualny->na_wierzchu == NULL){
			nowe->liczba = 1;
			nowe->poprz = NULL;
		}
		else{
			nowe->liczba = (aktualny->na_wierzchu->liczba)+1;
			nowe->poprz = aktualny->na_wierzchu;
		}
	}
	return nowe;
}

/*wyswietla co jest na tacy*/
void wyswietl_co_na_tacy(stosik *pocz, int liczba_stosikow){
	stosik *aktualny;
	int i;

	if(liczba_stosikow == 0)
		printf("\nTaca jest pusta.\n");

	else{
		aktualny = pocz;
		for(i = 0 ; i < liczba_stosikow ; i++){
			printf("Ciastka \"%s\"   szt. %d\n", aktualny->nazwa, aktualny->na_wierzchu->liczba);
			aktualny = aktualny->nast;
		}
	}
	return;
}

/*usuwa ciasteczka ze stosika - kolejno je zdejmujac. W przypadku podania zbyt duzej liczby zjada wszystkie ciasteczka na stosiku*/
stosik *zjedz_ciastka(stosik *aktualny, int *liczba, int *liczba_stosikow){
	ciasteczko *pom = NULL;
	stosik *wynik = NULL;

	while(*liczba > 0 && aktualny->na_wierzchu != NULL){
		pom = aktualny->na_wierzchu->poprz;
		free(aktualny->na_wierzchu);
		aktualny->na_wierzchu = pom;
		--*liczba;
		printf("Chrup\n");
	}

	if(aktualny->na_wierzchu == NULL){
		if(aktualny->poprz == aktualny){
			free(aktualny);
			wynik = NULL;
		}
			
		else{
			wynik = aktualny->poprz;
			aktualny->poprz->nast = aktualny->nast;
			aktualny->nast->poprz = aktualny->poprz;
			free(aktualny);
		}
		--*liczba_stosikow;
	}

	else
		wynik = aktualny;

	return wynik;
}

stosik *znajdz_pocz(stosik *pozycja, int liczba_stosikow){
	stosik *pocz, *aktualny;
	int i;
	aktualny = pocz = pozycja;
	for(i = 0 ; i < liczba_stosikow ; i++){
		if(strcmp(aktualny->nazwa, pocz->nazwa) < 0)
			pocz = aktualny;
		aktualny = aktualny->nast;
	}

	return pocz;
}

void sprzatanie(stosik *zjadacz){
	stosik *pom;
	ciasteczko *pom_c;
	if(zjadacz != NULL){
		zjadacz->poprz->nast = NULL;
		while(zjadacz != NULL){
			pom = zjadacz->nast;
			while(zjadacz->na_wierzchu != NULL){
				pom_c = zjadacz->na_wierzchu->poprz;
				free(zjadacz->na_wierzchu);
				zjadacz->na_wierzchu = pom_c;
			}

			if(zjadacz->poprz == zjadacz){
				free(zjadacz);
				pom = NULL;
			}
			
			else{
				zjadacz->poprz->nast = zjadacz->nast;
				zjadacz->nast->poprz = zjadacz->poprz;
				free(zjadacz);
			}
			zjadacz = pom;
		}
	}
}

void zapisz(stosik *aktualny, int liczba_stosikow){
	FILE *wy;
	int i, liczba;
	char nazwa[MAX_NAZWA], nazwa_pliku[MAX_NAZWA];
	/*ew: uzytkownik moze zstac poproszony o podanie nazwy*/

	printf("\nPodaj nazwe pliku w ktorym chcesz zapisac dane: ");
	wczytaj_wyrazy(nazwa_pliku, MAX_NAZWA);

	if(nazwa_pliku[0] == '\0'){
		printf("Nie utworzono zapisu\n");
		return;
	}

	if((wy = fopen(nazwa_pliku, "wb")) == NULL)
		printf("Blad przy otwieraniu pliku do zapisu.\n");

	else{
		for(i = 0 ; i < liczba_stosikow ; i++){
			strcpy(nazwa, aktualny->nazwa);
			liczba = aktualny->na_wierzchu->liczba;

			if(fwrite(nazwa, MAX_NAZWA, 1, wy) != 1)
				printf("Blad przy zapisywaniu nazwy\n");
			if(fwrite(&liczba, sizeof(int), 1, wy) != 1)
				printf("Blad przy zapisywaniu liczby\n");

			aktualny = aktualny->nast;
		}

		fclose(wy);
	}
}

int wczytaj(stosik **dokladacz, stosik **pocz, stosik **zjadacz, int ilosc, int wysokosc){
	FILE *we;
	int i, j, liczba = 0;
	int liczba_stosikow = 0;
	char nazwa[MAX_NAZWA], nazwa_pliku[MAX_NAZWA];
	ciasteczko *pom_c;
	stosik *pom;

	printf("\nPodaj nazwe pliku ktory chcesz wczytac: ");
	wczytaj_wyrazy(nazwa_pliku, MAX_NAZWA);

	if((we = fopen(nazwa_pliku, "rb")) == NULL)
		printf("\nNie udalo sie wczytac pliku\n");
	else{
		for(i = 0 ; i < ilosc ; i++){
			if(!feof(we)){
				if(fread(nazwa, sizeof(char), MAX_NAZWA, we) != MAX_NAZWA)
					break;

				if(fread(&liczba, sizeof(int), 1, we) != 1)
					break;

				if(liczba_stosikow <= ilosc){
					if((pom = stworz_stosik(*pocz, nazwa)) != NULL){
						*dokladacz = pom;
						++liczba_stosikow;
						if(liczba > wysokosc)
							liczba = wysokosc;
						for(j = 0 ; j < liczba ; j++){
							if((pom_c = dodaj_ciasteczko(*dokladacz)) != NULL)
								(*dokladacz)->na_wierzchu = pom_c;
						}
					}
					*pocz = znajdz_pocz((*dokladacz), liczba_stosikow);
				}
			}
			else
				break;
		}
		fclose(we);
	}
	*dokladacz = *pocz;

	if(liczba_stosikow >= 1){
		*zjadacz = *dokladacz;
		for(i = 0 ; i < liczba_stosikow/2 ; i++)
			*zjadacz = (*zjadacz)->nast;
	}
	return liczba_stosikow;
}
