# pomoce — moje gotowe funkcje do SOP1 (kopiuj i wklejaj)

Kazdy plik `.c` jest **samodzielny**: ma swoje `#include` i (jesli trzeba) makro `ERR`
w `#ifndef`, wiec mozna wkleic kilka naraz nad `main` bez konfliktow.
Na poczatku pliku jest komentarz: co robi, co zwraca i przyklad uzycia.
Kazdy podfolder ma swoj `README.md` z tabelka "plik -> kiedy".

| folder | temat |
|---|---|
| `00_szablon/` | pusty `main.c` z kompletem naglowkow + pelna sciagawka + checklist |
| `argumenty_getopt/` | argc/argv, `getopt`, liczby z argumentow, zmienne srodowiskowe |
| `parsowanie_tekstu/` | dzielenie napisow, liczby z tekstu, `klucz:wartosc`, formaty wiadomosci `NN:...`, zamiana tekstu |
| `poll_fifo/` | `poll`, FIFO, `O_NONBLOCK`, dzielenie strumienia na linie, timeout, timerfd |
| `pliki_read_write/` | `open/read/write`, caly plik, kopiowanie, plik stalego rozmiaru, linie po 64 B, `getline` |
| `mmap/` | mapowanie pliku, tablica struktur w mmap, `dump_hex` |
| `binarne_rekordy_readv/` | rekordy `uint32 + tytul`, `readv/writev`, `pread/pwrite` |
| `katalogi_sciezki/` | `readdir`, rekurencja, `nftw`, `mkdir -p`, `rm -rf`, sciezki |
| `symlinki/` | `symlink`, `readlink`, sciezki wzgledne |
| `stat_typy_plikow/` | `stat/lstat`, typ, rozmiar, uprawnienia |

## Szukam szybko...
- **"czytaj z kilku FIFO naraz"** -> `poll_fifo/petla_poll.c` + `poll_fifo/dziel_wiadomosci.c`
- **"wiadomosci w formacie 07:tekst"** -> `parsowanie_tekstu/waliduj_wiadomosc.c`
- **"plik ma miec dokladnie N bajtow / utworz jesli nie ma"** -> `pliki_read_write/otworz_lub_utworz_o_rozmiarze.c`
- **"zmapuj plik i zmieniaj pola"** -> `mmap/mmap_otworz_plik.c` + `mmap/mmap_struktura.c`
- **"baza rekordow tytul + rozmiar"** -> `binarne_rekordy_readv/rekordy_stalej_dlugosci.c`
- **"przejdz po katalogu (rekurencyjnie)"** -> `katalogi_sciezki/dla_kazdego_wpisu.c`
- **"opcje -p -e -o"** -> `argumenty_getopt/getopt_szablon.c`

Sprawdzenie, ze wszystko sie kompiluje: `./sprawdz.sh`
