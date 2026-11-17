#define _XOPEN_SOURCE 700
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

extern char **environ;

/* getenv z wartoscia domyslna:  int razy = atoi(getenv_lub("TIMES", "1")); */
const char *getenv_lub(const char *nazwa, const char *domyslna)
{
    const char *v = getenv(nazwa);
    return v ? v : domyslna;
}

/* setenv z obsluga bledu. EINVAL = nazwa pusta albo zawiera '='. */
void ustaw_zmienna(const char *nazwa, const char *wartosc)
{
    if (setenv(nazwa, wartosc, 1)) /* 1 = nadpisz, jesli istnieje */
    {
        if (errno == EINVAL)
        {
            fprintf(stderr, "Zla nazwa zmiennej (pusta albo z '='): %s\n", nazwa);
            exit(EXIT_FAILURE);
        }
        ERR("setenv");
    }
}

/* Wypisuje cale srodowisko (jak polecenie env). */
void wypisz_srodowisko(void)
{
    for (int i = 0; environ[i]; i++)
        printf("%s\n", environ[i]);
}

/* Plik wyjsciowy z opcji -o wziety ze zmiennej srodowiskowej (lab2 ex2):
 * nazwa w zmiennej, NULL -> stdout. Zamknij przez: if (f != stdout) fclose(f); */
FILE *wyjscie_ze_zmiennej(const char *nazwa_zmiennej)
{
    const char *sciezka = getenv(nazwa_zmiennej);
    if (sciezka == NULL)
        return stdout;
    FILE *f = fopen(sciezka, "w");
    if (f == NULL)
        ERR("fopen");
    return f;
}
