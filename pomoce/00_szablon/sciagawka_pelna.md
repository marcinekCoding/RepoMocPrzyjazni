# Ściągawka SOP1 — gotowe wzorce do kopiowania

Szukaj przez Ctrl+F po słowach kluczowych w nagłówkach. Każdy blok kodu jest
samodzielny — kopiuj, wklejaj, dostosuj nazwy zmiennych/stałych do zadania.

---

## Nagłówki (komplet, nie trzeba myśleć które potrzebne)

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>      // readv/writev
#include <sys/mman.h>      // mmap
#include <poll.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>          // kompiluj z -lm jeśli używasz
#include <signal.h>        // SIGPIPE

#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))
```

---

## Walidacja argc

```c
if (argc < MIN || argc > MAX)
{
    fprintf(stderr, "Uzycie: %s <argumenty>\n", argv[0]);
    exit(EXIT_FAILURE);
}
```
Rób to **zawsze na samym początku `main`**, zanim dotkniesz `argv[2]` i dalej.

---

## Otwieranie zwykłego pliku binarnego (tworzenie jeśli nie istnieje, walidacja rozmiaru)

```c
int open_and_validate(char* path, int expected_size)
{
    int fd = open(path, O_RDWR);
    if (fd == -1)
    {
        fd = open(path, O_RDWR | O_CREAT, 0644);
        if (fd == -1) ERR("open");
        char* zeros = calloc(1, expected_size);
        write(fd, zeros, expected_size);
        free(zeros);
        lseek(fd, 0, SEEK_SET);
    }
    struct stat st;
    fstat(fd, &st);
    if (st.st_size != expected_size) ERR("zly rozmiar pliku");
    return fd;
}
```
`fstat` zamiast `lseek(fd,0,SEEK_END)` do sprawdzenia rozmiaru — nie przesuwa kursora pliku, więc nie musisz go potem wracać na 0.

---

## Otwieranie FIFO — wariant "nigdy się nie blokuj" (trik O_RDWR)

Używaj, gdy **Ty** masz czytać z FIFO i nie obchodzi Cię prawdziwa semantyka czytelnik/pisarz (najczęstszy przypadek na tych labach):

```c
int open_fifo_rdwr(char* path)
{
    int fd = open(path, O_RDWR | O_NONBLOCK);
    if (fd == -1) ERR("open fifo");
    struct stat st;
    if (fstat(fd, &st) == -1) ERR("fstat");
    if (!S_ISFIFO(st.st_mode))
    {
        fprintf(stderr, "%s nie jest FIFO\n", path);
        exit(EXIT_FAILURE);
    }
    return fd;
}
```

## Otwieranie FIFO — wariant "prawdziwy czytelnik" (O_RDONLY)

Używaj, gdy zadanie wymaga realnej semantyki czytelnik/pisarz i musisz odróżnić "brak danych teraz" od "koniec":

```c
int fd = open(path, O_RDONLY | O_NONBLOCK);   // nie blokuje się na open()
// read() może teraz zwrocic:
//   n > 0            -> normalne dane
//   n == 0            -> EOF, wszyscy pisarze odpięci, mozna zamknac fd
//   n == -1 && errno == EAGAIN -> brak danych TERAZ, pisarz nadal moze wrocic, NIE zamykaj
```

## Otwieranie FIFO — wariant "pisarz" (O_WRONLY, trzeba sprawdzić błąd open!)

```c
signal(SIGPIPE, SIG_IGN);   // zeby write() zwracal EPIPE zamiast zabijac proces
int fd = open(path, O_WRONLY | O_NONBLOCK);
if (fd == -1 && errno == ENXIO)
{
    // nikt jeszcze nie czyta - open() od razu failuje, trzeba czekac/ponowic
}
```

---

## Czytanie całego FIFO/pliku do EOF (prosty wariant, jedna wiadomość = cały strumień)

```c
void czytaj_do_eof(int fd)
{
    char buf[4096];
    int n;
    while ((n = read(fd, buf, sizeof(buf))) > 0)
    {
        write(STDOUT_FILENO, buf, n);   // NIE printf("%s") - buf nie jest null-terminated!
    }
    if (n == -1) ERR("read");
}
```

---

## Dzielenie strumienia na wiadomości zakończone `\n` (WZORZEC SPRAWDZONY, NIE patchuj inaczej)

To jest **ten** fragment, który najczęściej psujemy. Kluczowe zasady:
1. Dopisz nowe dane do bufora w JEDNYM miejscu.
2. Przeskanuj CAŁY zbuforowany tekst (nie tylko nowo wczytany kawałek) pętlą — może być kilka pełnych wiadomości naraz.
3. Zamień `\n` na `\0` zamiast polegać na `strlen()` na nie-terminowanym buforze.
4. `memmove` (nie `memcpy`) przy przesuwaniu resztek, bo zakresy się nakładają.

```c
#define BUF_SIZE 1024

// wiadomosc: bufor per-polaczenie (np. wiadomosci[i] z tablicy[POLL][BUF_SIZE])
// przeczytano: wskaznik na licznik per-polaczenie (ile bajtow juz zbuforowanych)
// zwraca: 1 = ok (dalej czytamy), 0 = EOF (fd mozna zamknac)
int dziel_wiadomosci(int fd, char* wiadomosc, int* przeczytano, void* kontekst)
{
    char buf[BUF_SIZE];
    int n = read(fd, buf, BUF_SIZE);
    if (n == 0) return 0;                               // EOF
    if (n == -1 && errno == EAGAIN) return 1;            // brak danych teraz, nie EOF
    if (n == -1) ERR("read");

    memcpy(wiadomosc + *przeczytano, buf, n);
    *przeczytano += n;

    int start = 0;
    for (int i = 0; i < *przeczytano; i++)
    {
        if (wiadomosc[i] == '\n')
        {
            wiadomosc[i] = '\0';
            obsluz_wiadomosc(wiadomosc + start, kontekst);   // <- Twoja funkcja do konkretnego zadania
            start = i + 1;
        }
    }

    int reszta = *przeczytano - start;
    memmove(wiadomosc, wiadomosc + start, reszta);
    *przeczytano = reszta;
    return 1;
}
```

---

## Parsowanie liczby z fragmentu tekstu (bez `atoi` na nie-terminowanym buforze)

```c
int parsuj_liczbe(char* s, int start, int end)   // [start, end)
{
    int wynik = 0;
    for (int i = start; i < end; i++)
    {
        if (s[i] < '0' || s[i] > '9') return -1;   // sygnalizuj blad wywolujacemu
        wynik = wynik * 10 + (s[i] - '0');
    }
    return wynik;
}
```

---

## mmap na strukturę (zamiast ręcznych offsetów bajtowych)

```c
struct rekord
{
    uint32_t pole_a;
    uint32_t pole_b;
};
// WAZNE jesli mieszasz rozne rozmiary typow w strukturze (char obok int itp.):
// dodaj __attribute__((packed)) do definicji struct, zeby wylaczyc padding.

struct rekord* mapa = mmap(NULL, rozmiar_pliku, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
if (mapa == MAP_FAILED) ERR("mmap");

mapa[i].pole_a += 1;              // normalny dostep do pola, kompilator liczy offset
msync(mapa, rozmiar_pliku, MS_SYNC);
munmap(mapa, rozmiar_pliku);
```
`mmap` rób **na początku programu** (zaraz po otwarciu pliku), jeśli masz pisać do mapy
w trakcie działania (np. w pętli `poll`) — nie dopiero na końcu.

Sprawdzenie zgodności rozmiaru struktury ze specyfikacją zadania:
```c
_Static_assert(sizeof(struct rekord) == 8, "zly rozmiar struktury - sprawdz padding");
```

---

## readv / writev (scatter-gather, kilka buforów w jednym wywołaniu)

```c
struct iovec iov[2];
iov[0].iov_base = &naglowek;         iov[0].iov_len = sizeof(naglowek);
iov[1].iov_base = bufor_danych;      iov[1].iov_len = rozmiar_danych;

ssize_t n = readv(fd, iov, 2);       // albo writev(fd, iov, 2);
if (n == -1) ERR("readv");
```
Jeśli czytasz tablicę rekordów o stałym rozmiarze na raz (typowe w tych zadaniach):
```c
struct iovec iov[LICZBA_REKORDOW];
struct rekord* tab[LICZBA_REKORDOW];
for (int i = 0; i < LICZBA_REKORDOW; i++)
{
    tab[i] = malloc(sizeof(struct rekord));
    iov[i].iov_base = tab[i];
    iov[i].iov_len = sizeof(struct rekord);
}
readv(fd, iov, LICZBA_REKORDOW);
// pamietaj o free(tab[i]) na koniec dla kazdego!
```

---

## Bezpieczny `write`/`read` — obsługa zapisów/odczytów częściowych

Dla dużych buforów `write()`/`read()` mogą zwrócić mniej bajtów niż prosiłeś — pętla naprawcza:

```c
ssize_t write_all(int fd, const void* buf, size_t len)
{
    size_t zapisano = 0;
    while (zapisano < len)
    {
        ssize_t n = write(fd, (const char*)buf + zapisano, len - zapisano);
        if (n == -1)
        {
            if (errno == EINTR) continue;   // przerwane sygnalem, sprobuj ponownie
            return -1;
        }
        zapisano += n;
    }
    return zapisano;
}
```

---

## Szablon pętli `poll` (multipleksowanie kilku FIFO)

```c
int n_fds = argc - OFFSET;
struct pollfd fds[n_fds];
for (int i = 0; i < n_fds; i++)
{
    fds[i].fd = open_fifo_rdwr(argv[i + OFFSET]);
    fds[i].events = POLLIN;
}

int aktywne = n_fds;
while (aktywne > 0)
{
    int ret = poll(fds, n_fds, TIMEOUT_MS);
    if (ret == -1)
    {
        if (errno == EINTR) continue;    // sygnal przerwal poll, sprobuj jeszcze raz
        ERR("poll");
    }
    if (ret == 0)
    {
        printf("Timeout - cisza\n");
        continue;
    }
    for (int i = 0; i < n_fds; i++)
    {
        if (fds[i].fd == -1) continue;              // ten fd juz wylaczony, pomin
        if (fds[i].revents & (POLLIN | POLLHUP))
        {
            int zyje = dziel_wiadomosci(fds[i].fd, wiadomosci[i], &przeczytano[i], NULL);
            if (!zyje)
            {
                close(fds[i].fd);
                fds[i].fd = -1;                      // WAZNE: poll() ignoruje fd == -1
                aktywne--;
            }
        }
    }
}
```
Kluczowe: `fds[i].fd = -1` po zamknięciu — `poll()` po prostu olewa ujemne fd (nie liczy ich,
nie błąd), więc nie trzeba przesuwać/kurczyć tablicy. Ale `poll()` z samymi `-1` (albo
`timeout == -1`) będzie czekać w nieskończoność, bo nic nigdy nie wygeneruje zdarzenia.

---

## `timerfd` — osobny "zegar" jako dodatkowy fd w `poll`

```c
#include <sys/timerfd.h>

int tfd = timerfd_create(CLOCK_MONOTONIC, 0);
struct itimerspec its = {
    .it_value    = {.tv_sec = 2, .tv_nsec = 0},   // pierwsze zdarzenie za 2s
    .it_interval = {.tv_sec = 2, .tv_nsec = 0},   // potem co 2s (0,0 = tylko raz)
};
timerfd_settime(tfd, 0, &its, NULL);

// dorzuc do tablicy fds obok FIFO, events = POLLIN

// gdy poll zglosi POLLIN na tym fd:
uint64_t ile_razy;
read(tfd, &ile_razy, sizeof(ile_razy));   // KONIECZNE - inaczej poll bedzie wracal od razu w kolko
```

---

## `getopt` — szablon parsowania argumentów

```c
int c;
char* s_arg = NULL;
int   p_arg = -1;

// "s:p:" -> -s wymaga argumentu, -p wymaga argumentu, brak dwukropka = flaga bez argumentu
while ((c = getopt(argc, argv, "s:p:")) != -1)
{
    switch (c)
    {
        case 's': s_arg = optarg; break;
        case 'p': p_arg = atoi(optarg); break;
        case '?': fprintf(stderr, "Nieznana opcja\n"); exit(EXIT_FAILURE);
        default: break;
    }
}
// argv[optind] = pierwszy argument pozycyjny PO opcjach
```

### Trik: gdy pierwszy argument pozycyjny (np. plik) MUSI być na `argv[1]`,
### ale `getopt` na macOS/BSD nie "przeskakuje" przez niego (nie permutuje):

```c
char* plik = argv[1];               // zawsze argv[1], bez wzgledu na getopt
int c;
while ((c = getopt(argc - 1, argv + 1, "s:p:")) != -1)   // parsuj reszte, z przesunieciem
{
    // optarg/optind dzialaja normalnie, ale optind liczy sie wzgledem (argv+1)
    // jesli recznie bierzesz kolejny argv (np. dla opcji z dwoma wartosciami),
    // prawdziwy indeks w oryginalnym argv to: optind + 1
}
```

### Opcja z DWOMA argumentami (np. `-a tytul rozmiar`) — `getopt` tego nie umie sam:

```c
case 'a':
    a_arg1 = optarg;                 // pierwszy argument getopt daje normalnie
    a_arg2 = argv[optind];           // drugi bierzesz recznie
    optind++;                        // i reczne przesuwasz optind, inaczej nastepna opcja sie rozjedzie
    break;
```

---

## Checklist błędów, które już kiedyś popełniłeś (sprawdź przed oddaniem)

- [ ] Czy `wiadomosc`/bufor tekstowy jest null-terminowany PRZED `strlen()`/`%s`?
- [ ] Czy w pętli dzielącej wiadomości skanujesz CAŁY zbuforowany tekst, nie tylko nowo wczytany kawałek?
- [ ] Czy `*licznik += n;` i `memcpy(..., *licznik, ...)` są w dobrej kolejności (najpierw memcpy pod stare `*licznik`, potem zwiększenie)?
- [ ] Czy liczby z tekstu parsujesz cyfra-po-cyfrze (`*10 + cyfra-'0'`), a nie kopiujesz surowych bajtów ASCII do pola binarnego?
- [ ] Czy przy `mmap` na strukturę przesuwasz się przez `tablica[i]`/`sizeof(struct)`, a nie ręcznie `+= 4`?
- [ ] Czy struktura z mieszanymi typami (char+int) ma `__attribute__((packed))`, jeśli zadanie wymaga dokładnego rozmiaru bez paddingu?
- [ ] Czy po `close(fds[i].fd)` ustawiasz `fds[i].fd = -1` zamiast zostawić stary numer (poll() odpytałby zamkniety fd)?
- [ ] Czy sprawdzasz `argc` na początku `main`, zanim dotkniesz `argv[2]` i dalej?
- [ ] Czy po `write()`/`read()` sprawdzasz wartość zwracaną (częściowe zapisy, `EINTR`, `EAGAIN`)?
- [ ] Czy wołasz `munmap`/`close` na końcu dla wszystkiego, co otworzyłeś/zmapowałeś?

---

## 🆘 FUNKCJE AWARYJNE — wklej całość, nic więcej nie musisz pisać

Te cztery funkcje są w pełni samodzielne — nie zależą od żadnych innych helperów z tej
ściągawki. Wklejasz, wołasz, działa. Jeśli jesteś w czasowym kryzysie na labach, to jest
to miejsce, po które sięgasz najpierw.

### `multiplex_read_lines` — zastępuje CAŁĄ pętlę poll+bufor+split jednym wywołaniem

To jest "rozwiązanie" dokładnie tego buga, który psułeś trzy razy pod rząd w tej sesji
(dzielenie strumienia na linie). Dajesz tablicę już otwartych, nieblokujących fd i funkcję
callback — reszta (poll, buforowanie, EAGAIN, POLLHUP, zamykanie) dzieje się sama w środku.

```c
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

// fds: tablica JUZ otwartych deskryptorow (musza miec O_NONBLOCK!)
// n: ile fd w tablicy
// timeout_ms: po ilu ms braku JAKIEJKOLWIEK aktywnosci funkcja sie podda (-1 = czekaj wiecznie)
// handler: wywolywany raz na kazda kompletna linie; dostaje indeks fd w tablicy i tresc linii
// zwraca: 0 gdy wszystkie fd daly EOF, -1 gdy wyszlo przez timeout
int multiplex_read_lines(int* fds, int n, int timeout_ms,
                          void (*handler)(int ktory_fd, char* linia))
{
    struct pollfd pfds[n];
    char** bufory = malloc(n * sizeof(char*));
    int* dlugosc = calloc(n, sizeof(int));
    int pojemnosc = 4096;
    for (int i = 0; i < n; i++)
    {
        pfds[i].fd = fds[i];
        pfds[i].events = POLLIN;
        bufory[i] = malloc(pojemnosc);
    }

    int aktywne = n;
    int wynik = 0;
    while (aktywne > 0)
    {
        int ret = poll(pfds, n, timeout_ms);
        if (ret == -1)
        {
            if (errno == EINTR) continue;
            wynik = -1;
            break;
        }
        if (ret == 0) { wynik = -1; break; }

        for (int i = 0; i < n; i++)
        {
            if (pfds[i].fd == -1) continue;
            if (!(pfds[i].revents & (POLLIN | POLLHUP))) continue;

            char tmp[1024];
            int r = read(pfds[i].fd, tmp, sizeof(tmp));
            if (r == 0 || (r == -1 && errno != EAGAIN))
            {
                pfds[i].fd = -1;
                aktywne--;
                continue;
            }
            if (r == -1) continue;   // EAGAIN - nic teraz nie ma, nie blad

            while (dlugosc[i] + r > pojemnosc)
            {
                pojemnosc *= 2;
                bufory[i] = realloc(bufory[i], pojemnosc);
            }
            memcpy(bufory[i] + dlugosc[i], tmp, r);
            dlugosc[i] += r;

            int start = 0;
            for (int j = 0; j < dlugosc[i]; j++)
            {
                if (bufory[i][j] == '\n')
                {
                    bufory[i][j] = '\0';
                    handler(i, bufory[i] + start);
                    start = j + 1;
                }
            }
            int reszta = dlugosc[i] - start;
            memmove(bufory[i], bufory[i] + start, reszta);
            dlugosc[i] = reszta;
        }
    }

    for (int i = 0; i < n; i++) free(bufory[i]);
    free(bufory);
    free(dlugosc);
    return wynik;
}
```

**Użycie:**
```c
void moj_handler(int ktory_fd, char* linia)
{
    printf("Z fd nr %d: %s\n", ktory_fd, linia);
    // tu wklej swoja logike switch(linia[0]) itd.
}

int main(int argc, char* argv[])
{
    int fds[argc - 2];
    for (int i = 0; i < argc - 2; i++)
        fds[i] = open(argv[i + 2], O_RDWR | O_NONBLOCK);   // pamietaj O_NONBLOCK

    multiplex_read_lines(fds, argc - 2, 4000, moj_handler);
    return 0;
}
```
Jeśli potrzebujesz w `handler` dostępu do np. `mmap`-owanej struktury — zrób ją zmienną
globalną na czas zadania. To brzydkie w "poważnym" kodzie, ale na labach pod presją czasu
działające rozwiązanie jest lepsze niż ładne, które nie zdążysz napisać.

### `dump_hex` — gdy coś w pliku binarnym/mmap wygląda jak śmieci, zobacz surowe bajty

```c
#include <stdio.h>

void dump_hex(const void* buf, size_t len)
{
    const unsigned char* p = buf;
    for (size_t i = 0; i < len; i += 16)
    {
        printf("%04zx: ", i);
        for (size_t j = i; j < i + 16 && j < len; j++) printf("%02x ", p[j]);
        printf(" | ");
        for (size_t j = i; j < i + 16 && j < len; j++)
            printf("%c", (p[j] >= 32 && p[j] < 127) ? p[j] : '.');
        printf("\n");
    }
}
```
Wołasz `dump_hex(mapa, SIZE);` albo `dump_hex(buf, n);` w dowolnym miejscu kodu, gdy coś
"nie wygląda dobrze", i widzisz dokładnie, co tam faktycznie siedzi bajt po bajcie —
zamiast zgadywać na podstawie `printf("%s")`, który może Cię okłamać.

### `mmap_open_file` — otwiera, tworzy (jeśli trzeba), waliduje rozmiar i mapuje w jednym wywołaniu

```c
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void* mmap_open_file(const char* path, size_t size)
{
    int fd = open(path, O_RDWR);
    if (fd == -1)
    {
        fd = open(path, O_RDWR | O_CREAT, 0644);
        if (fd == -1) { perror("open"); exit(1); }
        char* zeros = calloc(1, size);
        write(fd, zeros, size);
        free(zeros);
    }
    struct stat st;
    fstat(fd, &st);
    if ((size_t)st.st_size != size) { fprintf(stderr, "zly rozmiar pliku: %s\n", path); exit(1); }

    void* p = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) { perror("mmap"); exit(1); }
    close(fd);   // po mmap fd juz niepotrzebny, mapowanie zyje dalej samo
    return p;
}
```
**Użycie:** `struct komora* mapa = mmap_open_file(argv[1], sizeof(struct komora) * 16);`
— jedna linijka zamiast dziesięciu. Pamiętaj tylko zrobić `munmap(mapa, size)` na końcu.

### `open_fifo_writer_retry` — jeśli TY masz pisać do FIFO, a czytelnik może się jeszcze nie zgłosić

```c
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int open_fifo_writer_retry(const char* path, int max_prob, int delay_ms)
{
    for (int i = 0; i < max_prob; i++)
    {
        int fd = open(path, O_WRONLY | O_NONBLOCK);
        if (fd != -1) return fd;
        if (errno != ENXIO) { perror("open"); exit(1); }
        usleep(delay_ms * 1000);
    }
    fprintf(stderr, "nikt nie podlaczyl sie do odczytu z %s\n", path);
    exit(1);
}
```
**Użycie:** `int fd = open_fifo_writer_retry("fifo1", 50, 100);` — spróbuje 50 razy co
100ms (czyli max 5 sekund), zanim się podda. Dostosuj liczby do zadania.
