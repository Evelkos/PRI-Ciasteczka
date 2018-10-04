#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"spis.h"

int main(){
	stosik *dokladacz = NULL, *zjadacz = NULL, *pocz = NULL, *pom = NULL; /*wskazniki na dwa stosy bedace naprzeciw siebie*/
	ciasteczko *pom_c; /*pomocniczy wskaznik na ciasteczko*/
	char nazwa[MAX_NAZWA];
	int liczba = 0, liczba_stosikow = 0;
	int wysokosc, ilosc, i; /*ilosc ciasteczek, ktore maksymalnie moga tworzyc stosik, maksymalna ilosc stosikow oraz zmienna pomocnicza*/

	printf("\nUWAGA: PODANIE ZBYT MALYCH WARTOSCI MOZE SPOWODOWAC OBCIECIE DANYCH.\n");
	printf("Podaj maksymalna liczbe ciastek na stosiku (z przedzialu od 3 do 10)\n");
	while((wysokosc = wczytaj_liczbe()) == -1 || wysokosc < 3 || wysokosc > 10)
		printf("Podaj liczbe z przedzialu od 3 do 10\n");

	printf("\nPodaj maksymalna liczbe stosikow na tacy (z przedzialu od 3 do 20)\n");
	while((ilosc = wczytaj_liczbe()) == -1 || ilosc < 3 || ilosc > 20)
		printf("Podaj liczbe z przedzialu od 3 do 20\n");
	
	liczba_stosikow = wczytaj(&dokladacz, &pocz, &zjadacz, ilosc, wysokosc);

	while(1){
		printf("\nWitaj, kim teraz jestes? (wybierz jedna z ponizszych opcji)\n");
		printf("1. Teraz jestem zjadaczem!\n");
		printf("2. A ja tu tylko ciastka dokladam...\n");
		printf("3. Chyba sie zgubilem... zegnam\n");
		printf("\nWYBOR: ");
		while((liczba = wczytaj_liczbe()) == -1 || (liczba != 1 && liczba != 2 && liczba!= 3))
			printf("Podaj 1, 2 lub 3\n");

		if(liczba == 1){
			while(1){
				printf("\nZJADACZ\n");
				printf("1. Spojrz, co jest na tacy\n");
				printf("2. Zjedz ciastka!\n");
				printf("3. Zmien osobe\n");
				printf("\nWYBOR: ");

				while((liczba = wczytaj_liczbe()) == -1 || (liczba != 1 && liczba != 2 && liczba!= 3))
					printf("Podaj 1, 2 lub 3\n");

				if(liczba == 1)
					wyswietl_co_na_tacy(pocz, liczba_stosikow);

				else if(liczba == 2){
					while(1){
						printf("\nZjadacz patrzy na stosik przed soba");
						if(zjadacz == NULL){
							printf("... i zadnego nie widzi.\n");
							printf("- Jak dlugo ta taca bedzie jeszcze pusta? Doloz cos, Dokladaczu!\n");
							break;
						}
						printf(". Pyszne ciasteczko %s zdaje sie do niego usmiechac\n", zjadacz->nazwa);
						printf("1. Zjedz ciasteczka lezace przed toba\n");
						printf("2. Obroc tace\n");
						printf("3. Jednak nie jestem glodny...\n");
						printf("\nWYBOR: ");

						while((liczba = wczytaj_liczbe()) == -1 || (liczba != 1 && liczba != 2 && liczba!= 3))
							printf("Podaj 1, 2 lub 3\n");

						if(liczba == 1){
							while(1){
								printf("Ile ciastek zjesc? (UWAGA: podanie zbyt duzej liczby spowoduje zjedzenie wszystkich ciasteczek ze stosika)\n");
								if((liczba = wczytaj_liczbe()) != -1)
									break;
							}
							zjadacz = zjedz_ciastka(zjadacz, &liczba, &liczba_stosikow);	/*zjedzenie ciastek*/
							pocz = znajdz_pocz(zjadacz, liczba_stosikow);					/*szukanie poczatku*/
							break;
						}
						else if(liczba == 2){
							while(1){
								printf("Podaj nazwe ciastka, na ktorego pozycje chcesz obrocic tace: ");
								wczytaj_wyrazy(nazwa, MAX_NAZWA);
								if(nazwa[0] != '\0')
									break;
							}
							if((pom = sprawdz_czy_istnieje(zjadacz, nazwa)) == NULL)
								printf("Hmm... takich ciasteczek chyba nie ma.\n");

							else
								zjadacz = pom;
						}

						else if(liczba == 3)
							break;
					}

					if(liczba_stosikow >= 1){	/*dokladacz po przeciwnej stronie tacy*/
						dokladacz = zjadacz;
						for(i = 0 ; i < liczba_stosikow/2 ; i++)
							dokladacz = dokladacz->nast;
					}
				}

				else if(liczba == 3)
					break;
			}
		}

		else if(liczba == 2){
			while(1){
				printf("\nDOKLADACZ\n");
				printf("1. Spojrz, co jest na tacy\n");
				printf("2. Doloz ciastka\n");
				printf("3. Zmien osobe\n");
				printf("\nWYBOR: ");

				while((liczba = wczytaj_liczbe()) == -1 || (liczba != 1 && liczba != 2 && liczba!= 3))
					printf("Podaj 1, 2 lub 3\n");

				if(liczba == 1)
					wyswietl_co_na_tacy(pocz, liczba_stosikow);

				else if(liczba == 2){
					while(1){
						printf("Podaj nazwe ciastka jakie chcesz dolozyc: ");
						wczytaj_wyrazy(nazwa, MAX_NAZWA);
						if(nazwa[0] != '\0')
							break;
					}

					if((dokladacz = sprawdz_czy_istnieje(pocz, nazwa)) == NULL){
						if(liczba_stosikow < ilosc){
							if((pom = stworz_stosik(pocz, nazwa)) != NULL){
								dokladacz = pom;
								liczba_stosikow++;
								printf("Wlasnie utworzylem nowy stosik ciastek: %s\n", dokladacz->nazwa);

								if(liczba_stosikow >= 1){	/*zjadacz po przeciwnej stronie tacy*/
									zjadacz = dokladacz;
									for(i = 0 ; i < liczba_stosikow/2 ; i++)
										zjadacz = zjadacz->nast;
								}
								pocz = znajdz_pocz(dokladacz, liczba_stosikow);
							}
						}
						else{
							printf("Na tacy nie ma juz miejsca. Nie moge utworzyc kolejnego stosika\n");
							dokladacz = NULL;
						}
					}
					if(dokladacz != NULL && (dokladacz->na_wierzchu == NULL || dokladacz->na_wierzchu->liczba < wysokosc)){
						if((pom_c = dodaj_ciasteczko(dokladacz)) != NULL)
							dokladacz->na_wierzchu = pom_c;
						else
							printf("Dziwne... nie moge dolozyc ciasteczka. Jest problem.\n");
					}

					else if(dokladacz == NULL && zjadacz != NULL){
						if(liczba_stosikow >= 1){	/*dokladacz po przeciwnej stronie tacy - ustawianie na nowo*/
							dokladacz = zjadacz;
							for(i = 0 ; i < liczba_stosikow/2 ; i++)
								dokladacz = dokladacz->nast;
						}
					}

					else if(dokladacz->na_wierzchu->liczba >= wysokosc)
						printf("O nie! Ten stosik jest zbyt wysoki. Nie moge dolozyc kolejnego ciasteczka.\n");

					else
						printf("Wystapil blad\n");
				}

				else if(liczba == 3)
					break;
			}
        }

		else if(liczba == 3)
			break;
	}
	zapisz(pocz, liczba_stosikow);
	sprzatanie(zjadacz);
	return 0;
}
