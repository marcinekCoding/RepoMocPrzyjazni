# pliki_read_write — open/read/write, cale pliki, pliki o stalym rozmiarze

| plik | kiedy |
|---|---|
| `czytaj_calosc.c` / `zapisz_calosc.c` | read/write w petli dokladnie N bajtow (0 / -1) |
| `bulk_read_write.c` | to samo, wersja z wykladu (zwraca liczbe bajtow) |
| `czytaj_do_eof.c` | przepisz fd -> fd do konca (cat, kopiowanie) |
| `caly_plik.c` | wczytaj caly plik do napisu, zapisz/dopisz napis, skopiuj plik |
| `otworz_z_uprawnieniami.c` | `open` z dokladnymi bitami (umask na chwile 0) |
| `otworz_lub_utworz_o_rozmiarze.c` | **plik danych stalego rozmiaru**: utworz+wypelnij albo sprawdz rozmiar; `% RECORD` |
| `linia_stalej_szerokosci.c` | plik z liniami po 64 znaki (sop-cake): wpisz/czytaj linie nr N (`pwrite`/`pread`) |
| `losowy_plik.c` | plik o rozmiarze N z losowymi literami i dokladnymi uprawnieniami |
| `linie_w_pliku.c` | `getline`: znajdz linie z prefiksem, usun linie (tmp + rename), callback na kazda linie |
