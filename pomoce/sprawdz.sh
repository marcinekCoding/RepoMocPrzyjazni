#!/bin/sh
# Kompiluje (tylko sprawdzenie skladni) kazdy plik .c osobno. Uruchom: ./sprawdz.sh
cd "$(dirname "$0")" || exit 1
CC=${CC:-cc}
bledy=0
err=$(mktemp)
for f in */*.c; do
    if ! $CC -std=gnu11 -Wall -Wextra -fsyntax-only "$f" 2>"$err"; then
        echo "BLAD: $f"; cat "$err"; bledy=$((bledy + 1))
    elif [ -s "$err" ]; then
        echo "ostrzezenia: $f"; cat "$err"
    fi
done
rm -f "$err"
echo "Gotowe, plikow z bledami: $bledy"
