# katalogi_sciezki — readdir, chodzenie po drzewie, mkdir, sklejanie sciezek

| plik | kiedy |
|---|---|
| `dla_kazdego_wpisu.c` | **readdir z callbackiem** + opcjonalna glebokosc (`-d N`), dostajesz `lstat` |
| `przejdz_rekurencyjnie.c` | rekurencyjnie, callback tylko dla zwyklych plikow, mozna przerwac |
| `przejdz_nftw.c` | to samo przez `nftw` (z adapterem na wlasne dane) |
| `policz_typy.c` | ile plikow / katalogow / linkow w katalogu |
| `przenies_pasujace.c` | przenies pliki spelniajace warunek do podkatalogu (np. wykonywalne -> `bin/`) |
| `usun_rekurencyjnie.c` | `rm -rf` |
| `mkdir_nowy.c` | mkdir, ktory MA sie nie udac gdy istnieje |
| `mkdir_jesli_trzeba.c` | mkdir, istniejacy katalog to OK |
| `utworz_katalogi.c` | `mkdir -p a/b/c` |
| `dolacz_sciezki.c` / `../parsowanie_tekstu/zlep_napisy.c` | sklej `katalog` + `/` + `nazwa` |
| `nazwa_pliku.c` | `"a/b/c.txt"` -> `"c.txt"` |
| `rozszerzenie.c` | `"c.txt"` -> `"txt"` (filtr `-e txt`) |
| `wejdz_do_katalogu.c` + `wroc_do_katalogu.c` | `chdir` z zapamietaniem, skad przyszedles |

Szybko inline: `snprintf(sciezka, sizeof(sciezka), "%s/%s", katalog, e->d_name);`
