#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* (sop-library, lab2 ex4) Baza = rekordy po 68 B: uint32 rozmiar + 64 B tytulu (dopelniony '\0').
 * Rekord nr i lezy pod offsetem i * REKORD. pread/pwrite = bez osobnego lseek.
 */
#define REKORD 68
#define TYTUL 64

typedef struct
{
    uint32_t rozmiar;
    char tytul[TYTUL + 1]; /* +1 na '\0' — w pliku jest tylko 64 */
} Ksiazka;

/* Czyta rekord nr i. Zwraca 1 = ok, 0 = poza plikiem. */
int czytaj_rekord(int fd, int i, Ksiazka *k)
{
    struct iovec iov[2] = {{&k->rozmiar, 4}, {k->tytul, TYTUL}};
    if (lseek(fd, (off_t)i * REKORD, SEEK_SET) == -1)
        ERR("lseek");
    ssize_t n = readv(fd, iov, 2);
    if (n == -1)
        ERR("readv");
    if (n < REKORD)
        return 0;
    k->tytul[TYTUL] = '\0';
    return 1;
}

/* Wczytuje WSZYSTKIE rekordy do tablicy (trzeba free). *ile = liczba rekordow. */
Ksiazka *wczytaj_wszystkie(int fd, int *ile)
{
    struct stat st;
    if (fstat(fd, &st))
        ERR("fstat");
    if (st.st_size % REKORD)
    {
        fprintf(stderr, "Uszkodzona baza\n");
        exit(EXIT_FAILURE);
    }
    *ile = (int)(st.st_size / REKORD);
    Ksiazka *tab = calloc(*ile > 0 ? *ile : 1, sizeof(Ksiazka));
    if (!tab)
        ERR("calloc");
    for (int i = 0; i < *ile; i++)
        czytaj_rekord(fd, i, &tab[i]);
    return tab;
}

/* Nadpisuje tylko pole rozmiaru w rekordzie nr i (pierwsze 4 bajty rekordu). */
void zmien_rozmiar(int fd, int i, uint32_t nowy)
{
    if (pwrite(fd, &nowy, 4, (off_t)i * REKORD) != 4)
        ERR("pwrite");
}

/* Dopisuje nowy rekord na koniec pliku (tytul dopelniony zerami do 64 B, obciety gdy dluzszy). */
void dopisz_rekord(int fd, uint32_t rozmiar, const char *tytul)
{
    char tyt[TYTUL];
    memset(tyt, 0, TYTUL);
    strncpy(tyt, tytul, TYTUL);
    struct iovec iov[2] = {{&rozmiar, 4}, {tyt, TYTUL}};
    if (lseek(fd, 0, SEEK_END) == -1)
        ERR("lseek");
    if (writev(fd, iov, 2) != REKORD)
        ERR("writev");
}

/* Przyklad (filtr po prefiksie tytulu, jak -s w sop-library):
 *   int n; Ksiazka *t = wczytaj_wszystkie(fd, &n);
 *   for (int i = 0; i < n; i++)
 *       if (!prefiks || strncmp(t[i].tytul, prefiks, strlen(prefiks)) == 0)
 *           printf("Size: %u, Title: %s\n", t[i].rozmiar, t[i].tytul);
 *   free(t);
 */
