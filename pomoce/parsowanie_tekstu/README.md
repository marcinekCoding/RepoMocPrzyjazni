# parsowanie_tekstu — dzielenie napisow, liczby z tekstu, formaty wiadomosci

| plik | kiedy |
|---|---|
| `podziel_napis.c` + `zwolnij_napisy.c` | podziel po spacjach (max 256) |
| `dziel_po_separatorach.c` | podziel po dowolnych separatorach (`":,"`), bez limitu; zwalniaj `zwolnij_napisy` |
| `zlep_napisy.c` | sklej n napisow w nowy (`zlep_napisy(3, a, "/", b)`) |
| `napisy_drobne.c` | usun `\n` z konca, przytnij spacje, `zaczyna_sie_od`, `konczy_sie_na`, dlugosc bez dopelnienia spacjami |
| `obetnij_do_bajtow.c` | obetnij napis do N bajtow (tytul max 64) |
| `parsuj_liczbe.c` | liczba z fragmentu bufora bez `'\0'`, czytanie liczb po kolei (`"12,340"`), `same_cyfry` |
| `podziel_klucz_wartosc.c` | `"pakiet==1.0"`, `"title:Histories"` -> klucz + wartosc |
| `meta_pole.c` | znajdz w PLIKU linie `klucz:wartosc` i zwroc wartosc |
| `waliduj_wiadomosc.c` | wiadomosci z FIFO: `"NN:tresc"` (numer czujnika) i `"G:12,340"` (typ + liczby) |
| `zamien_wystapienia.c` | zamien `"raw~berries"` -> `"raspberries"` w buforze/mmap, szukanie w buforze bez `'\0'` |
| `NAPISY_ktora_funkcja.txt` | Twoja sciaga: strlen/strncpy/memcpy/snprintf/strtok — ktorej kiedy uzyc |
