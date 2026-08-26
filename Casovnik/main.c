
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DUZINA_NAZIVA   50
#define DUZINA_OPISA    100
#define NAZIV_DATOTEKE  "komande.txt"

typedef struct Komanda {
    int  sat;
    int  minut;
    int  sekund;
    char naziv[DUZINA_NAZIVA];
    char opis[DUZINA_OPISA];
    int  izvrsena;
    struct Komanda *sledeca;
} Komanda;

typedef struct {
    int sat;
    int minut;
    int sekund;
} Casovnik;

Komanda* kreirajKomandu(int sat, int minut, int sekund,
                         const char *naziv, const char *opis);
Komanda* dodajKomanduNaKraj(Komanda *glava, Komanda *nova);
void     prikaziKomande(Komanda *glava);
void     oslobodiListu(Komanda **glava);
void     sacuvajUDatoteku(Komanda *glava, const char *imeDatoteke);
Komanda* ucitajIzDatoteke(const char *imeDatoteke);
void     pomeriCasovnikZaJedanTakt(Casovnik *c);
int      uporediVreme(Casovnik c, int sat, int minut, int sekund);
void     ispisiVreme(Casovnik c);
void     pokreniSimulaciju(Komanda *glava, Casovnik pocetno, int brojTaktova);
void     obrisiKomandu(Komanda **glava, const char *naziv);
int      ucitajCeoBroj(const char *poruka, int min, int max);
void     ucitajTekst(char *bafer, int duzina, const char *poruka);
void     unosVisheKomandiUListu(Komanda **glava);
void     ucitajIzDatotekeIPrikaziNaEkran(Komanda **glava);
int main(void) {
    Komanda *glavaListe = NULL;
    int izbor;

    printf("=================================================================\n");
    printf("   SIMULACIJA: CASOVNIK REALNOG VREMENA U RACUNARU I IZVRSAVANJE \n KOMANDI U TAKTOVIMA \n");
    printf("=================================================================\n");

    do {
        printf("\n------------------------- GLAVNI MENI --------------------------\n");
        printf(" 1. Unos komandi u listu (unos vise komandi, pa snimanje svih u datoteku)\n");
        printf(" 2. Prikaz sadrzaja datoteke (ucitavanje iz datoteke u listu i na ekran)\n");
        printf(" 3. Dodaj samo jednu komandu u listu (bez snimanja)\n");
        printf(" 4. Prikazi trenutnu listu (iz memorije) na ekranu\n");
        printf(" 5. Sacuvaj trenutnu listu u datoteku\n");
        printf(" 6. Pokreni simulaciju casovnika (izvrsavanje po taktovima)\n");
        printf(" 7. Obrisi komandu po nazivu\n");
        printf(" 8. Obrisi sve komande (isprazni listu)\n");
        printf(" 0. Izlaz iz programa\n");
        printf("------------------------------------------------------------------\n");
        izbor = ucitajCeoBroj("Unesite izbor: ", 0, 8);

        switch (izbor) {


            case 1:
                unosVisheKomandiUListu(&glavaListe);
                break;

            case 2:
                ucitajIzDatotekeIPrikaziNaEkran(&glavaListe);
                break;

            case 3: {
                char naziv[DUZINA_NAZIVA];
                char opis[DUZINA_OPISA];
                int s, m, sek;

                s   = ucitajCeoBroj("Unesite sat izvrsavanja   (0-23): ", 0, 23);
                m   = ucitajCeoBroj("Unesite minut izvrsavanja (0-59): ", 0, 59);
                sek = ucitajCeoBroj("Unesite sekund izvrsavanja(0-59): ", 0, 59);

                ucitajTekst(naziv, DUZINA_NAZIVA, "Unesite naziv komande: ");
                ucitajTekst(opis,  DUZINA_OPISA,  "Unesite opis komande: ");

                Komanda *nova = kreirajKomandu(s, m, sek, naziv, opis);
                glavaListe = dodajKomanduNaKraj(glavaListe, nova);
                printf(">> Komanda je uspesno dodata u listu (u memoriji).\n");
                break;
            }

            case 4:
                prikaziKomande(glavaListe);
                break;

            case 5:
                sacuvajUDatoteku(glavaListe, NAZIV_DATOTEKE);
                break;

            case 6: {

                if (glavaListe == NULL) {
                    printf(">> Upozorenje: lista komandi je prazna - simulacija nema\n");
                    printf("   sta da izvrsi, ali casovnik ce ipak biti simuliran.\n");
                } else {
                    printf(">> Ucitano je zakazanih komandi - simulacija moze da pocne.\n");
                }

                Casovnik pocetno;
                pocetno.sat    = ucitajCeoBroj("Pocetni sat simulacije   (0-23): ", 0, 23);
                pocetno.minut  = ucitajCeoBroj("Pocetni minut simulacije (0-59): ", 0, 59);
                pocetno.sekund = ucitajCeoBroj("Pocetni sekund simulacije(0-59): ", 0, 59);
                int trajanje   = ucitajCeoBroj("Broj taktova (sekundi) simulacije (1-500): ", 1, 500);

                pokreniSimulaciju(glavaListe, pocetno, trajanje);
                break;
            }

            case 7: {
                char naziv[DUZINA_NAZIVA];
                ucitajTekst(naziv, DUZINA_NAZIVA, "Unesite naziv komande za brisanje: ");
                obrisiKomandu(&glavaListe, naziv);
                break;
            }

            case 8:
                oslobodiListu(&glavaListe);
                printf(">> Sve komande su obrisane iz liste.\n");
                break;

            case 0:
                printf("Hvala na koriscenju programa. Dovidjenja!\n");
                break;

            default:
                printf(">> Nepoznat izbor, pokusajte ponovo.\n");
        }

    } while (izbor != 0);


    oslobodiListu(&glavaListe);
    return 0;
}


Komanda* kreirajKomandu(int sat, int minut, int sekund, const char *naziv, const char *opis) {
    Komanda *nova = (Komanda*) malloc(sizeof(Komanda));
    if (nova == NULL) {
        printf("Greska: nema dovoljno memorije!\n");
        exit(1);
    }
    nova->sat    = sat;
    nova->minut  = minut;
    nova->sekund = sekund;

    strncpy(nova->naziv, naziv, DUZINA_NAZIVA - 1);
    nova->naziv[DUZINA_NAZIVA - 1] = '\0';

    strncpy(nova->opis, opis, DUZINA_OPISA - 1);
    nova->opis[DUZINA_OPISA - 1] = '\0';

    nova->izvrsena = 0;
    nova->sledeca  = NULL;
    return nova;
}

Komanda* dodajKomanduNaKraj(Komanda *glava, Komanda *nova) {
    if (glava == NULL) {
        return nova;
    }
    Komanda *tekuci = glava;
    while (tekuci->sledeca != NULL) {
        tekuci = tekuci->sledeca;
    }
    tekuci->sledeca = nova;
    return glava;
}

void prikaziKomande(Komanda *glava) {
    if (glava == NULL) {
        printf(">> Lista komandi je prazna.\n");
        return;
    }

    printf("\n%-4s %-10s %-18s %-28s %-10s\n",
           "Rb.", "Vreme", "Naziv", "Opis", "Status");
    printf("---------------------------------------------------------------------\n");

    int redniBroj = 1;
    for (Komanda *tekuci = glava; tekuci != NULL; tekuci = tekuci->sledeca, redniBroj++) {
        printf("%-4d %02d:%02d:%02d   %-18s %-28s %-10s\n",
               redniBroj, tekuci->sat, tekuci->minut, tekuci->sekund,
               tekuci->naziv, tekuci->opis,
               tekuci->izvrsena ? "IZVRSENA" : "CEKA");
    }
}

void oslobodiListu(Komanda **glava) {
    Komanda *tekuci = *glava;
    while (tekuci != NULL) {
        Komanda *sledeci = tekuci->sledeca;
        free(tekuci);
        tekuci = sledeci;
    }
    *glava = NULL;
}

void obrisiKomandu(Komanda **glava, const char *naziv) {
    Komanda *tekuci = *glava;
    Komanda *prethodni = NULL;

    while (tekuci != NULL) {
        if (strcmp(tekuci->naziv, naziv) == 0) {
            if (prethodni == NULL) {
                *glava = tekuci->sledeca;
            } else {
                prethodni->sledeca = tekuci->sledeca;
            }
            free(tekuci);
            printf(">> Komanda \"%s\" je obrisana.\n", naziv);
            return;
        }
        prethodni = tekuci;
        tekuci = tekuci->sledeca;
    }
    printf(">> Komanda sa nazivom \"%s\" nije pronadjena.\n", naziv);
}


void sacuvajUDatoteku(Komanda *glava, const char *imeDatoteke) {
    FILE *dat = fopen(imeDatoteke, "w");
    if (dat == NULL) {
        printf(">> Greska pri otvaranju datoteke za upis!\n");
        return;
    }

    int brojac = 0;
    for (Komanda *tekuci = glava; tekuci != NULL; tekuci = tekuci->sledeca) {
        fprintf(dat, "%d %d %d %d\n%s\n%s\n",
                tekuci->sat, tekuci->minut, tekuci->sekund,
                tekuci->izvrsena, tekuci->naziv, tekuci->opis);
        brojac++;
    }

    fclose(dat);
    printf(">> Uspesno sacuvano komandi u datoteku: %d (datoteka: \"%s\")\n",
           brojac, imeDatoteke);
}

Komanda* ucitajIzDatoteke(const char *imeDatoteke) {
    FILE *dat = fopen(imeDatoteke, "r");
    if (dat == NULL) {
        printf(">> Datoteka \"%s\" ne postoji. Ucitana je prazna lista.\n", imeDatoteke);
        return NULL;
    }

    Komanda *glava = NULL;
    int s, m, sek, izv;
    char naziv[DUZINA_NAZIVA];
    char opis[DUZINA_OPISA];
    int brojac = 0;

    while (fscanf(dat, "%d %d %d %d\n", &s, &m, &sek, &izv) == 4) {
        if (fgets(naziv, DUZINA_NAZIVA, dat) == NULL) break;
        naziv[strcspn(naziv, "\n")] = '\0';

        if (fgets(opis, DUZINA_OPISA, dat) == NULL) break;
        opis[strcspn(opis, "\n")] = '\0';

        Komanda *nova = kreirajKomandu(s, m, sek, naziv, opis);
        nova->izvrsena = izv;
        glava = dodajKomanduNaKraj(glava, nova);
        brojac++;
    }

    fclose(dat);
    printf(">> Uspesno ucitano komandi iz datoteke: %d (datoteka: \"%s\")\n",
           brojac, imeDatoteke);
    return glava;
}

void pomeriCasovnikZaJedanTakt(Casovnik *c) {
    c->sekund++;
    if (c->sekund >= 60) {
        c->sekund = 0;
        c->minut++;
        if (c->minut >= 60) {
            c->minut = 0;
            c->sat++;
            if (c->sat >= 24) {
                c->sat = 0;
            }
        }
    }
}

int uporediVreme(Casovnik c, int sat, int minut, int sekund) {
    if (c.sat == sat && c.minut == minut && c.sekund == sekund)
        return 1;
    else
        return 0;
}

void ispisiVreme(Casovnik c) {
    printf("%02d:%02d:%02d", c.sat, c.minut, c.sekund);
}

void pokreniSimulaciju(Komanda *glava, Casovnik pocetno, int brojTaktova) {
    Casovnik trenutno = pocetno;

    printf("\n>>> POKRETANJE SIMULACIJE CASOVNIKA <<<\n");
    printf(">>> Pocetno vreme: ");
    ispisiVreme(trenutno);
    printf("   |   Broj taktova: %d\n\n", brojTaktova);

    for (int takt = 1; takt <= brojTaktova; takt++) {
        pomeriCasovnikZaJedanTakt(&trenutno);

        printf("[Takt %3d]  Trenutno vreme sistema: ", takt);
        ispisiVreme(trenutno);
        printf("\n");

        Komanda *tekuci = glava;
        while (tekuci != NULL) {
            if (!tekuci->izvrsena &&
                uporediVreme(trenutno, tekuci->sat, tekuci->minut, tekuci->sekund)) {
                printf("    >> PREKID (INTERRUPT): izvrsava se komanda \"%s\"\n",
                       tekuci->naziv);
                printf("       Opis komande: %s\n", tekuci->opis);
                tekuci->izvrsena = 1;
            }
            tekuci = tekuci->sledeca;
        }

        sleep(1);
    }

    printf("\n>>> SIMULACIJA JE ZAVRSENA <<<\n");
}


int ucitajCeoBroj(const char *poruka, int min, int max) {
    int broj;
    int uspesno;

    do {
        printf("%s", poruka);
        uspesno = scanf("%d", &broj);
        while (getchar() != '\n')
            ;

        if (!uspesno) {
            printf(">> Greska: unesite ceo broj!\n");
            broj = min - 1;
        } else if (broj < min || broj > max) {
            printf(">> Greska: broj mora biti u opsegu [%d, %d]!\n", min, max);
        }
    } while (broj < min || broj > max);

    return broj;
}

void ucitajTekst(char *bafer, int duzina, const char *poruka) {
    printf("%s", poruka);
    fgets(bafer, duzina, stdin);
    bafer[strcspn(bafer, "\n")] = '\0';
}

void unosVisheKomandiUListu(Komanda **glava) {
    char nastavi;

    printf("\n>>> UNOS KOMANDI U LISTU <<<\n");

    do {
        char naziv[DUZINA_NAZIVA];
        char opis[DUZINA_OPISA];
        int s, m, sek;

        s   = ucitajCeoBroj("Unesite sat izvrsavanja   (0-23): ", 0, 23);
        m   = ucitajCeoBroj("Unesite minut izvrsavanja (0-59): ", 0, 59);
        sek = ucitajCeoBroj("Unesite sekund izvrsavanja(0-59): ", 0, 59);

        ucitajTekst(naziv, DUZINA_NAZIVA, "Unesite naziv komande: ");
        ucitajTekst(opis,  DUZINA_OPISA,  "Unesite opis komande: ");

        Komanda *nova = kreirajKomandu(s, m, sek, naziv, opis);
        *glava = dodajKomanduNaKraj(*glava, nova);
        printf(">> Komanda \"%s\" je dodata u listu.\n", naziv);

        printf("Da li zelite da unesete jos jednu komandu? (d/n): ");
        scanf(" %c", &nastavi);
        while (getchar() != '\n')
            ;

    } while (nastavi == 'd' || nastavi == 'D');

    printf("\n>> Unos je zavrsen. Cuvanje cele liste u datoteku...\n");
    sacuvajUDatoteku(*glava, NAZIV_DATOTEKE);
}

void ucitajIzDatotekeIPrikaziNaEkran(Komanda **glava) {
    printf("\n>>> UCITAVANJE IZ DATOTEKE U LISTU <<<\n");

    oslobodiListu(glava);
    *glava = ucitajIzDatoteke(NAZIV_DATOTEKE);

    printf("\n>>> PRIKAZ UCITANE LISTE NA EKRANU <<<\n");
    prikaziKomande(*glava);
}
