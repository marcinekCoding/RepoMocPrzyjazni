# argumenty_getopt — argc/argv, opcje, liczby z argumentow, zmienne srodowiskowe

| plik | kiedy |
|---|---|
| `getopt_szablon.c` | zwykle `-p X -e Y -o`, opcje powtarzalne, `argv[optind]` po opcjach |
| `getopt_plik_na_argv1.c` | plik ZAWSZE w `argv[1]`, opcje za nim; opcja z dwoma wartosciami (`-a tytul rozmiar`) |
| `liczba_z_argumentu.c` | `optarg` -> liczba z walidacja (`strtol`), tez osemkowe uprawnienia `0644` |
| `zmienne_srodowiskowe.c` | `getenv` z domyslna, `setenv` + EINVAL, wypisanie `environ`, wyjscie do pliku ze zmiennej |
