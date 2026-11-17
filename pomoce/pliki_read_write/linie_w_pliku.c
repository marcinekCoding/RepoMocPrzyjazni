#define _XOPEN_SOURCE 700
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* (baza_wiedzy.h, lab2 ex3 requirements) Plik tekstowy linia po linii przez getline. */

/* Pierwsza linia zaczynajaca sie od prefiksu (bez '\n', trzeba free) albo NULL. */
char *znajdz_linie(const char *sciezka, const char *prefiks)
{
    FILE *f = fopen(sciezka, "r");
    if (f == NULL)
        return NULL;
    char *linia = NULL, *wynik = NULL;
    size_t cap = 0;
    while (wynik == NULL && getline(&linia, &cap, f) != -1)
    {
        if (strncmp(linia, prefiks, strlen(prefiks)) == 0)
        {
            linia[strcspn(linia, "\r\n")] = '\0';
            wynik = strdup(linia);
        }
    }
    free(linia);
    fclose(f);
    return wynik;
}

/* Usuwa wszystkie linie zaczynajace sie od prefiksu (zapis do .tmp + rename,
 * uprawnienia zachowane). Zwraca ile usunieto, -1 gdy pliku nie ma.
 *   usun_linie(path, "numpy ");   // usun pakiet z requirements */
int usun_linie(const char *sciezka, const char *prefiks)
{
    struct stat st;
    if (stat(sciezka, &st) == -1)
    {
        if (errno == ENOENT)
            return -1;
        ERR("stat");
    }
    char tmp[4096];
    snprintf(tmp, sizeof(tmp), "%s.tmp", sciezka);
    FILE *in = fopen(sciezka, "r");
    FILE *out = fopen(tmp, "w");
    if (!in || !out)
        ERR("fopen");
    char *linia = NULL;
    size_t cap = 0;
    int usuniete = 0;
    while (getline(&linia, &cap, in) != -1)
    {
        if (strncmp(linia, prefiks, strlen(prefiks)) == 0)
            usuniete++;
        else if (fputs(linia, out) == EOF)
            ERR("fputs");
    }
    free(linia);
    fclose(in);
    fclose(out);
    if (chmod(tmp, st.st_mode & 07777) || rename(tmp, sciezka))
        ERR("rename");
    return usuniete;
}

/* Wywoluje f dla kazdej linii (bez '\n'). Zwraca liczbe linii, -1 gdy brak pliku. */
int dla_kazdej_linii(const char *sciezka, void (*f)(char *linia, int nr, void *dane), void *dane)
{
    FILE *pl = fopen(sciezka, "r");
    if (pl == NULL)
        return -1;
    char *linia = NULL;
    size_t cap = 0;
    int nr = 0;
    while (getline(&linia, &cap, pl) != -1)
    {
        linia[strcspn(linia, "\r\n")] = '\0';
        f(linia, nr++, dane);
    }
    free(linia);
    fclose(pl);
    return nr;
}
