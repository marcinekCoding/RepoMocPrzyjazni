#include <stddef.h>
#include <stdio.h>

/* Gdy w pliku binarnym / mapie "cos jest nie tak" — pokaz surowe bajty
 * (hex + znaki), zamiast zgadywac z printf("%s").
 *   dump_hex(mapa, 128);   dump_hex(buf, n);
 */
void dump_hex(const void *buf, size_t len)
{
    const unsigned char *p = buf;
    for (size_t i = 0; i < len; i += 16)
    {
        printf("%04zx: ", i);
        for (size_t j = i; j < i + 16; j++)
        {
            if (j < len)
                printf("%02x ", p[j]);
            else
                printf("   ");
        }
        printf(" | ");
        for (size_t j = i; j < i + 16 && j < len; j++)
            printf("%c", (p[j] >= 32 && p[j] < 127) ? p[j] : '.');
        printf("\n");
    }
}
