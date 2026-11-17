#include <sys/stat.h>

/* Wklej nad main. Tworzy katalog. Zwraca -1 takze wtedy, gdy juz istnieje
 * (errno == EEXIST). Do "index", ktore nie moze byc drugi raz:
 * if (mkdir_nowy("index", 0777) != 0) ERR("mkdir");
 */
int mkdir_nowy(const char *sciezka, mode_t tryb)
{
    return mkdir(sciezka, tryb);
}
