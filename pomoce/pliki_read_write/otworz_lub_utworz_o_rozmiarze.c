#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef ERR
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
#endif

/* (sop-merge, sop-ratking, sop-cake) Plik danych o STALYM rozmiarze:
 * jesli nie istnieje -> utworz i wypelnij bajtem `wypelnienie` (0 albo ' ');
 * jesli istnieje -> sprawdz rozmiar, zly = "uszkodzony" -> koniec programu.
 * Zwraca fd otwarty O_RDWR, kursor na poczatku.
 *
 *   int fd = otworz_lub_utworz(argv[1], 128, 0);      // mapa 16 x (2 x uint32)
 *   int fd = otworz_lub_utworz(argv[1], 2048, ' ');   // 32 linie po 64 znaki
 *
 * Sprawdzenie "rozmiar % RECORD == 0" (baza rekordow) -> rozmiar_podzielny_przez().
 */
int otworz_lub_utworz(const char *sciezka, off_t rozmiar, char wypelnienie)
{
    int fd = open(sciezka, O_RDWR);
    if (fd == -1)
    {
        fd = open(sciezka, O_RDWR | O_CREAT, 0644);
        if (fd == -1)
            ERR("open");
        char blok[4096];
        memset(blok, wypelnienie, sizeof(blok));
        off_t zostalo = rozmiar;
        while (zostalo > 0)
        {
            size_t kawalek = zostalo < (off_t)sizeof(blok) ? (size_t)zostalo : sizeof(blok);
            if (write(fd, blok, kawalek) != (ssize_t)kawalek)
                ERR("write");
            zostalo -= kawalek;
        }
    }
    struct stat st;
    if (fstat(fd, &st) == -1) /* fstat nie rusza kursora (w odroznieniu od lseek SEEK_END) */
        ERR("fstat");
    if (st.st_size != rozmiar)
    {
        fprintf(stderr, "%s: zly rozmiar pliku (%lld zamiast %lld)\n", sciezka, (long long)st.st_size,
                (long long)rozmiar);
        exit(EXIT_FAILURE);
    }
    if (lseek(fd, 0, SEEK_SET) == -1)
        ERR("lseek");
    return fd;
}

/* Liczba rekordow w pliku; konczy program, gdy rozmiar nie jest wielokrotnoscia rekordu. */
int rozmiar_podzielny_przez(int fd, int rekord)
{
    struct stat st;
    if (fstat(fd, &st) == -1)
        ERR("fstat");
    if (st.st_size % rekord != 0)
    {
        fprintf(stderr, "Uszkodzona baza\n");
        exit(EXIT_FAILURE);
    }
    return (int)(st.st_size / rekord);
}
